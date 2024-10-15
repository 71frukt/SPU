#include <stdio.h>
#include <string.h>

#include "stack.h"

enum FuncCodes
{
    PUSH = 1,
    ADD  = 2,
    SUB  = 3,
    MUL  = 4,
    DIV  = 5,
    OUT  = 0,
    HLT  = -1
};

const size_t MAX_CMD_SIZE = 50;
const char  *code_file_name = "txts/program_code.txt";

typedef struct Cmd
{
    size_t pi;
    size_t size;
    int code[MAX_CMD_SIZE];
} cmd_t;

void Run();

int main()
{
    Run();
    return 0;
}

void Run()
{
    FILE *code_file = fopen(code_file_name, "r");

    cmd_t cmd = {};

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

        if (cmd.code[cmd.pi] == HLT)
        {
            cmd.pi++;
            break;
        }

        fprintf(code_file, "Syntax error: '%llu'\n", cmd.code[cmd.pi]);
    }

    fclose(code_file);
}