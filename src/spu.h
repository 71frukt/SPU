#ifndef SPU_H
#define SPU_H

#include "TXLib.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"

// #define SPU_DEBUG
#include "spu_debug.h"

const size_t REGISTERS_NUM  = 10;

const int    RAM_SIZE_X      = 40;
const int    RAM_SIZE_Y      = 40;
const int    PIXEL_SIZE      = 10;                         // одна €чейка RAM - пиксель (PIXEL_SIZE х PIXEL_SIZE)
const double START_CELL_FREQ = 0.3;
const int    SLEEP_TIME      = 100;                         // в миллисекундах

const int    RAM_SIZE      = RAM_SIZE_X * RAM_SIZE_Y;
const int    WINDOW_SIZE_X = RAM_SIZE_X * PIXEL_SIZE;
const int    WINDOW_SIZE_Y = RAM_SIZE_Y * PIXEL_SIZE;

const size_t MAX_CMD_SIZE  = 1000;

const size_t REGISTER_POISON = 0xEBA1;
const size_t CMD_POISON      = 0xFAFA;

const int   FUNC_CODE_BYTE_SIZE = 13;

const int RANDOM_CONST = 5;

enum StkElmsCmpVal
{
    A, B, E
};

enum FuncCodes
{
    PUSH   = 1,
    POP    = 2,
    JUMP   = 3,
    CALL   = 4,
    RET    = 5,
    JA     = 6,
    JAE    = 7,
    JB     = 8,
    JBE    = 9,
    JE     = 10,
    JNE    = 11,
    ADD    = 12,
    SUB    = 13,
    MUL    = 14,
    DIV    = 15,
    SQRT   = 16,
    MOD    = 17,
    CRTWND = 18,
    DRAW   = 19,
    SETRNDRAM = 20,

    SPU_OUT = 0,
    HLT = 666
};

enum ManagerBits
{
    IMM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 0),
    REG_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 1),
    RAM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 2)
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
    StackID data_stk;
    StackID func_stk;

    FILE *code_file;
    ON_SPU_DEBUG(FILE *logfile); 
};

void SpuCtor(spu_t *spu);
void SpuDtor(spu_t *spu);

void DrawInConsole();
void DrawInWindow ();
void SpuCreateWindow();

StackElem_t *GetArg(spu_t *spu);

StkElmsCmpVal StkTwoLastElmsCmp(StackID stk);

int GetMaskForFunc();
void SetRandomRam();

void SpuAssert   (spu_t *compiler, const char *file, int line, const char *func);
int  SpuVerify   (spu_t *spu);
void SpuDump     (spu_t *spu, const char *file, int line, const char *func);
void PrintSpuErr (int error);

#endif