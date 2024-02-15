#include <windows.h>
#include <iostream>

int main() {
    // Open serial port
    HANDLE serialPort = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (serialPort == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }

    // Set parameters
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(serialPort, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state." << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600; // Example baud rate
    dcbSerialParams.ByteSize = 8; // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity = NOPARITY; // No parity

    if (!SetCommState(serialPort, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state." << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    // Write to serial port
    char data[] = "Hello, serial port!";
    DWORD bytesWritten;
    if (!WriteFile(serialPort, data, sizeof(data), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port." << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    // Read from serial port
    char buffer[256];
    DWORD bytesRead;
    if (!ReadFile(serialPort, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port." << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    std::cout << "Received data: " << buffer << std::endl;

    // Close serial port
    CloseHandle(serialPort);

    return 0;
}
