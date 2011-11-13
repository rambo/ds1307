// safety againts double-include
#ifndef ds1307_h
#define ds1307_h
#include <WProgram.h> 
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
        void begin(boolean wire_begin);
        boolean read_clock(); // This reads the RTC data to local buffer used by the 
        byte minute(); // 0-60
        byte second(); // 0-60
        byte hour(); // 0-23
        byte day(); // 1-31
        byte month(); // 1-12
        byte years(); // 0-99
        int year(); // DS1307_CENTURY + 0-99
        byte dow(); // 1-7
#ifndef DS1307_CONSERVE_SPACE
        char* iso_ts(); // YYYY-MM-DD HH:MM:SS
#endif
        boolean set_clock(byte years, byte month, byte day, byte hour, byte minute, byte second); // Note that years is 00-99
    
    protected:
        byte rtc_regs[7];
#ifndef DS1307_CONSERVE_SPACE
		char isobuffer[20];
#endif
};


extern ds1307 DS1307;


#endif
// *********** END OF CODE **********
