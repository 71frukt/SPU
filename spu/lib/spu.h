#ifndef SPU_H
#define SPU_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"
#include "../../commands_enum.h"

#define SPU_DEBUG
#include "spu_debug.h"

const char *const code_file_name = "../res_program_code.txt";


const size_t REGISTERS_NUM    = 10;

const int    RAM_SIZE_X       = 120;
const int    RAM_SIZE_Y       = RAM_SIZE_X;
const int    EXTRA_RAM_SIZE_Y = RAM_SIZE_Y * 2;
const int    PIXEL_SIZE       = 5;                         // ������ �������� RAM - ������� (PIXEL_SIZE ? PIXEL_SIZE)
const int    START_CELL_FREQ  = 3;
const int    SLEEP_TIME       = 0;                       

const int    RAM_SIZE         = RAM_SIZE_X * RAM_SIZE_Y;
const int    EXTRA_RAM_SIZE   = RAM_SIZE_X * EXTRA_RAM_SIZE_Y;
const int    WINDOW_SIZE_X    = RAM_SIZE_X * PIXEL_SIZE;
const int    WINDOW_SIZE_Y    = RAM_SIZE_Y * PIXEL_SIZE;

const size_t MAX_CMD_SIZE     = 1000;

const size_t REGISTER_POISON  = 0xEBA1;
const size_t CMD_POISON       = 0xFAFA;

const int   FUNC_CODE_BYTE_SIZE = 13;

const int RANDOM_CONST = 5;

enum StkElmsCmpVal
{
    A, B, E
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
    int spu_err;
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