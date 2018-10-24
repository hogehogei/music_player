
#include "sdcard/SDC_Drv_SPI.hpp"
#include "spi.h"

static constexpr uint32_t sk_SPI_Send_Timeout = 1000;
static constexpr uint32_t sk_SPI_Recv_Timeout = 1000;

SDC_Drv_SPI::SDC_Drv_SPI()
{}

SDC_Drv_SPI::~SDC_Drv_SPI()
{}

void SDC_Drv_SPI::CS_Hi()
{
    HAL_GPIO_WritePin( GPIOA, SDCARD_CS_Pin, GPIO_PIN_SET );
    // MMC/SDC の場合は SCLKに同期して DO信号の解放が行われる。
    // DO信号を確実に解放するために、1byte分クロックを送っておく。

    uint8_t tmp = 0;
    HAL_SPI_Transmit( &hspi1, &tmp, 1, sk_SPI_Send_Timeout );
}

void SDC_Drv_SPI::CS_Lo()
{
    HAL_GPIO_WritePin( GPIOA, SDCARD_CS_Pin, GPIO_PIN_RESET );
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
