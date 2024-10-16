#include <stdio.h>
#include <string.h>

#include "stack.h"

#ifdef DEBUG
#define SPU_DUMP(registers, cmd_ptr, logfile)  SpuDump(registers, cmd_ptr, logfile, __FILE__, __LINE__, __func__)

#else
#define SPU_DUMP 
#endif


enum FuncCodes
{
    PUSH  = 1,
    ADD   = 2,
    SUB   = 3,
    MUL   = 4,
    DIV   = 5,
    PUSHR = 6,
    POPR  = 7,
    OUT   = 0,
    HLT   = -1
};

const size_t MAX_CMD_SIZE   = 500;
const char  *code_file_name = "txts/program_code.txt";
const char  *logs_file_name = "txts/spu_logs.txt";
const int    REGISTERS_NUM  = 5;

int REGISTERS[REGISTERS_NUM] = {};

typedef struct Cmd
{
    size_t pi;
    size_t size;
    int code[MAX_CMD_SIZE];
} cmd_t;

void Run();
void SpuDump(int *registers, cmd_t *cmd, FILE *logfile, const char *file, int line, const char *func);

int main()
{
    FILE *code_file = fopen(code_file_name, "r");
    FILE *log_file = fopen(logs_file_name, "w");

    cmd_t cmd = {.pi = 0};

    for (cmd.size = 0; cmd.size < MAX_CMD_SIZE; cmd.size++)
    {
        if (fscanf(code_file, "%d", cmd.code + cmd.size) != 1)
            break;
    }

    StackID stk = {};
    STACK_CTOR(&stk, 0);

    while (cmd.pi < cmd.size)
    {
        if (cmd.code[cmd.pi] == PUSH)
        {
            StackElem_t arg = cmd.code[++cmd.pi];

            StackPush(stk, arg);
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == ADD)
        {
            StackElem_t a = 0;
            StackElem_t b = 0; 
            
            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a + b); 
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == MUL)
        {
            StackElem_t a = 0;
            StackElem_t b = 0;

            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a * b);
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == DIV)
        {
            StackElem_t divisible = 0;
            StackElem_t splitter  = 0;

            StackPop(stk, &divisible);
            StackPop(stk, &splitter);

            StackPush(stk, divisible / splitter);
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == OUT)
        {

            StackElem_t res = 0;
            StackPop(stk, &res);
            fprintf(stderr, "res = %d\n", res);
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == PUSHR)
        {
            StackElem_t arg = cmd.code[++cmd.pi];

            StackPush(stk, REGISTERS[arg]);
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == POPR)
        {
            StackElem_t arg = cmd.code[++cmd.pi];

            StackPop(stk, &REGISTERS[arg]);
            cmd.pi++;
            continue;
        }

        if (cmd.code[cmd.pi] == HLT)
        {
            cmd.pi++;
            break;
        }

        fprintf(code_file, "Syntax error: '%llu'\n", cmd.code[cmd.pi]);
    }
    
    ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

    fclose(log_file);
    fclose(code_file);

    return 0;
}

void SpuDump(int *registers, cmd_t *cmd, FILE *logfile, const char *file, int line, const char *func)
{
    fprintf(logfile, "SPU_DUMP called from %s:%d  (%s)\n{\n", file, line, func);

    fprintf(logfile, "\tRegisters  [%p]:\n\t{\n", registers);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
        fprintf(logfile, "\t\t[%llu] = %d\n", i, registers[i]);

    fprintf(logfile, "\t}\n\n");

    fprintf(logfile, "\tCMD  [%p]:\n\t{\n", cmd);

    fprintf(logfile, "\t\tsize = %llu\n", cmd->size);
    fprintf(logfile, "\t\tpi   = %llu\n\n", cmd->pi);

    fprintf(logfile, "\t\tcode  [%p]\n\t\t{\n", cmd->code);
    for (size_t i = 0; i < cmd->size; i++)
        fprintf(logfile, "\t\t\t[%llu] \t=\t%d\n", i, cmd->code[i]);

    fprintf(logfile, "\t\t}\n");

    fprintf(logfile, "\t}\n");

    fprintf(logfile, "}");
}