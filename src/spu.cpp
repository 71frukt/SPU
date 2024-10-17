#include <stdio.h>

#include "spu.h"

const char  *code_file_name = "txts/program_code.txt";
const char  *logs_file_name = "txts/logs/spu_logs.log";

void SpuCtor(spu_t *spu)
{
    cmd_t *cmd       = &spu->cmd;
    int   *registers =  spu->registers;
    FILE **code_file = &spu->code_file;
    ON_DEBUG(FILE **logfile = &spu->logfile);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
    {
        registers[i] = (int) REGISTER_POISON;
    }

    *code_file = fopen(code_file_name, "r");
    ON_DEBUG(*logfile = fopen(logs_file_name, "w"));

    *cmd = {};

    for (cmd->size = 0; cmd->size < MAX_CMD_SIZE; cmd->size++)
    {
        if (fscanf(*code_file, "%d", cmd->code + cmd->size) != 1)
            break;
    }
}

void SpuDtor(spu_t *spu)
{
    cmd_t *cmd       = &spu->cmd;
    int *registers   =  spu->registers;
    FILE **code_file = &spu->code_file;
    ON_DEBUG(FILE **logfile = &spu->logfile);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
        registers[i] = (int) REGISTER_POISON;

    for (size_t i = 0; i < cmd->size; i++)
        cmd->code[i] = (int) CMD_POISON;

    *cmd = {};

    fclose(*code_file);
    ON_DEBUG(fclose(*logfile));
}