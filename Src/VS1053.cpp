
#include "VS1053.hpp"
#include <cstdint>
#include "gpio.h"
#include "spi.h"
#include "Timer.hpp"
#include "UARTOut.hpp"

static constexpr uint32_t sk_SPI_Send_Timeout = 1000;
static constexpr uint32_t sk_SPI_Recv_Timeout = 1000;
static constexpr uint32_t sk_Busy_Timeout = 1000;

VS1053_Drv_SPI VS1053_Drv_SPI::s_Driver;

VS1053_Drv_SPI::VS1053_Drv_SPI()
{}

VS1053_Drv_SPI::~VS1053_Drv_SPI()
{}

VS1053_Drv_SPI& VS1053_Drv_SPI::Instance()
{
    return s_Driver;
}

bool VS1053_Drv_SPI::CommandSelect()
{
    HAL_GPIO_WritePin(GPIOA, XCS_Pin, GPIO_PIN_RESET);
    return true;
}

void VS1053_Drv_SPI::CommandRelease()
{
    HAL_GPIO_WritePin(GPIOA, XCS_Pin, GPIO_PIN_SET);
}

bool VS1053_Drv_SPI::DataSelect()
{
    HAL_GPIO_WritePin(GPIOA, XDCS_Pin, GPIO_PIN_RESET);
    return true;
}

void VS1053_Drv_SPI::DataRelease()
{
    HAL_GPIO_WritePin(GPIOA, XDCS_Pin, GPIO_PIN_SET);
}

bool VS1053_Drv_SPI::IsBusy() const
{
    return HAL_GPIO_ReadPin(GPIOA, DREQ_Pin) == GPIO_PIN_RESET;
}

bool VS1053_Drv_SPI::Send( const uint8_t* data, uint16_t len )
{
    HAL_StatusTypeDef status = HAL_SPI_Transmit( &hspi1, const_cast<uint8_t*>(data), static_cast<uint16_t>(len), sk_SPI_Send_Timeout );
    return status == HAL_OK;
}

bool VS1053_Drv_SPI::Recv( uint8_t* data, uint16_t len )
{
	// HAL_SPI_Receive 関数では、0x00を送信してしまうため正常にコマンド送信ができない。
	// 受信時も0xFFを送信するように HAL_SPI_TransmitReceive を使う。
	constexpr int k_TxBufSize = 16;
	uint8_t txbuf[k_TxBufSize] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
								  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	uint16_t remain_len = len;
	uint8_t* dstp = data;

	while( remain_len >= k_TxBufSize ){
		if( HAL_SPI_TransmitReceive( &hspi1, txbuf, dstp, k_TxBufSize, sk_SPI_Recv_Timeout ) != HAL_OK ){
			return false;
		}
		remain_len -= k_TxBufSize;
		dstp += 16;
	}
	if( remain_len > 0 ){
		if( HAL_SPI_TransmitReceive( &hspi1, txbuf, dstp, static_cast<uint16_t>(remain_len), sk_SPI_Recv_Timeout ) != HAL_OK ){ 
			return false;
		}
	}

    return true;
}
    
bool VS1053_Drv_SPI::SendRecv( const uint8_t* senddata, uint8_t* recvdata, uint16_t recvlen )
{
    return HAL_SPI_TransmitReceive( &hspi1, const_cast<uint8_t*>(senddata), recvdata, recvlen, sk_SPI_Recv_Timeout ) == HAL_OK;
}




VS1053_Drv::VS1053_Drv()
{}

VS1053_Drv::~VS1053_Drv()
{}

