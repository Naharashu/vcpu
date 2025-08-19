#ifndef VCPU_H
#define VCPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ARC "x32"
#define VPROC_NAME "N100"
#define VPROC_COMP "Nowa Co."
#define CORES 1

typedef struct {
    int32_t* memory;
    uint32_t pc;
    int32_t reg[16];
} VCPU;

extern VCPU vcpu;

void init_vcpu();
int parse(char input[]);
void execute();
int compile();


#endif
