#include "lib.h"

extern mem_t memory;
extern char command_buffer[COMMAND_SIZE];
extern int fd;

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
    printf("R0: %d, R1: %d, R2: %d, R3: %d, R4: %d, R5: %d, R6: %d, WSR: %d, IP: %d\n\n", memory.regs[0], memory.regs[1], memory.regs[2], memory.regs[3], memory.regs[4], memory.regs[5], memory.regs[6], memory.wsr, memory.rip);
    printf("Memory:  [");
    for (int i = 0; i < MEM_SIZE; ++i)
    {
        printf("%d ", memory.simulated_memory[i]);
    }
    printf("\b] (size %d)\n", MEM_SIZE);
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
        if (memory.wsr == WSR_DEACTIVE) ++memory.rip;
        memory.wsr = WSR_ACTIVE;
    }
    else if (!strcmp(token, "EXIT"))
    {
        if (memory.wsr == WSR_DEACTIVE)
        {
            close(fd);
            exit(EXIT_SUCCESS);
        }
        memory.wsr = WSR_DEACTIVE;
        memset(command_buffer, 0, COMMAND_SIZE);
        memset(memory.regs, 0, sizeof(int) * REG_COUNT);
    }
    else if ((!strcmp(token, "ADD") || !strcmp(token, "SUB")) && (memory.wsr == WSR_ACTIVE))
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
            memset(command_buffer, 0, COMMAND_SIZE);
            return;
        }
        memory.rip++;
        alu_logic(memory.regs[first_reg], memory.regs[second_reg], &memory.regs[dest_reg], opcode);
    }
    else if (!strcmp(token, "LAYO"))
    {
        memory.rip++;
        layo();
    }
    else if ((!strcmp(token, "MOV")) && (memory.wsr == WSR_ACTIVE))
    {
        int first_reg, value;
        token = strtok(NULL, " ,");
        first_reg = atoi(token + 1);
        token = strtok(NULL, " ,");
        if (*token == 'R') {
            value = atoi(token + 1);
            if (value < 0 || value > 6) {
                printf("Invalid Instruction!\n");
                memset(command_buffer, 0, COMMAND_SIZE);
                return;
            }
            value = memory.regs[value];
        } else value = atoi(token);
        if (first_reg < 0 || first_reg > 6)
        {
            printf("Invalid Instruction!\n");
            memset(command_buffer, 0, COMMAND_SIZE);
            return;
        }
        mov(&memory.regs[first_reg], value);
        memory.rip++;
    }
    else if ((!strcmp(token, "LOAD") || !strcmp(token, "STORE")) && (memory.wsr == WSR_ACTIVE)) {
        int is_load;
        if (!strcmp(token, "LOAD")) is_load = 1;
        else is_load = 0;
        int reg_index, address;
        token = strtok(NULL, " ,");
        reg_index = atoi(token + 1);
        if (reg_index < 0 || reg_index > 6) {
                printf("Invalid Instruction!\n");
                memset(command_buffer, 0, COMMAND_SIZE);
                return;
        }
        token = strtok(NULL, " ,");
        address = atoi(token);
        if (address < 0 || address > MEM_SIZE) {
            printf("Invalid Instruction!\n");
            memset(command_buffer, 0, COMMAND_SIZE);
            return;
        }
        if (is_load) {
            memory.regs[reg_index] = memory.simulated_memory[address];
        } else {
            memory.simulated_memory[address] = memory.regs[reg_index];
        }
        ++memory.rip;
    } else if ((!strcmp(token, "DISC") && (memory.wsr == WSR_ACTIVE))) {
        if (memory.rip == 0) {
            return;
        }
        int count = 1;
        token = strtok(NULL, " ,");
        if (token) {
            count = atoi(token);
        }
        lseek(fd, 0, SEEK_SET);
        int file_size = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        ftruncate(fd, file_size - sizeof(mem_t) * count);
        memory.rip -= count;
        lseek(fd, -sizeof(mem_t), SEEK_END);
        read(fd, &memory, sizeof(mem_t));
        return;
    } else {
        printf("Invalid Instruction! Try running START first!\n");
    }
    memset(command_buffer, 0, COMMAND_SIZE);
    write(fd, &memory, sizeof(mem_t));
}