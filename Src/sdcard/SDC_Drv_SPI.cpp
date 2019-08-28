#include "stm32f0xx_hal.h"
#include "sdcard/SDC_Drv_SPI.hpp"
#include "spi.h"

static constexpr uint32_t sk_SPI_Send_Timeout = 1000;
static constexpr uint32_t sk_SPI_Recv_Timeout = 1000;

SDC_Drv_SPI::SDC_Drv_SPI()
{}

SDC_Drv_SPI::~SDC_Drv_SPI()
{}

bool SDC_Drv_SPI::InitSlowSpeed()
{
	MX_SPI1_Init();

	return true;
}

bool SDC_Drv_SPI::InitFastSpeed()
{
	SPI1_Init_Fast();

	return true;
}

bool SDC_Drv_SPI::Select()
{
    HAL_GPIO_WritePin( GPIOA, SDCARD_CS_Pin, GPIO_PIN_RESET );

	uint8_t tmp = 0xFF;
    if( !send( &tmp, 1 ) ){
		return false;
	}

	if( waitReady() ){
		return true;
	}
	
	Release();
	return false;
}

void SDC_Drv_SPI::Release()
{
    HAL_GPIO_WritePin( GPIOA, SDCARD_CS_Pin, GPIO_PIN_SET );

    // MMC/SDC の場合は SCLKに同期して DO信号の解放が行われる。
    // DO信号を確実に解放するために、1byte分クロックを送っておく。
	uint8_t tmp = 0xFF;
	send( &tmp, 1 );
}

bool SDC_Drv_SPI::send( const uint8_t* data, uint32_t len )
{
    HAL_StatusTypeDef status = HAL_SPI_Transmit( &hspi1, const_cast<uint8_t*>(data), static_cast<uint16_t>(len), sk_SPI_Send_Timeout );
    return status == HAL_OK ? true : false;
}

bool SDC_Drv_SPI::recv( uint8_t* data, uint32_t len )
{
    HAL_StatusTypeDef status = HAL_SPI_Receive( &hspi1, data, static_cast<uint16_t>(len), sk_SPI_Recv_Timeout );
    return status == HAL_OK ? true : false;
}

bool SDC_Drv_SPI::flush()
{
    // 書き込みバッファは持たないので、特に何もしない
    return true;
}

bool SDC_Drv_SPI::waitReady()
{
	uint8_t tmp = 0;
	while(1){
		recv( &tmp, 1 );
		if( tmp == 0xFF ){
			break;
		}
	}

	return tmp == 0xFF;
}
