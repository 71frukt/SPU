#ifndef COMPILER_H
#define COMPILER_H

#define DEBUG

#include <stdio.h>

#define MARK_SYMBOL ":"

const int   COMMAND_NAME_LEN = 20;
const int   MARK_NAME_LEN    = 10;
const int   REG_NAME_LEN     = 2;

const int   REGISTERS_NUM    = 5;

const int   CMD_POISON       = 0xEBEBEB;
const int   MARK_POISON      = 0xDEB41C;
const int   REGISTER_POISON  = 0xEB1A4;

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
    size_t ip;
    size_t size;
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
    POPR  =  7,
    JUMP  =  8,
    CALL  =  9,
    RET   =  10,
    OUT   =  0,
    HLT   =  -1
};

void    CompilerCtor     (compiler_t *compiler);
void    CompilerDtor     (compiler_t *compiler);
void    PrintCMD         (compiler_t *compiler);
void    MakeFixUp        (compiler_t *compiler);
void    GetCommands      (const char *file_name, trans_commands_t *trans_commands);
void    WriteCommandCode (char *cur_command_name, compiler_t *compiler);
size_t  GetCountOfLines  (FILE *text);
size_t  GetCountOfWords  (FILE *text);
int     ReadRegister     (char *reg_name);
bool    IsMark           (char *str);
mark_t *FindMarkInList   (char *mark_name, marklist_t *list);

#endif