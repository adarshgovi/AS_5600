#include "AS_5600.h"
#include <bitset>


AS_5600::AS_5600(){
    cout<<"start read"<<endl;
    int adapter_nr = 1;
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    this->file = open(filename, O_RDWR);
    if (file < 0) {
        cout << "error using i2c, check that i2c communication is enabled"<<endl;
        exit(1);
    }
    int addr = 0x36;
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        cout << "i2c read error" <<endl;
        exit(1);
    }

}

int AS_5600::getRawAngle(){
    return readTwoBytes(addressRawAngle);
}

double AS_5600::getAngle(){
    // scale angle value to convert to degrees
    return ((double) readTwoBytes(addressAngle))*(360.0)/4096.0;
}

int AS_5600::getZPosition(){
    return readTwoBytes(addressZpos);
}

void AS_5600::writeStartAngle(){
    return writeTwoBytes(addressZpos, getRawAngle());
}

bool AS_5600::detectMagnet(){
    uint8_t value;
    value = i2c_smbus_read_word_data(file, _addr_magnet_status_);
    if (value < 0) {
        cout<<"err" <<endl;
    }
    return (value & 0x20); 
}

void AS_5600::writeTwoBytes(int addr_out, int bytes){
    uint8_t highByte=(bytes>>8) & 0xFF;
    uint8_t lowByte = bytes & 0xFF;
    i2c_smbus_write_byte_data(file, (addr_out), highByte);
    i2c_smbus_write_byte_data(file, (addr_out+1), lowByte);
}

int AS_5600::readTwoBytes(int addr_in){
    __u8 reg = addr_in;
    uint16_t highByte;
    uint8_t lowByte;
    char buf[10];
    highByte = i2c_smbus_read_word_data(file, reg);
    lowByte = i2c_smbus_read_word_data(file, reg+1);
    if (highByte < 0 || lowByte < 0) {
        cout<<"err" <<endl;
    /* ERROR HANDLING: i2c transaction failed */
    } else {
        std::bitset<16> binary(highByte);
        std::bitset<8> binary1(lowByte);
        // cout<<"high byte: "<< binary << endl;
        // cout<<"low byte: "<< binary1 << endl;
        highByte=highByte<<8;
    }
     return ((int)(highByte|lowByte));
}