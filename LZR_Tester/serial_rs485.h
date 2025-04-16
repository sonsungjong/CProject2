#ifndef SERIAL_RS485_H_
#define SERIAL_RS485_H_

#ifdef __cplusplus
extern "C" {
#endif


int openSerialPort(char* portName, int baudRate);
void closeSerialPort(void);


#ifdef __cplusplus
}
#endif

#endif