
#include "UARTOut.hpp"
#include "usart.h"
#include <cstring>

namespace exlib
{
// Serial Out の実体
SerialOut sout;

void UART_Send( const uint8_t* tx, int len )
{
    HAL_UART_Transmit( &huart1, const_cast<uint8_t*>(tx), len, 1000 );
}

void UART_Print( const char* message )
{
    HAL_UART_Transmit( &huart1, reinterpret_cast<uint8_t*>(const_cast<char*>(message)), std::strlen(message), 1000 );
}

void UART_HexPrint( const uint8_t* hex, int len )
{
	static const char table[] = {
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
	};

	int i = 0;
	uint8_t val, c[2];
	for( i = 0; i < len; ++i ){
		val = hex[i];
		c[0] = table[val >> 4];
		c[1] = table[0xF & val];
		UART_Send( c, 2 );
	}
}

SerialOut& operator<<( SerialOut& out, const char* str )
{
	if( str ){
    	UART_Print( str );
	}

    return out;
}

SerialOut& operator<<( SerialOut& out, uint16_t value )
{
    uint8_t bytes[2] = { ((value >> 8) & 0xFF), (value & 0xFF) };

    UART_HexPrint( bytes, 2 );
    return out;
}

}
