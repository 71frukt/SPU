#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "spu.h"
#include "spu_debug.h"

int main()
{
    spu_t spu = {};

    SpuCtor(&spu);
    cmd_t *cmd       = &spu.cmd;
    int   *registers =  spu.registers;
    FILE  *code_file =  spu.code_file;
    ON_DEBUG(FILE *logfile = spu.logfile);

    StackID stk = {};
    STACK_CTOR(&stk, 0);

    bool keep_doing = true;

    while (cmd->ip < cmd->size && keep_doing)
    {
        switch (cmd->code[cmd->ip])
        {
        case PUSH:
        {
            SPU_DUMP(&spu);

            StackElem_t arg = cmd->code[++cmd->ip];
            
            StackPush(stk, arg);
            cmd->ip++;
            
            break;
        }

        case ADD:
        {
            SPU_DUMP(&spu);

            StackElem_t a = 0;
            StackElem_t b = 0; 
            
            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a + b); 
            cmd->ip++;
            
            break;
        }

        case MUL:
        {
            SPU_DUMP(&spu);

            StackElem_t a = 0;
            StackElem_t b = 0;

            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a * b);
            cmd->ip++;

            break;
        }

        case DIV:
        {
            SPU_DUMP(&spu);

            StackElem_t divisible = 0;
            StackElem_t splitter  = 0;

            StackPop(stk, &divisible);
            StackPop(stk, &splitter);

            StackPush(stk, divisible / splitter);
            cmd->ip++;

            break;
        }

        case OUT:
        {
            SPU_DUMP(&spu);

            StackElem_t res = 0;
            StackPop(stk, &res);
            fprintf(stderr, "res = %d\n", res);
            cmd->ip++;
            
            break;
        }

        case PUSHR:
        {
            SPU_DUMP(&spu);

            StackElem_t arg = cmd->code[++cmd->ip];

            StackPush(stk, registers[arg]);
            cmd->ip++;
            
            break;
        }

        case POPR:
        {
            SPU_DUMP(&spu);

            StackElem_t arg = cmd->code[++cmd->ip];

            StackPop(stk, &registers[arg]);
            cmd->ip++;
            
            break;
        }

        case JUMP:
        {
            SPU_DUMP(&spu);

            StackElem_t arg = cmd->code[++cmd->ip];
            cmd->ip = arg;
            
            break;     
        }

        case HLT:
        {
            SPU_DUMP(&spu);
            
            cmd->ip++;

            keep_doing = false;
            
            break;
        }

        default:
        {
            fprintf(code_file, "Syntax error: '%llu'\n", cmd->code[cmd->ip]);
            
            SPU_DUMP(&spu);
            break;
        }
        }
    }
    
    SPU_DUMP(&spu);

    SpuDtor(&spu);

    return 0;
}