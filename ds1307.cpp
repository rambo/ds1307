#include "ds1307.h"


void ds1307::begin(boolean wire_begin)
{
    i2c_device::begin(DS1307_ADDRESS, wire_begin);
    /*
    // Set 24H mode
    this->read_modify_write(0x2, B10111111, 0xff);
    // Enabled clock
    this->read_modify_write(0x0, B10000000, 0x0);
    */
}

boolean ds1307::read_clock()
{
    return read_many(0x0, 7, rtc_regs);
}

boolean ds1307::set_clock(byte years, byte month, byte day, byte hour, byte minute, byte second)
{
    // FIXME: there is something wrong with my bit-magic here
    byte tmp[3];
    // Seconds register (also clock-halt which we disable at this point)
    tmp[0] = ((second / 10) << 4) | ((second - second / 10) & B00001111);
    // Minutes register
    tmp[1] = ((minute / 10) << 4) | ((minute - minute / 10) & B00001111);
    // Hours register (and set 24h mode)
    tmp[2] = B010000 | ((hour / 10) << 4) | ((hour - hour / 10) & B00001111);
    // Write the above values
    if (!this->write_many(0x0, 3, tmp))
    {
        return false;
    }
    // address 0x3 is skipped
    // day
    tmp[0] = ((day / 10) << 4) | ((day - day / 10) & B00001111);
    // month
    tmp[1] = ((month / 10) << 4) | ((month - month / 10) & B00001111);
    // year
    tmp[2] = ((years / 10) << 4) | ((years - years / 10) & B00001111);
    if (!this->write_many(0x4, 3, tmp))
    {
        return false;
    }
    return true;
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
