#ifndef RAM_H
#define RAM_H

#include <stdlib.h>
#include <stdio.h>

extern int* ram;

void init_ram();
void free_ram();
int read_ram(int address);
void write_ram(int address, int value);

#endif