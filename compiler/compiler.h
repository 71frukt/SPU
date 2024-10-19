#ifndef COMPILER_H
#define COMPILER_H

#define DEBUG

#include <stdio.h>
#include "compiler_debug.h"

#define MARK_SYMBOL ":"

const int   COMMAND_NAME_LEN = 20;
const int   MARK_NAME_LEN    = 10;
const int   MAX_ARG_NAME_LEN = 10;
const int   REG_NAME_LEN     = 2;

const int   REGISTERS_NUM    = 5;

const int   CMD_POISON       = -0xEBE;
const int   MARK_POISON      = -0xDEB41C;
const int   REGISTER_POISON  = -0xACCCCC;

const int   FUNC_CODE_BYTE_SIZE = 13;

struct command_t
{
    char name[COMMAND_NAME_LEN];
    int key;
};

struct trans_commands_t
{
    command_t *commands;
    size_t    size;
};

struct cmd_t
{
    size_t    ip;
    size_t    size;
    int *code;
};

struct mark_t
{
    size_t address;
    char   name[MARK_NAME_LEN];
};

struct marklist_t
{
    size_t  ip;
    size_t  size;
    mark_t *list;
};

struct fixup_el_t
{
    size_t num_in_marklist;
    size_t mark_ip;
};

struct fixup_t
{
    size_t ip;
    size_t size;
    fixup_el_t *data;
};

struct compiler_t
{
    FILE *logfile;

    FILE *asm_file;    
    FILE *code_file;

    trans_commands_t trans_commands;
    cmd_t cmd;
    fixup_t fixup;
    marklist_t marklist;
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
    MEM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 2)
};

void    CompilerCtor     (compiler_t *compiler);
void    CompilerDtor     (compiler_t *compiler);

size_t  GetCountOfLines  (FILE *text);
size_t  GetCountOfWords  (FILE *text);

void    MakeFixUp        (compiler_t *compiler);
void    GetCommands      (const char *file_name, trans_commands_t *trans_commands);
void    WriteCommandCode (char *cur_command_name, compiler_t *compiler);

int     ReadRegister     (char *reg_name);
bool    IsRegister       (char *reg_name);

mark_t *FindMarkInList   (char *mark_name, marklist_t *list);
bool    IsMark           (char *str);

void    PrintCMD         (compiler_t *compiler);

void CompilerAssert   (compiler_t *compiler, int *cmp_err, const char *file, int line, const char *func);
int  CompilerVerify   (compiler_t *compiler);
void CompilerDump     (compiler_t *compiler, const char *file, int line, const char *func);
void PrintCompilerErr (int error);

#endif