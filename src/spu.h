#ifndef SPU_H
#define SPU_H

#include "stack.h"

#define SPU_DEBUG
#include "spu_debug.h"

const size_t REGISTERS_NUM  = 4;

const int    RAM_SIZE_X = 100;
const int    RAM_SIZE_Y = 100;
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
    JUMP  =  3,
    CALL  =  4,
    RET   =  5,
    JA    =  6,
    JAE   =  7,
    JB    =  8,
    JBE   =  9,
    JE    =  10,
    JNE   =  11,
    DRAW  =  12,
    ADD   =  13,
    SUB   =  14,
    MUL   =  15,
    DIV   =  16,
    SQRT  =  17,
    MOD   =  18,
    
    OUT   =  0,
    HLT   =  666
};

enum ManagerBits
{
    IMM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 0),
    REG_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 1),
    RAM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 2)    //TODO RAM_BIT
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
    ON_SPU_DEBUG(FILE *logfile); 
};

void SpuCtor(spu_t *spu);
void SpuDtor(spu_t *spu);

void Draw(spu_t *spu);

StackElem_t *GetArg(spu_t *spu);

StkElmsCmpVal StkTwoLastElmsCmp(StackID stk);

int GetMaskForFunc();

void SpuAssert   (spu_t *compiler, const char *file, int line, const char *func);
int  SpuVerify   (spu_t *spu);
void SpuDump     (spu_t *spu, const char *file, int line, const char *func);
void PrintSpuErr (int error);

#endif