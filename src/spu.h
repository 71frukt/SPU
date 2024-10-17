#ifndef SPU_H
#define SPU_H

#include "stack.h"

#define DEBUG

const size_t MAX_CMD_SIZE   = 500;
const size_t REGISTERS_NUM  = 5;

const size_t REGISTER_POISON = 0xEBA1F04C0;
const size_t CMD_POISON      = 0xFAFAFAFAF;

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

struct spu_t
{
    int registers[REGISTERS_NUM];
    cmd_t cmd;

    FILE *code_file;
    ON_DEBUG(FILE *logfile); 
};

void SpuCtor(spu_t *spu);
void SpuDtor(spu_t *spu);

#endif