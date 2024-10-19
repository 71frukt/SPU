#ifndef SPU_H
#define SPU_H

#include "stack.h"

#define DEBUG

const size_t REGISTERS_NUM  = 5;

const int    RAM_SIZE_X = 20;
const int    RAM_SIZE_Y = 10;
const int    RAM_SIZE   = RAM_SIZE_X * RAM_SIZE_Y;

const size_t MAX_CMD_SIZE   = 500;

const size_t REGISTER_POISON = 0xEBA1;
const size_t CMD_POISON      = 0xFAFA;

const int   FUNC_CODE_BYTE_SIZE = 13;

enum StkElmsCmpVal
{
    A, AE, B, BE, E, NE
};

enum FuncCodes
{
    PUSH  =  1,
    POP   =  2,
    ADD   =  3,
    SUB   =  4,
    MUL   =  5,
    DIV   =  6,
    JUMP  =  7,
    CALL  =  8,
    RET   =  9,
    JA    =  10,
    JAE   =  11,
    JB    =  12,
    JBE   =  13,
    JE    =  14,
    JNE   =  15,
    OUT   =  0,
    HLT   =  666
};

enum ManagerBits
{
    IMM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 0),
    REG_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 1),
    MEM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 2)    //TODO RAM_BIT
};

struct cmd_t
{
    size_t ip;
    size_t size;
    int code[MAX_CMD_SIZE];
};

struct spu_t
{
    int RAM[RAM_SIZE];
    int registers[REGISTERS_NUM];
    cmd_t cmd;
    StackID data_stk;
    StackID func_stk;

    FILE *code_file;
    ON_DEBUG(FILE *logfile); 
};

void SpuCtor(spu_t *spu);
void SpuDtor(spu_t *spu);

StackElem_t GetArg(spu_t *spu);

StkElmsCmpVal StkTwoLastElmsCmp(StackID stk);

int GetMaskForFunc();

#endif