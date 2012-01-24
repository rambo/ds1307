// safety againts double-include
#ifndef ds1307_h
#define ds1307_h
#include <Arduino.h> 
#include <i2c_device.h>
#ifndef DS1307_ADDRESS
// 7 bit binary addr 1101000 0x68 -> 0xD0 write, 0xD1 read
#define DS1307_ADDRESS 0x68
#endif
#ifndef DS1307_CENTURY
#define DS1307_CENTURY 2000
#endif


// Stub extension for now
class ds1307 : public i2c_device
{
    public:
        void begin(uint8_t wire_begin);
        uint8_t read_clock(); // This reads the RTC data to local buffer used by the 
        uint8_t minute(); // 0-60
        uint8_t second(); // 0-60
        uint8_t hour(); // 0-23
        uint8_t day(); // 1-31
        uint8_t month(); // 1-12
        uint8_t years(); // 0-99
        int year(); // DS1307_CENTURY + 0-99
        uint8_t dow(); // 1-7
#ifndef DS1307_CONSERVE_SPACE
        char* iso_ts(); // YYYY-MM-DD HH:MM:SS
#endif
        uint8_t set_clock(uint8_t years, byte month, byte day, byte hour, byte minute, byte second); // Note that years is 00-99
    
    protected:
        uint8_t rtc_regs[7];
#ifndef DS1307_CONSERVE_SPACE
		char isobuffer[20];
#endif
};


extern ds1307 DS1307;


#endif
// *********** END OF CODE **********
