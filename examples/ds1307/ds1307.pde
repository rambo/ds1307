// Get this from http://dsscircuits.com/articles/arduino-i2c-master-library.html
#include <I2C.h> // For some weird reason including this in the relevant .h file does not work
#include <i2c_device.h> // For some weird reason including this in the relevant .h file does not work
#include <ds1307.h>

void setup()
{
    DS1307.begin(true);
}

void loop()
{
    DS1307.read_clock();
    
    //Serial.println(DS1307.iso_ts());
    Serial.print(DS1307.year(), DEC);
    Serial.print("-");
    Serial.print(DS1307.month(), DEC);
    Serial.print("-");
    Serial.print(DS1307.day(), DEC);
    Serial.print(" ");
    Serial.print(DS1307.hour(), DEC);
    Serial.print(":");
    Serial.print(DS1307.minute(), DEC);
    Serial.print(":");
    Serial.println(DS1307.second(), DEC);
    
    DS1307.dump_registers(0x0, 0x7);
    delay(500);
}
