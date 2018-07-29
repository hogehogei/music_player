
#include "SDC_Drv_SPI.hpp"

SDC_Drv_SPI::SDC_Drv_SPI()
{}

SDC_Drv_SPI::~SDC_Drv_SPI()
{}

void SDC_Drv_SPI::CS_Hi()
{

}

void SDC_Drv_SPI::CS_Lo()
{

}

bool SDC_Drv_SPI::send( const uint8_t* data, uint32_t len )
{
    return true;
}

bool SDC_Drv_SPI::sendTransEnd()
{
    return true;
}

bool SDC_Drv_SPI::recv( uint8_t* data, uint32_t len )
{
    return true;
}

bool SDC_Drv_SPI::recvTransEnd()
{
    return true;
}
