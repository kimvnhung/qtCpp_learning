#include "serialport.h"

#include <codecvt>
#include <windows.h>
#include <iostream>

SerialPort** SerialPort::instances = NULL;
uint8_t SerialPort::instances_size = 0;

SerialPort* SerialPort::getComPort(const char* comName)
{
    refreshPort();
    for (int i = 0; i < instances_size; i++) {
        if(instances != NULL && *(instances+i) != NULL){
            SerialPort *result = *(instances+i);
            if(result->getComName().compare(comName)){
                return result;
            }
        }
    }
    return NULL;
}

SerialPort** SerialPort::getComPorts()
{
    refreshPort();
    return instances;
}

uint8_t SerialPort::getComPortsSize()
{
    refreshPort();
    return instances_size;
}

bool SerialPort::open()
{
    return false;
}

std::string SerialPort::getComName()
{
    return m_comName;
}

std::string SerialPort::getDescription()
{
    return m_descriptionName;
}

void SerialPort::setBaudRate(int baudrate)
{
    m_baudrate = baudrate;
}

void SerialPort::setByteSize(uint8_t byteSize)
{
    m_byteSize = byteSize;
}

void SerialPort::setStopBits(uint8_t stopBits)
{
    m_stopBits = stopBits;
}

void SerialPort::setParity(uint8_t parity)
{
    m_parity = parity;
}

void SerialPort::closePort()
{

}

bool SerialPort::write(char* data, int data_size)
{
    return false;
}

char* SerialPort::read()
{
    return NULL;
}

//private

const wchar_t* toWString(std::string str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideStr = converter.from_bytes(str);

    return wideStr.c_str();
}

void SerialPort::refreshPort()
{
    for (int i = 1; i <= 256; ++i) {
        std::string comPortName = "COM" + std::to_string(i);
        HANDLE port = CreateFile(comPortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (port != INVALID_HANDLE_VALUE) {
            COMMTIMEOUTS timeouts = { 0 };
            timeouts.ReadIntervalTimeout = MAXDWORD;
            timeouts.ReadTotalTimeoutConstant = 50;
            timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
            timeouts.WriteTotalTimeoutConstant = 50;
            timeouts.WriteTotalTimeoutMultiplier = MAXDWORD;
            if (SetCommTimeouts(port, &timeouts)) {
                WCHAR portName[256];
                if (QueryDosDeviceW(toWString(comPortName), portName, sizeof(portName) / sizeof(WCHAR))) {
                    std::cout << comPortName << " Description: " << portName << std::endl;
                    instances_size++;

                } else {
                    std::cerr << "Error querying device description." << std::endl;
                }
            } else {
                std::cerr << "Error setting timeouts." << std::endl;
            }
            CloseHandle(port);
        }
    }
}

SerialPort::SerialPort(const char* comName, const char* descriptionName) :
    m_comName(comName),
    m_descriptionName(descriptionName),
    m_baudrate(115200),
    m_byteSize(7),
    m_stopBits(0),
    m_parity(0)
{

}
