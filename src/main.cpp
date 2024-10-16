#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "spu.h"
#include "spu_debug.h"

const char  *code_file_name = "txts/program_code.txt";
const char  *logs_file_name = "txts/spu_logs.txt";
int REGISTERS[REGISTERS_NUM] = {};

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

    bool keep_doing = true;

    while (cmd.pi < cmd.size && keep_doing)
    {
        switch (cmd.code[cmd.pi])
        {
        case PUSH:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t arg = cmd.code[++cmd.pi];

            StackPush(stk, arg);
            cmd.pi++;
            
            break;
        }

        case ADD:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t a = 0;
            StackElem_t b = 0; 
            
            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a + b); 
            cmd.pi++;
            
            break;
        }

        case MUL:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t a = 0;
            StackElem_t b = 0;

            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a * b);
            cmd.pi++;

            break;
        }

        case DIV:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t divisible = 0;
            StackElem_t splitter  = 0;

            StackPop(stk, &divisible);
            StackPop(stk, &splitter);

            StackPush(stk, divisible / splitter);
            cmd.pi++;

            break;
        }

        case OUT:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t res = 0;
            StackPop(stk, &res);
            fprintf(stderr, "res = %d\n", res);
            cmd.pi++;
            
            break;
        }

        case PUSHR:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t arg = cmd.code[++cmd.pi];

            StackPush(stk, REGISTERS[arg]);
            cmd.pi++;
            
            break;
        }

        case POPR:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t arg = cmd.code[++cmd.pi];

            StackPop(stk, &REGISTERS[arg]);
            cmd.pi++;
            
            break;
        }

        case JUMP:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

            StackElem_t arg = cmd.code[++cmd.pi];
            cmd.pi = arg;
            
            break;     
        }

        case HLT:
        {
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));
            
            cmd.pi++;

            keep_doing = false;
            
            break;
        }

        default:
        {
            fprintf(code_file, "Syntax error: '%llu'\n", cmd.code[cmd.pi]);
            
            ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));
            break;
        }
        }
    }
    
    ON_DEBUG(SPU_DUMP(REGISTERS, &cmd, log_file));

    fclose(log_file);
    fclose(code_file);

    return 0;
}