#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "spu.h"
#include "spu_debug.h"

const char  *code_file_name = "txts/program_code.txt";
const char  *logs_file_name = "txts/logs/spu_logs.txt";

int main()
{
    int REGISTERS[REGISTERS_NUM] = {};
    FILE *code_file = fopen(code_file_name, "r");
    ON_DEBUG(FILE *log_file = fopen(logs_file_name, "w"));

    cmd_t cmd = {.ip = 0}; // TODO: ip

    for (cmd.size = 0; cmd.size < MAX_CMD_SIZE; cmd.size++)
    {
        if (fscanf(code_file, "%d", cmd.code + cmd.size) != 1)
            break;
    }

    StackID stk = {};
    STACK_CTOR(&stk, 0);

    bool keep_doing = true;

    while (cmd.ip < cmd.size && keep_doing)
    {
        switch (cmd.code[cmd.ip])
        {
        case PUSH:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t arg = cmd.code[++cmd.ip];

            StackPush(stk, arg);
            cmd.ip++;
            
            break;
        }

        case ADD:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t a = 0;
            StackElem_t b = 0; 
            
            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a + b); 
            cmd.ip++;
            
            break;
        }

        case MUL:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t a = 0;
            StackElem_t b = 0;

            StackPop(stk, &a);
            StackPop(stk, &b);

            StackPush(stk, a * b);
            cmd.ip++;

            break;
        }

        case DIV:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t divisible = 0;
            StackElem_t splitter  = 0;

            StackPop(stk, &divisible);
            StackPop(stk, &splitter);

            StackPush(stk, divisible / splitter);
            cmd.ip++;

            break;
        }

        case OUT:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t res = 0;
            StackPop(stk, &res);
            fprintf(stderr, "res = %d\n", res);
            cmd.ip++;
            
            break;
        }

        case PUSHR:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t arg = cmd.code[++cmd.ip];

            StackPush(stk, REGISTERS[arg]);
            cmd.ip++;
            
            break;
        }

        case POPR:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t arg = cmd.code[++cmd.ip];

            StackPop(stk, &REGISTERS[arg]);
            cmd.ip++;
            
            break;
        }

        case JUMP:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

            StackElem_t arg = cmd.code[++cmd.ip];
            cmd.ip = arg;
            
            break;     
        }

        case HLT:
        {
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));
            
            cmd.ip++;

            keep_doing = false;
            
            break;
        }

        default:
        {
            fprintf(code_file, "Syntax error: '%llu'\n", cmd.code[cmd.ip]);
            
            ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));
            break;
        }
        }
    }
    
    ON_DEBUG(SPU_DUMP(log_file, REGISTERS, &cmd));

    ON_DEBUG(fclose(log_file));
    fclose(code_file);

    return 0;
}