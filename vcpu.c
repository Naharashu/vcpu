#include "vcpu.h"
#include "ram.h"
#include "vports.h"

VCPU vcpu;

uint8_t ZF = 0;
uint8_t IDF = 0;

void init_vcpu() {
	vcpu.memory = calloc(16384, sizeof(int32_t));
	vcpu.pc = 0;
	memset(vcpu.reg, 0, sizeof(vcpu.reg));
}

int parse(char input[]) {
    if (input == NULL) {
		vcpu.memory[0] = 0x00;
		vcpu.memory[1] = 0xFF;
		return 1;
    }

    char input_copy[512];
    strncpy(input_copy, input, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';

    char *toks = strtok(input_copy, " ");
	int code = 0;
	int i = 0;
    while (toks != NULL) {
    	code = strtol(toks, NULL, 16);
		vcpu.memory[i] = code;
		i++;
        toks = strtok(NULL, " ");
    }
	
    return i;
}

void execute() {
	register int i = 0;
	#define FETCH (vcpu.memory[vcpu.pc++])
	for(;;) {
		i = FETCH;
		#ifdef SAFE_MODE
			if(read_port_std(STATUS_PORT, 0) == 0x02) {
				fprintf(stderr, "Execution halted due to port status error.\n");
				return;
			}
			if(vcpu.pc >= 16384) {
				fprintf(stderr, "Program counter out of bounds: %d\n", vcpu.pc);
				return;
			}
		#endif
		switch(i) {
			case 0x00: {
				goto next;
			}
			case 0x01: {
				uint8_t r = FETCH;
				int32_t value = FETCH;
				vcpu.reg[r] = value;
				goto next;
			}
			case 0x02: {
				uint8_t r1 = FETCH;
				uint8_t r2 = FETCH;
				vcpu.reg[r1] = vcpu.reg[r2];
				goto next;
			}
			case 0x03: {
				uint8_t r = FETCH;
				write_ram(FETCH, vcpu.reg[r]);
				goto next;
			}
			case 0x04: {
				uint8_t r = FETCH;
				vcpu.reg[r] = read_ram(FETCH);
				goto next;
			}
			case 0x05: {
				uint8_t r1 = FETCH;
				int32_t val = FETCH;
				vcpu.reg[r1] += val;
				goto next;
			}
			case 0x06: {
				uint8_t r1 = FETCH;
				int32_t val = FETCH;
				vcpu.reg[r1] -= val;
				goto next;
			}
			case 0x07: {
				uint8_t r1 = FETCH;
				int32_t val = FETCH;
				vcpu.reg[r1] *= val;
				goto next;
			}
			case 0x08: {
				uint8_t r1 = FETCH;
				int32_t val = FETCH;
				vcpu.reg[r1] /= val;
				goto next;
			}
			case 0x09: {
				uint8_t r1 = FETCH;
				uint8_t r2 = FETCH;
				vcpu.reg[r1] += vcpu.reg[r2];
				goto next;
			}
			case 0x0A: {
				uint8_t r1 = FETCH;
				uint8_t r2 = FETCH;
				vcpu.reg[r1] -= vcpu.reg[r2];
				goto next;
			}
			case 0x0B: {
				uint8_t r1 = FETCH;
				uint8_t r2 = FETCH;
				vcpu.reg[r1] *= vcpu.reg[r2];
				goto next;
			}
			case 0x0C: {
				uint8_t r1 = FETCH;
				uint8_t r2 = FETCH;
				vcpu.reg[r1] /= vcpu.reg[r2];
				goto next;
			}
			case 0x0D: {
				uint8_t r1 = FETCH;
				uint8_t addr = FETCH;
				write_port_std(OUTPUT_PORT, addr, vcpu.reg[r1]);
				goto next;
			}
			case 0x0E: {
				uint8_t r1 = FETCH;
				uint8_t addr = FETCH;
				vcpu.reg[r1] = read_port_std(INPUT_PORT, addr);
				goto next;
			}
			case 0x0F: {
				uint8_t addr = FETCH;
				printf("%d", read_port_std(OUTPUT_PORT, addr));
				goto next;
			}
			case 0x10: {
				uint8_t port = FETCH;
				uint8_t addr = FETCH;
				int value = FETCH;
				write_port_std(port, addr, value);
				goto next;
			}
			case 0x11: {
				uint8_t port = FETCH;
				uint8_t addr = FETCH;
				int r = FETCH;
				vcpu.reg[r] = read_port_std(port, addr);
				goto next;
			}
			case 0x12: {
				uint8_t r = FETCH;
				uint8_t r2 = FETCH;
				ZF = (vcpu.reg[r] == 0);
				IDF = (vcpu.reg[r] == vcpu.reg[r2]);
				goto next;
			}
			case 0x13: {
				uint32_t addr = FETCH;
				#ifdef SAFE_MODE
					if(addr >= 16384) {
						fprintf(stderr, "Address out of bounds: %d\n", addr);
						return;
					}
				#endif
				vcpu.pc = addr;
				goto next;
			}
			case 0x14: {
				uint32_t addr = FETCH;
				#ifdef SAFE_MODE
					if(addr >= 16384) {
						fprintf(stderr, "Address out of bounds: %d\n", addr);
						return;
					}
				#endif
				if(ZF) vcpu.pc = addr;
				goto next;
			}
			case 0x15: {
				uint32_t addr = FETCH;
				#ifdef SAFE_MODE
					if(addr >= 16384) {
						fprintf(stderr, "Address out of bounds: %d\n", addr);
						return;
					}
				#endif
				if(!ZF) vcpu.pc = addr;
				goto next;
			}
			case 0x16: {
				uint32_t addr = FETCH;
				#ifdef SAFE_MODE
					if(addr >= 16384) {
						fprintf(stderr, "Address out of bounds: %d\n", addr);
						return;
					}
				#endif
				if(IDF) vcpu.pc = addr;
				goto next;
			}
			case 0x17: {
				uint32_t addr = FETCH;
				#ifdef SAFE_MODE
					if(addr >= 16384) {
						fprintf(stderr, "Address out of bounds: %d\n", addr);
						return;
					}
				#endif
				if(!IDF) vcpu.pc = addr;
				goto next;
			}
			case 0xFF:
				return;
			default:
				printf("Unknown instruction: 0x%X\n", i);
				return;
		}
		next:
			continue;
	}
	#undef FETCH
}

int compile() {
	FILE *fp = fopen("output.vbin", "wb");
	if (fp == NULL) {
		perror("Failed to open output file");
		return -1;
	};
	fwrite(vcpu.memory, sizeof(int32_t), vcpu.pc, fp);
	fclose(fp);
	return 0;
}


int main(int argc, char *argv[]) {
	init_ram();
	init_ports();
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-sm") == 0) {
			#define SAFE_MODE
			printf("Safe mode enabled.\n");
		}
	}
	if(strcmp(argv[1], "run") == 0) {
		init_vcpu();
		parse(argv[2]);
		execute();
		free(vcpu.memory);
		free_ram();
		return 0;
	}
	if(strcmp(argv[1], "compile") == 0) {
		init_vcpu();
		vcpu.pc = parse(argv[2]);
		compile();
		free(vcpu.memory);
		free_ram();
		return 0;
	}
	if(strcmp(argv[1], "execute") == 0) {
		init_vcpu();
		FILE *f = fopen(argv[2], "rb");
		if (f == NULL) {
			perror("Failed to open input file");
			return -1;
		}
		fread(vcpu.memory, sizeof(int32_t), 16384, f);
		vcpu.pc = 0;
		execute();
		fclose(f);
		free(vcpu.memory);
		free_ram();
		return 0;
	}
	init_vcpu();
	execute();
	free(vcpu.memory);
	free_ram();
	return 0;
}
