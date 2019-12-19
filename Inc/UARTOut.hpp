
#ifndef    UART_OUT_INCLUDED
#define    UART_OUT_INCLUDED

#include <cstdint>

namespace exlib
{

void UART_Print( const char* message );
void UART_HexPrint( const uint8_t* hex, int len );

class SerialOut
{
public:

private:
};

extern SerialOut sout;

SerialOut& operator<<( SerialOut& out, const char* str );
SerialOut& operator<<( SerialOut& out, uint16_t value );

}


#endif    // UART_OUT_INCLUDED
