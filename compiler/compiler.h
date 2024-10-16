#ifndef COMPILER_H
#define COMPILER_H

#define DEBUG

#include <stdio.h>

const int   COMMAND_NAME_LEN = 20;
const int   MARK_NAME_LEN   = 10;
const int   REG_NAME_LEN    = 2;

const int   REGISTERS_NUM   = 5;
const int   POISON          = 0xDEB41C;

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

void    GetCommands      (const char *file_name, trans_commands_t *trans_commands);
void    WriteCommandCode (char *cur_command_name, marklist_t *mark_list, FILE *asm_file, cmd_t *cmd);
size_t  GetCountOfLines  (FILE *text);
size_t  GetCountOfWords  (FILE *text);
int     ReadRegister     (char *reg_name);
void    PrintCMD         (cmd_t *cmd, FILE *file);
bool    IsMark           (char *str);
void    MarkVerify       (char *mark);
mark_t *FindMarkInList   (char *mark_name, marklist_t *list);



#endif