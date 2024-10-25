#ifndef COMPILER_H
#define COMPILER_H

#define CMP_DEBUG

#include <stdio.h>

#include "../commands_enum.h"
#include "compiler_debug.h"

#define MARK_SYMBOL     ":"
#define COMMENT_SYMBOL  ';'

const int   COMMAND_NAME_LEN = 100;
const int   MARK_NAME_LEN    = 100;
const int   MAX_ARG_NAME_LEN = 100;
const int   REG_NAME_LEN     = 2;

const int   REGISTERS_NUM    = 10;

const size_t   CMD_POISON       = 0xEBEFF;
const size_t   MARK_POISON      = 0xDEB41C;
const size_t   REGISTER_POISON  = 0xACCCCC;

const int   FUNC_CODE_BYTE_SIZE = 13;

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
    // trans_commands_t trans_commands;
    cmd_t cmd;
    fixup_t fixup;
    marklist_t marklist;

    FILE *asm_file;
    FILE *code_file;
    
    ON_COMPILER_DEBUG (FILE *logfile);
    int compiler_err;
};

enum ManagerBits
{
    IMM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 0),
    REG_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 1),
    RAM_BIT = 1 << (FUNC_CODE_BYTE_SIZE + 2)
};

void    CompilerCtor     (compiler_t *compiler, const char *code_file_name);
void    CompilerDtor     (compiler_t *compiler);

size_t  GetCountOfLines  (FILE *text);
size_t  GetCountOfWords  (FILE *text);

void    MakeFixUp        (compiler_t *compiler);
// void    GetCommands      (const char *file_name, trans_commands_t *trans_commands);
void    WriteCommandCode (char *cur_command_name, compiler_t *compiler);

int     ReadRegister     (char *reg_name);
bool    IsRegister       (char *reg_name);

int     FindMarkInList   (char *mark_name, marklist_t *marklist);
bool    IsMark           (char *str);

void    PrintCMD         (compiler_t *compiler);

void CompilerAssert      (compiler_t *compiler, const char *file, int line, const char *func);
int  CompilerVerify      (compiler_t *compiler);
void CompilerDump        (compiler_t *compiler, const char *file, int line, const char *func);
void PrintCompilerErr    (int error);

#endif