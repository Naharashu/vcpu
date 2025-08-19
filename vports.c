#include "vports.h"

port_t ports[4];

void init_ports() {
    ports[0].port_number = INPUT_PORT;
    ports[1].port_number = OUTPUT_PORT;
    ports[2].port_number = STATUS_PORT;
    ports[3].port_number = CONTROL_PORT;

    ports[0].type = IO_PORT;
    ports[1].type = IO_PORT;
    ports[2].type = DATA_PORT;
    ports[3].type = DATA_PORT;

    for (int i = 0; i < 4; i++) {
        memset(ports[i].data, 0, sizeof(ports[i].data));
    }

    /*
    ports[STATUS_PORT].data[0] = 0x00 - WAITING
    ports[STATUS_PORT].data[0] = 0x01 - OK
    ports[STATUS_PORT].data[0] = 0x02 - Error
    ports[STATUS_PORT].data[0] = 0x03 - Warning
    ports[STATUS_PORT].data[0] = 0x04 - NOT READY
    */

    ports[STATUS_PORT].data[0] = 0x00;
}

int read_port_std(int port_number, unsigned char address) {
    if(port_number < 0 || port_number >= 4) {
        fprintf(stderr, "Invalid port number: %d\n", port_number);
        ports[STATUS_PORT].data[0] = 0x02;
        return 1;
    }
    if(address >= BUFFER_SMALL) {
        fprintf(stderr, "Invalid address: %d\n", address);
        ports[STATUS_PORT].data[0] = 0x02;
        return 1;
    }
    return ports[port_number].data[address];
}

void write_port_std(int port_number, unsigned char address, int value) {
    if(port_number < 0 || port_number >= 4) {
        fprintf(stderr, "Invalid port number: %d\n", port_number);
        ports[STATUS_PORT].data[0] = 0x02;
        return;
    }
    if(address >= BUFFER_SMALL) {
        fprintf(stderr, "Invalid address: %d\n", address);
        ports[STATUS_PORT].data[0] = 0x02;
        return;
    }
    ports[port_number].data[address] = value;
}

