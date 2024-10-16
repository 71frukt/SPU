#ifndef SPU_H
#define SPU_H

const size_t MAX_CMD_SIZE   = 500;
const int    REGISTERS_NUM  = 5;

enum FuncCodes
{
    PUSH  = 1,
    ADD   = 2,
    SUB   = 3,
    MUL   = 4,
    DIV   = 5,
    PUSHR = 6,
    POPR  = 7,
    OUT   = 0,
    HLT   = -1
};

typedef struct Cmd
{
    size_t pi;
    size_t size;
    int code[MAX_CMD_SIZE];
} cmd_t;


#endif