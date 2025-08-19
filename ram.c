#include "ram.h"

int* ram;

void init_ram() {
	ram = calloc(65536, sizeof(int));
    if (ram == NULL) {
        fprintf(stderr, "Failed to allocate RAM.\n");
        exit(1);
    }
}

void free_ram() {
    if (ram != NULL) {
        free(ram);
    }
}

int read_ram(int address) {
    int value = ram[address];
    return value;
}

void write_ram(int address, int value) {
    ram[address] = value;
}