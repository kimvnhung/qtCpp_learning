#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>

class SerialPort
{
public:
    static SerialPort* getComPort(const char* comName);
    static SerialPort** getComPorts();
    static uint8_t getComPortsSize();

    bool open();
    std::string getComName();
    std::string getDescription();
    void setBaudRate(int baudrate);
    void setByteSize(uint8_t byteSize);
    void setStopBits(uint8_t stopBits);
    void setParity(uint8_t parity);
    void closePort();
    bool write(char* data,int data_size);
    char* read();
private:
    static SerialPort **instances;
    static uint8_t instances_size;
    static void refreshPort();
    SerialPort(const char* comName,const char* descriptionName);
    std::string m_comName;
    std::string m_descriptionName;
    int m_baudrate;
    uint8_t m_byteSize,m_stopBits,m_parity;
};

#endif // SERIALPORT_H
