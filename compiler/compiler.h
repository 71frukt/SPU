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

const int   FUNC_CODE_BYTE_SIZE = 12;

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
    ADD   =  2,
    SUB   =  3,
    MUL   =  4,
    DIV   =  5,
    PUSHR =  6,
    POP   =  7,
    JUMP  =  8,
    CALL  =  9,
    RET   =  10,
    JA    =  11,
    JAE   =  12,
    JB    =  13,
    JBE   =  14,
    JE    =  15,
    JNE   =  16,
    OUT   =  0,
    HLT   =  -1
};

enum ManagerBits
{
    IMM_BIT = 1 << FUNC_CODE_BYTE_SIZE + 1,
    REG_BIT = 1 << FUNC_CODE_BYTE_SIZE + 2,
    MEM_BIT = 1 << FUNC_CODE_BYTE_SIZE + 3
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