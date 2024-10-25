#include "spu.h"
#include "spu_debug.h"

extern StackElem_t RAM[];

const char  *code_file_name = "txts/program_code.txt";
const char  *logs_file_name = "txts/logs/spu_logs.log";

void SpuCtor(spu_t *spu)
{
    cmd_t    *cmd       = &spu->cmd;
    int      *registers =  spu->registers;
    StackID  *func_stk  = &spu->func_stk;
    StackID  *data_stk  = &spu->data_stk;

    FILE    **code_file = &spu->code_file;
    ON_SPU_DEBUG(FILE **logfile = &spu->logfile);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
    {
        registers[i] = REGISTER_POISON;
    }

    *code_file = fopen(code_file_name, "r");
    ON_SPU_DEBUG(*logfile = fopen(logs_file_name, "w"));

    *cmd = {};

    for (cmd->size = 0; cmd->size < MAX_CMD_SIZE; cmd->size++)
    {
        if (fscanf(*code_file, "%d", cmd->code + cmd->size) != 1)
            break;
    }

    spu->spu_err = 0;

    STACK_CTOR(func_stk, 20);
    STACK_CTOR(data_stk, 20);
    
    SPU_ASSERT(spu);
}

void SpuDtor(spu_t *spu)
{
    cmd_t    *cmd       = &spu->cmd;
    int      *registers =  spu->registers;
    StackID  *data_stk  = &spu->data_stk;
    StackID  *func_stk  = &spu->func_stk;

    FILE    **code_file = &spu->code_file;
    ON_SPU_DEBUG(FILE **logfile = &spu->logfile);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
        registers[i] = (int) REGISTER_POISON;

    for (size_t i = 0; i < cmd->size; i++)
        cmd->code[i] = CMD_POISON;

    *cmd = {};

    StackDtor(*func_stk);
    StackDtor(*data_stk);

    fclose(*code_file);
    ON_SPU_DEBUG(fclose(*logfile));
}

StackElem_t *GetArg(spu_t *spu)
{
    SPU_ASSERT(spu);

    // SPU_DUMP(spu);

    cmd_t *cmd = &spu->cmd;

    int func_code = cmd->code[cmd->ip++];

    static int arg_val;
    arg_val = 0;

    int *reg_ptr = NULL;
    int *arg_ptr = NULL;

    if ((func_code & REG_BIT) && (func_code & IMM_BIT) && !(func_code & RAM_BIT))
    {
        fprintf(stderr, "perhaps you forgot the \"[]\"?\n"); // TODO: fix error message
        spu->spu_err |= SYNTAX_ERR;
    }

    if (func_code & REG_BIT)
    {
        reg_ptr = &spu->registers[cmd->code[cmd->ip++]];
        arg_val += *reg_ptr;
    }

    if (func_code & IMM_BIT)
        arg_val += (int) cmd->code[cmd->ip++];

    // выбираем, чем будет arg_ptr

    if (func_code & RAM_BIT)    // записываем в RAM
    {
    // fprintf(stderr, "fuck_code = %d!\n", arg_val);
        if (arg_val < EXTRA_RAM_SIZE)
            arg_ptr = (int *) &RAM[arg_val];
        
        else
            spu->spu_err |= RAM_ERR;
    }

    else if (func_code & REG_BIT)
        arg_ptr = reg_ptr;
    
    else
        arg_ptr = &arg_val;

    SPU_ASSERT(spu);

    return (StackElem_t*) arg_ptr;
}

StkElmsCmpVal StkTwoLastElmsCmp(StackID stk)
{
    StackElem_t val_1 = 0;
    StackElem_t val_2 = 0;

    StackPop  (stk, &val_2);
    StackPop  (stk, &val_1);

    if (val_1 > val_2)
        return A;
    
    else if (val_1 < val_2)
        return B;  

    else
        return E;
}

int GetMaskForFunc()
{
    int mask = 0;
    mask |= 1 << (FUNC_CODE_BYTE_SIZE + 0);
    mask |= 1 << (FUNC_CODE_BYTE_SIZE + 1);
    mask |= 1 << (FUNC_CODE_BYTE_SIZE + 2);
    mask = ~mask;   // mask = 111..110001111111111111
    return mask;
}

void SetRandomRam()
{
    fprintf(stderr, "Start rand!\n");

    for (size_t i = 0; i < RAM_SIZE; i++)     // первая и последняя колонка - границы, там жизни нет
    {
        int tmp = rand() % START_CELL_FREQ;

        if (tmp == 0)
            RAM[i] = 1;

        else 
            RAM[i] = 0;

        // RAM[i] = (StackElem_t) (rand() % 2);

        // fprintf(stderr, "[%llu]\tram = %d\n", i, RAM[i]);
    }

    fprintf(stderr, "endrand\n");
}