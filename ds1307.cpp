#include "ds1307.h"


void ds1307::begin(boolean wire_begin)
{
    i2c_device::begin(DS1307_ADDRESS, wire_begin);
    // Set 24H mode
    this->read_modify_write(0x2, B10111111, 0xFF);
}

boolean ds1307::read_clock()
{
    return read_many(0x0, 7, rtc_regs);
}

byte ds1307::second()
{
    return ((rtc_regs[0] & B01110000) >> 4) * 10 + (rtc_regs[0] & B00001111);
}
byte ds1307::minute()
{
    return ((rtc_regs[1] & B01110000) >> 4) * 10 + (rtc_regs[1] & B00001111);
}
byte ds1307::hour()
{
    return ((rtc_regs[2] & B00110000) >> 4) * 10 + (rtc_regs[2] & B00001111);
}
byte ds1307::dow()
{
    return (rtc_regs[3] & B00000111);
}
byte ds1307::day()
{
    return ((rtc_regs[4] & B00110000) >> 4) * 10 + (rtc_regs[4] & B00001111);
}
byte ds1307::month()
{
    return ((rtc_regs[5] & B00010000) >> 4) * 10 + (rtc_regs[5] & B00001111);
}
byte ds1307::years()
{
    return ((rtc_regs[6] & B11110000) >> 4) * 10 + (rtc_regs[6] & B00001111);
}

int ds1307::year()
{
    return DS1307_CENTURY + years();
}

char* ds1307::iso_ts()
{
    /**
     * This causes a weird compile error
    sprintf(isobuffer,"%04u-%02u-%02u %02u:%02u:%02u",
        this->year(),
        this->month(),
        this->day(),
        this->hour(),
        this->minute(),
        this->second()
    );
    */
    
    return isobuffer;
}


// Instance 
ds1307 DS1307 = ds1307();
