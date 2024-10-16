#ifndef SPU_H
#define SPU_H

const size_t MAX_CMD_SIZE   = 500;
const int    REGISTERS_NUM  = 5;

enum FuncCodes
{
    PUSH  =  1,
    ADD   =  2,
    SUB   =  3,
    MUL   =  4,
    DIV   =  5,
    PUSHR =  6,
    POPR  =  7,
    JUMP  =  8,
    OUT   =  0,
    HLT   =  -1
};

struct cmd_t
{
    size_t ip;
    size_t size;
    int code[MAX_CMD_SIZE];
};


#endif