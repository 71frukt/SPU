#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"
#include "spu.h"
#include "spu_debug.h"

extern int SpuErr_val;

int main()
{
    spu_t spu = {};

    SpuCtor(&spu);
    
    cmd_t   *cmd       = &spu.cmd;
    int     *registers = spu.registers;
    StackID  data_stk  = spu.data_stk;
    StackID  func_stk  = spu.func_stk;

    FILE  *code_file =  spu.code_file;
    ON_DEBUG(FILE *logfile = spu.logfile);

    int mask = GetMaskForFunc();   // mask = 111..110001111111111111

    bool keep_doing = true;
// fprintf(stderr, "-1 & mask: %d\n", (-1) & mask); // 100..001

    while (cmd->ip < cmd->size && keep_doing)
    {
        int cur_command = cmd->code[cmd->ip] & mask;

        switch (cur_command)    // ������� ��� ����������� �����
        {
        case PUSH:
        {
            SPU_ASSERT(&spu);

            StackElem_t *arg = GetArg(&spu);
            StackPush(data_stk, *arg);

            break;
        }
    
        case POP:
        {
            SPU_ASSERT(&spu);

            StackElem_t *arg = GetArg(&spu);

            StackPop(data_stk, arg);
            
            break;
        }

        case ADD:
        {
            SPU_ASSERT(&spu);

            StackElem_t a = 0;
            StackElem_t b = 0; 
            
            StackPop(data_stk, &a);
            StackPop(data_stk, &b);

            StackPush(data_stk, a + b); 
            cmd->ip++;
            
            break;
        }

        case SUB:
        {
            SPU_ASSERT(&spu);

            StackElem_t a = 0;
            StackElem_t b = 0; 
            
            StackPop(data_stk, &a);
            StackPop(data_stk, &b);

            StackPush(data_stk, a - b); 
            cmd->ip++;
            
            break;
        }

        case MUL:
        {
            SPU_ASSERT(&spu);

            StackElem_t a = 0;
            StackElem_t b = 0;

            StackPop(data_stk, &a);
            StackPop(data_stk, &b);

            StackPush(data_stk, a * b);
            cmd->ip++;

            break;
        }

        case DIV:
        {
            SPU_ASSERT(&spu);

            StackElem_t divisible = 0;
            StackElem_t splitter  = 0;

            StackPop(data_stk, &splitter);
            StackPop(data_stk, &divisible);

            StackPush(data_stk, divisible / splitter);
            cmd->ip++;

            break;
        }

        // case PUSHR:
        // {
        //     SPU_ASSERT(&spu);

        //     StackElem_t arg = cmd->code[++cmd->ip];

        //     StackPush(data_stk, registers[arg]);
        //     cmd->ip++;
            
        //     break;
        // }

        case CALL:
        {
            SPU_ASSERT(&spu);

            StackPush(func_stk, cmd->ip + 2);

            goto JUMP_MARK;

            break;
        }

        case RET:
        {
            SPU_ASSERT(&spu);

            int tmp = cmd->ip;
            StackPop(func_stk, &tmp);
            cmd->ip = (size_t) tmp; 

            SPU_ASSERT(&spu);
            break;
        }

        case JUMP:
        {
            JUMP_MARK:

            SPU_ASSERT(&spu);

            cmd->ip = cmd->code[cmd->ip + 1];
            SPU_ASSERT(&spu);
            break;     
        }

        case JA:
        {
            if (StkTwoLastElmsCmp(data_stk) == A)
                goto JUMP_MARK;

            else
                cmd->ip += 2;

            break;
        }

        case JAE:
        {
            if (StkTwoLastElmsCmp(data_stk) == AE)
                goto JUMP_MARK;

            else
                cmd->ip += 2;

            break;
        }

        case JB:
        {
            if (StkTwoLastElmsCmp(data_stk) == B)
                goto JUMP_MARK;

            else
                cmd->ip += 2;

            break;
        }

        case JBE:
        {
            if (StkTwoLastElmsCmp(data_stk) == BE)
                goto JUMP_MARK;

            else
                cmd->ip += 2;

            break;
        }

        case JE:
        {
            if (StkTwoLastElmsCmp(data_stk) == E)
                goto JUMP_MARK;

            else
                cmd->ip += 2;

            break;
        }

        case JNE:
        {
            if (StkTwoLastElmsCmp(data_stk) == NE)
                goto JUMP_MARK;

            else
                cmd->ip += 2;
                
            break;
        }

        case DRAW:
        {
            SPU_ASSERT(&spu);

            Draw(&spu);

            cmd->ip++;

            SPU_ASSERT(&spu);

            break;
        }

        case SQRT:
        {
            SPU_ASSERT(&spu);

            StackElem_t elem = 0;
            StackPop(data_stk, &elem);

            elem = (StackElem_t) sqrt(elem);

            StackPush(data_stk, elem);

            cmd->ip++;

            break;
            SPU_ASSERT(&spu);
        }

        case MOD:
        {
            SPU_ASSERT(&spu);

            StackElem_t divisible = 0;
            StackElem_t splitter  = 0;

            StackPop(data_stk, &splitter);
            StackPop(data_stk, &divisible);

            StackPush(data_stk, divisible % splitter);
            cmd->ip++;

            break;
        }

        case OUT:
        {
            SPU_ASSERT(&spu);

            StackElem_t res = 0;
            StackPop(data_stk, &res);
            cmd->ip++;
            
            SPU_ASSERT(&spu);
            break;
        }

        case HLT:
        {
            SPU_ASSERT(&spu);
            
            cmd->ip++;

            keep_doing = false;
            break;
        }

        default:
        {
            fprintf(stderr, "Syntax error: '%llu'\n", cmd->code[cmd->ip]);
            ON_DEBUG(fprintf(logfile, "Syntax error: '%llu'\n\n", cmd->code[cmd->ip]));
            
            SPU_ASSERT(&spu);
            break;
        }
        }
    }
    
    SPU_ASSERT(&spu);

    SpuDtor(&spu);

    return 0;
}