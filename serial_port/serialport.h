#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>

class SerialPort
{
public:
    static SerialPort* getComPort(const char* comName);

    bool open();
    std::string getComName();
    std::string getDescription();
    void setBaudRate(uint8_t baudrate);
    void setByteSize(uint8_t byteSize);
    void setStopBits(uint8_t stopBits);
    void setParity(uint8_t parity);
    void closePort();
    bool write(char* data);
    char* read();
private:
    SerialPort();
    std::string m_comName;
    std::string m_descriptionName;
    uint8_t m_baudrate,m_byteSize,m_stopBits,m_parity;
};

#endif // SERIALPORT_H
