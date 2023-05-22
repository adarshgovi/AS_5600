//
//    FILE: AS5600.h
//  AUTHOR: Adarsh Govindan
// VERSION: 0.1.1
// PURPOSE: Raspberry PI library for AS5600 magnetic rotation meter
//    DATE: 2023-05-22


#ifndef AS_5600
#define AS_5600
extern "C" {
    #include <linux/i2c-dev.h>
    #include <i2c/smbus.h>
}
#include <stdint.h>
#include <bitset>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

using namespace std;

class AS_5600{
    public:
    AS_5600(void);
    bool detectMagnet();
    int getRawAngle();
    double getAngle();
    int getZPosition();
    int readTwoBytes(int addr_in);
    void writeTwoBytes(int addr_out, int bytes);
    void writeStartAngle();

    private:
    int file;
    static const uint8_t addressAngle=0x0E;
    static const uint8_t rawAngleAddress=0x0C;
    static const uint8_t addressMagnetStatus=0x0B;
    static const uint8_t addressZpos=0x01;
};

#endif