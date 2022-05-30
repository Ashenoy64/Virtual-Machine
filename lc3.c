#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>

#define MEMORY_MAX (1 << 16)
uint16_t memory[MEMORY_MAX];
enum        /*Registers*/
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC, /* program counter */
    R_COND,
    R_COUNT
};

uint16_t reg[R_COUNT];

enum        /*OpCodes*/
{
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};

enum        /*Condition Flags*/
{
    FL_POS = 1 << 0, /* P */
    FL_ZRO = 1 << 1, /* Z */
    FL_NEG = 1 << 2, /* N */
};

void handle_interrupt(int signal)
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

int main(int argc, const char* argv[])

{
    if (argc<2)
    {
        /* This line is to show usage string */
        printf("lc3 [image-file1]...\n");
        exit(2);
    }

    for(int j=1;j<argc;++j)
    {
        if(!read_image(argv[j]))
        {
            printf("Failed to load image: %s\n",argv[j]);
            exit(1);
        }
    }
    
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();
    /* since exactly one condition flag should be set at any given time, set the Z flag */
    reg[R_COND] = FL_ZRO;
    /* set the PC to starting position */
    /* 0x3000 is the default */
    enum { PC_START = 0x3000 };
    reg[R_PC] = PC_START;
}