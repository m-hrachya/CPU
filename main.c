#include "lib.h"

mem_t memory;
char command_buffer[COMMAND_SIZE];
int fd;

int main()
{
    fd = open("base.txt", O_RDWR | O_TRUNC);
    assert(fd != -1);

    while (1)
    {
        control_unit();
    }
    return 0;
}