bool VS1053_Drv::Initialize()
{
    HAL_GPIO_WritePin(GPIOA, AUDIO_RESET_Pin, GPIO_PIN_SET);
    exlib::sout <<  "VS1053_Drv::Initialize() called.\n";

    constexpr uint16_t k_Status_WhenInitCompleted = 0x0040;
    uint16_t status = 0x0000;
    {
        MsTimer timer;

        while( status != k_Status_WhenInitCompleted ){
            if( !ReadSCI( SCI_STATUS, &status ) ){
                break;
            }
            exlib::sout << "Staus: " << status << " \n";
            if( timer.IsElapsed( sk_Busy_Timeout ) ){
                exlib::sout << "VS1053 initialize timeout.\n";
                return false;
            }
        }
    }
    
    if( status != k_Status_WhenInitCompleted ){
        exlib::sout << "VS1053 init command failed.\n";
        return false;
    }
    
    // クロックのセット
    // 3.5 * XTALI, マルチプライヤ追加 2.0x まで
    if( !setClock() ){
        return false;
    }
    // デフォルトのボリュームをセット
    // 初期値は最大ボリュームなのである程度静かにしておく
    if( !SetVolume( sk_DefaultVolume, sk_DefaultVolume ) ){
        return false;
    }

    return true;
}

bool VS1053_Drv::ReadSCI( SCI_Register regaddr, uint16_t* data )
{
    VS1053_Drv_SPI& driver = VS1053_Drv_SPI::Instance();
    {
        MsTimer timer;
        while( driver.IsBusy() ){
            if( timer.IsElapsed( sk_Busy_Timeout ) ){
                exlib::sout << "ReadSCI command timeout.\n";
                return false;
            }
        }
    }

    const uint8_t senddata[4] = { sk_SCI_ReadInstruction, regaddr, 0xFF, 0xFF };
    uint8_t recvdata[4];
    bool is_command_send_ok = false;

    driver.CommandSelect();
    is_command_send_ok = driver.SendRecv( senddata, recvdata, 4 );
    driver.CommandRelease();

    if( is_command_send_ok ){
        exlib::sout << "Send ReadSCI command OK.\n";
        *data = (recvdata[2] << 8) | recvdata[3];
    }

    return is_command_send_ok;
}

bool VS1053_Drv::WriteSCI( SCI_Register regaddr, uint16_t data )
{
    VS1053_Drv_SPI& driver = VS1053_Drv_SPI::Instance();
    {
        MsTimer timer;
        while( driver.IsBusy() ){
            if( timer.IsElapsed( sk_Busy_Timeout ) ){
                return false;
            }
        }
    }

    const uint8_t senddata[4] = { sk_SCI_WriteInstruction, regaddr, static_cast<uint8_t>(data >> 8), static_cast<uint8_t>(data) };

    bool is_command_send_ok = false;

    driver.CommandSelect();
    is_command_send_ok = driver.Send( senddata, 4 );
    driver.CommandRelease();

    return is_command_send_ok;
}

bool VS1053_Drv::WriteSDI( const uint8_t* data, uint16_t len )
{
    VS1053_Drv_SPI& driver = VS1053_Drv_SPI::Instance();
    {
        MsTimer timer;
        while( driver.IsBusy() ){
            if( timer.IsElapsed( sk_Busy_Timeout ) ){
                return false;
            }
        }
    }

    bool is_data_send_ok = false;

    driver.DataSelect();
    is_data_send_ok = driver.Send( data, len );
    driver.DataRelease();

    return is_data_send_ok;
}

uint16_t VS1053_Drv::ReadWRAM( uint16_t addr )
{
    uint16_t wram = 0;
    WriteSCI( SCI_WRAMADDR, addr );
    ReadSCI( SCI_WRAM, &wram );

    return wram;
}

bool VS1053_Drv::IsBusy() const
{
    return VS1053_Drv_SPI::Instance().IsBusy();
}

bool VS1053_Drv::SetVolume( uint8_t left, uint8_t right )
{
    uint16_t volume = (static_cast<uint16_t>(left) << 8) | (static_cast<uint16_t>(right) & 0x00FF);
    return WriteSCI( SCI_VOL, volume );
}

uint8_t VS1053_Drv::ReadEndFillByte()
{
    return (ReadWRAM( sk_EndFillByte_Address ) & 0xFF);
}

void VS1053_Drv::SoftReset()
{

}

bool VS1053_Drv::setClock()
{
    return WriteSCI( SCI_CLOCKF, 0x4C00 );
}