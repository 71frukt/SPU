#include <stdio.h>

#include "spu.h"
#include "spu_debug.h"

const char  *code_file_name = "txts/program_code.txt";
const char  *logs_file_name = "txts/logs/spu_logs.log";

int SpuErr_val = 0;

void SpuCtor(spu_t *spu)
{
    cmd_t    *cmd       = &spu->cmd;
    int      *registers =  spu->registers;
    StackID  *data_stk  = &spu->data_stk;
    StackID  *func_stk  = &spu->func_stk;

    FILE    **code_file = &spu->code_file;
    ON_DEBUG(FILE **logfile = &spu->logfile);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
    {
        registers[i] = REGISTER_POISON;
    }

    *code_file = fopen(code_file_name, "r");
    ON_DEBUG(*logfile = fopen(logs_file_name, "w"));

    *cmd = {};

    for (cmd->size = 0; cmd->size < MAX_CMD_SIZE; cmd->size++)
    {
        if (fscanf(*code_file, "%d", cmd->code + cmd->size) != 1)
            break;
    }

    STACK_CTOR(data_stk, 0);
    STACK_CTOR(func_stk, 0);
    
    SPU_ASSERT(spu);
}

void SpuDtor(spu_t *spu)
{
    cmd_t    *cmd       = &spu->cmd;
    int      *registers =  spu->registers;
    StackID  *data_stk  = &spu->data_stk;
    StackID  *func_stk  = &spu->func_stk;

    FILE    **code_file = &spu->code_file;
    ON_DEBUG(FILE **logfile = &spu->logfile);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
        registers[i] = (int) REGISTER_POISON;

    for (size_t i = 0; i < cmd->size; i++)
        cmd->code[i] = CMD_POISON;

    *cmd = {};

    StackDtor(*func_stk);
    StackDtor(*data_stk);

    fclose(*code_file);
    ON_DEBUG(fclose(*logfile));
}

StkElmsCmpVal StkTwoLastElmsCmp(StackID stk)
{
    StackElem_t val_1 = 0;
    StackElem_t val_2 = 0;

    StackPop  (stk, &val_1);
    StackPop  (stk, &val_2);
    StackPush (stk,  val_2);
    StackPush (stk,  val_1);

    if (val_1 > val_2)
        return A;
    
    else if (val_1 >= val_2)
        return AE;
    
    else if (val_1 < val_2)
        return B;

    else if (val_1 <= val_2)
        return BE;

    else if (val_1 == val_2)
        return E;

    else
        return NE;
}