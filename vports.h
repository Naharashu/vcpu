#ifndef VPORTS_H
#define VPORTS_H

#include <stdio.h>
#include <string.h>

#define OUTPUT_PORT 0x01
#define INPUT_PORT 0x02
#define STATUS_PORT 0x03
#define CONTROL_PORT 0x04

#define IO_PORT 1
#define DATA_PORT 2

#define BUFFER_MINIMAL 16
#define BUFFER_SMALL 256
#define BUFFER_LARGE 1024

typedef struct port_t {
    int port_number;
    int data[BUFFER_SMALL];
    unsigned char type;
} port_t;



extern port_t ports[4];

void init_ports();
int read_port_std(int port_number, unsigned char address);
void write_port_std(int port_number, unsigned char address, int value);

#endif 