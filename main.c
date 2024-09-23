#include "lib.h"

int regs[REG_COUNT];
int wsr;
int rip;
int simulated_memory[MEM_SIZE];
char command_buffer[COMMAND_SIZE];

int main()
{
    while (1)
    {
        control_unit();
    }
    return 0;
}