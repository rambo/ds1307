#include "ds1307.h"


void ds1307::begin(boolean wire_begin)
{
    i2c_device::begin(DS1307_ADDRESS, wire_begin);
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
    tmp[0] = ((second / 10) << 4) | (second % 10);
    // Minutes register
    tmp[1] = ((minute / 10) << 4) | (minute % 10);
    // Hours register (and set 24h mode)
    tmp[2] = B010000 | ((hour / 10) << 4) | (hour % 10);
    // Write the above values
    if (!this->write_many(0x0, 3, tmp))
    {
        return false;
    }
    // address 0x3 is skipped
    // day
    tmp[0] = ((day / 10) << 4) | (day % 10);
    // month
    tmp[1] = ((month / 10) << 4) | (month % 10);
    // year
    tmp[2] = ((years / 10) << 4) | (years % 10);
    if (!this->write_many(0x4, 3, tmp))
    {
        return false;
    }
    return true;
}

inline byte parse_bcd(byte reg_value, byte high_mask)
{
    return ((reg_value & high_mask) >> 4) * 10 + (reg_value & B00001111);
}

byte ds1307::second()
{
    //return ((rtc_regs[0] & B01110000) >> 4) * 10 + (rtc_regs[0] & B00001111);
    return parse_bcd(rtc_regs[0], B01110000);
}
byte ds1307::minute()
{
    //return ((rtc_regs[1] & B01110000) >> 4) * 10 + (rtc_regs[1] & B00001111);
    return parse_bcd(rtc_regs[1], B01110000);
}
byte ds1307::hour()
{
    //return ((rtc_regs[2] & B00110000) >> 4) * 10 + (rtc_regs[2] & B00001111);
    return parse_bcd(rtc_regs[2], B00110000);
}
byte ds1307::dow()
{
    //return (rtc_regs[3] & B00000111);
    return parse_bcd(rtc_regs[3], B00000000);
}
byte ds1307::day()
{
    //return ((rtc_regs[4] & B00110000) >> 4) * 10 + (rtc_regs[4] & B00001111);
    return parse_bcd(rtc_regs[4], B00110000);
}
byte ds1307::month()
{
    //return ((rtc_regs[5] & B00010000) >> 4) * 10 + (rtc_regs[5] & B00001111);
    return parse_bcd(rtc_regs[5], B00010000);
}
byte ds1307::years()
{
    //return ((rtc_regs[6] & B11110000) >> 4) * 10 + (rtc_regs[6] & B00001111);
    return parse_bcd(rtc_regs[6], B11110000);
}

int ds1307::year()
{
    return DS1307_CENTURY + years();
}
#ifndef DS1307_CONSERVE_SPACE
char* ds1307::iso_ts()
{
    sprintf(isobuffer,"%04u-%02u-%02u %02u:%02u:%02u",
        /**
         * Inlined versions of the register parsing functions
         */
        (int)(DS1307_CENTURY + parse_bcd(rtc_regs[6], B11110000)),
        (int)parse_bcd(rtc_regs[5], B00010000),
        (int)parse_bcd(rtc_regs[4], B00110000),
        (int)parse_bcd(rtc_regs[2], B00110000),
        (int)parse_bcd(rtc_regs[1], B01110000),
        (int)parse_bcd(rtc_regs[0], B01110000)
    );

    return isobuffer;
}
#endif

// Instance 
ds1307 DS1307 = ds1307();
