#ifndef LIB
#define LIB

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>

#define MEM_SIZE 256
#define OP_ADD 1
#define OP_SUB 2
#define COMMAND_SIZE 128
#define WSR_ACTIVE 1
#define WSR_DEACTIVE 0
#define REG_COUNT 7

void alu_logic(int first_reg, int second_reg, int *dest_reg, int opcode);
void control_unit();
void layo();
void mov(int * reg, int val);

#endif