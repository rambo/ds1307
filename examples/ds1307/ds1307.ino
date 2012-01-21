/**
 * Example sketch to show the current time from the RTC and register states
 *
 * Can also be used to set the (24h) clock by sending YYYY-MM-DD HH:MM:SS (use newline as line ending) in serial monitor
 *
 * The iso_ts() method used sprintf that will use quite a bit of program memory, if this is a problem define DS1307_CONSERVE_SPACE 
 * before including the library to disable that method
 *
 */
// https://github.com/rambo/I2C
#include <I2C.h> // For some weird reason including this in the relevant .h file does not work
#define I2C_DEVICE_DEBUG
// Get this from https://github.com/rambo/i2c_device
#include <i2c_device.h> // For some weird reason including this in the relevant .h file does not work
#include <ds1307.h>
#define COMMAND_STRING_SIZE 20

char incoming_command[COMMAND_STRING_SIZE+2]; // Allocate for CRLF too
byte incoming_position;

void setup()
{
    DS1307.begin(true);
}

inline void process_command()
{
    char tmp[3]; // Temp buffer for atoi
    tmp[0] = incoming_command[2]; // Ignore the century part of 4-digit year
    tmp[1] = incoming_command[3];
    byte year = atoi(tmp);

    tmp[0] = incoming_command[5]; 
    tmp[1] = incoming_command[6];
    byte month = atoi(tmp);
    
    tmp[0] = incoming_command[8]; 
    tmp[1] = incoming_command[9];
    byte day = atoi(tmp);

    tmp[0] = incoming_command[11]; 
    tmp[1] = incoming_command[12];
    byte hour = atoi(tmp);

    tmp[0] = incoming_command[14]; 
    tmp[1] = incoming_command[15];
    byte minute = atoi(tmp);

    tmp[0] = incoming_command[17]; 
    tmp[1] = incoming_command[18];
    byte second = atoi(tmp);

    Serial.print("Parsed command '");
    Serial.print(incoming_command);
    Serial.print("' to: ");
    Serial.print(year, DEC);
    Serial.print("-");
    Serial.print(month, DEC);
    Serial.print("-");
    Serial.print(day, DEC);
    Serial.print(" ");
    Serial.print(hour, DEC);
    Serial.print(":");
    Serial.print(minute, DEC);
    Serial.print(":");
    Serial.println(second, DEC);
    
    DS1307.set_clock(year, month, day, hour, minute, second);
    
    
}

inline void read_command()
{
    for (byte d = Serial.available(); d > 0; d--)
    {
        incoming_command[incoming_position] = Serial.read();
        // Check for line end and in such case do special things
        if (   incoming_command[incoming_position] == 0xA // LF
            || incoming_command[incoming_position] == 0xD) // CR
        {
            incoming_command[incoming_position] = 0x0;
            if (   incoming_position > 0
                && (   incoming_command[incoming_position-1] == 0xD // CR
                    || incoming_command[incoming_position-1] == 0xA) // LF
               )
            {
                incoming_command[incoming_position-1] = 0x0;
            }
            process_command();
            // Clear the buffer and reset position to 0
            memset(&incoming_command, 0, COMMAND_STRING_SIZE+2);
            incoming_position = 0;
            return;
        }
        incoming_position++;

        // Sanity check buffer sizes
        if (incoming_position > COMMAND_STRING_SIZE+2)
        {
            Serial.print("PANIC: No end-of-line seen and incoming_position=");
            Serial.print(incoming_position, DEC);
            Serial.println(" clearing buffers");
            
            memset(&incoming_command, 0, COMMAND_STRING_SIZE+2);
            incoming_position = 0;
            Serial.flush();
            break;
        }
    }
}

unsigned long last_report_time;
void loop()
{
    read_command();
    if ((millis() - last_report_time) > 1000)
    {
        DS1307.read_clock();
        
#ifndef DS1307_CONSERVE_SPACE
        Serial.println(DS1307.iso_ts());
#else
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
#endif
        
        DS1307.dump_registers(0x0, 0x7);

        last_report_time = millis();
    }
}
