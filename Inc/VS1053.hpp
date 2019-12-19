#ifndef VS1053_HPP_DEFINED
#define VS1053_HPP_DEFINED

#include <cstdint>

class VS1053_Drv_SPI
{
public:

    ~VS1053_Drv_SPI();
    VS1053_Drv_SPI& operator=( const VS1053_Drv_SPI& ) = delete;
    VS1053_Drv_SPI( const VS1053_Drv_SPI& ) = delete;

    static VS1053_Drv_SPI& Instance();

    bool CommandSelect();
    void CommandRelease();
    bool DataSelect();
    void DataRelease();

    bool IsBusy() const;

    bool Send( const uint8_t* data, uint16_t len );
    bool Recv( uint8_t* data, uint16_t len );
    bool SendRecv( const uint8_t* senddata, uint8_t* recvdata, uint16_t recvlen );

private:
    static VS1053_Drv_SPI s_Driver;
    VS1053_Drv_SPI();
};

class VS1053_Drv
{
public:

    enum SCI_Register
    {
        SCI_MODE        = 0x00,
        SCI_STATUS      = 0x01,
        SCI_BASS        = 0x02,
        SCI_CLOCKF      = 0x03,
        SCI_DECODE_TIME = 0x04,
        SCI_AUDATA      = 0x05,
        SCI_WRAM        = 0x06,
        SCI_WRAMADDR    = 0x07,
        SCI_HDAT0       = 0x08,
        SCI_HDAT1       = 0x09,
        SCI_AIADDR      = 0x0A,
        SCI_VOL         = 0x0B,
        SCI_AICTRL0     = 0x0C,
        SCI_AICTRL1     = 0x0D,
        SCI_AICTRL2     = 0x0E,
        SCI_AICTRL3     = 0x0F
    };

    static constexpr uint8_t sk_DefaultVolume = 80;

public:
    VS1053_Drv();
    ~VS1053_Drv() noexcept;

    bool Initialize();
    bool ReadSCI( SCI_Register regaddr, uint16_t* data );
    bool WriteSCI( SCI_Register regaddr, uint16_t data );
    bool WriteSDI( const uint8_t* data, uint16_t len );
    uint16_t ReadWRAM( uint16_t addr );

    bool IsBusy() const;
    bool SetVolume( uint8_t left, uint8_t right );
    uint8_t ReadEndFillByte();

    void SoftReset();
    
private:
        
    bool setClock();

    static constexpr uint8_t sk_SCI_ReadInstruction  = 0x03;
    static constexpr uint8_t sk_SCI_WriteInstruction = 0x02;
    static constexpr uint16_t sk_EndFillByte_Address = 0x1E06;
};

#endif    // VS1053_HPP_DEFINED