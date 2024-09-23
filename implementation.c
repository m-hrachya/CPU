#include "lib.h"

extern int regs[REG_COUNT];
extern int wsr;
extern int rip;
extern int simulated_memory[MEM_SIZE];
extern char command_buffer[COMMAND_SIZE];

void alu_logic(int first_reg, int second_reg, int *dest_reg, int opcode)
{
    assert(dest_reg != NULL);
    if (opcode == OP_SUB)
        second_reg *= -1;
    *dest_reg = first_reg + second_reg;
    return;
}

void layo()
{
    printf("CPU State:\n\n");
    printf("R0: %d, R1: %d, R2: %d, R3: %d, R4: %d, R5: %d, R6: %d, WSR: %d, IP: %d\n\n", regs[0], regs[1], regs[2], regs[3], regs[4], regs[5], regs[6], wsr, rip);
    // printf("Memory:  [");
    // for (int i = 0; i < MEM_SIZE; ++i)
    // {
    //     printf("%d ", simulated_memory[i]);
    // }
    // printf("\b] (size 256)\n");
}

void mov(int *reg, int value)
{
    *reg = value;
}

void control_unit()
{
    fgets(command_buffer, COMMAND_SIZE - 2, stdin);
    command_buffer[strlen(command_buffer) - 1] = '\0';
    char *token = strtok(command_buffer, " ,");
    if (!strcmp(token, "START"))
    {
        wsr = WSR_ACTIVE;
    }
    else if (!strcmp(token, "EXIT"))
    {
        if (wsr == WSR_DEACTIVE)
        {
            exit(EXIT_SUCCESS);
        }
        wsr = WSR_DEACTIVE;
        memset(command_buffer, 0, COMMAND_SIZE);
        memset(regs, 0, sizeof(int) * REG_COUNT);
        rip = 0;
    }
    else if ((!strcmp(token, "ADD") || !strcmp(token, "SUB")) && (wsr == WSR_ACTIVE))
    {
        int opcode;
        if (!strcmp(token, "ADD"))
            opcode = OP_ADD;
        else
            opcode = OP_SUB;
        int first_reg, second_reg, dest_reg;
        token = strtok(NULL, " ,");
        dest_reg = atoi(token + 1);
        token = strtok(NULL, " ,");
        first_reg = atoi(token + 1);
        token = strtok(NULL, " ,");
        second_reg = atoi(token + 1);
        if (first_reg < 0 || second_reg < 0 || dest_reg < 0 || first_reg > 6 || second_reg > 6 || dest_reg > 6)
        {
            printf("Invalid instruction!\n");
            return;
        }
        rip++;
        alu_logic(regs[first_reg], regs[second_reg], &regs[dest_reg], opcode);
    }
    else if (!strcmp(token, "LAYO"))
    {
        rip++;
        layo();
    }
    else if ((!strcmp(token, "MOV")) && (wsr == WSR_ACTIVE))
    {
        int first_reg, value;
        token = strtok(NULL, " ,");
        first_reg = atoi(token + 1);
        token = strtok(NULL, " ,");
        value = atoi(token);
        if (first_reg < 0 || first_reg > 6)
        {
            printf("Invalid Instruction!\n");
            return;
        }
        mov(&regs[first_reg], value);
        rip++;
    }
    memset(command_buffer, 0, COMMAND_SIZE);
}