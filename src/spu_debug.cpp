#include <stdio.h>

#include "spu.h"
#include "spu_debug.h"

void SpuAssert(spu_t *spu, int *spu_err, const char *file, int line, const char *func)
{
    *spu_err = SpuVerify(spu);

    if (*spu_err != 0)
    {
        fprintf(stderr, "myassertion failed in\t%s:%d\nErrors:\t", file, line);
        PrintSpuErr(*spu_err);
    }
}

void PrintSpuErr(int error)
{   
    #define PRINT_ERROR(err, code)                      \
    if (err & code)                                     \
    {                                                   \
        fprintf(stderr, #code);                         \
        fprintf(stderr, " ");                           \
    }                                                   
    
    PRINT_ERROR (error, SPU_PTR_ERR);
    PRINT_ERROR (error, REGISTERS_ERR);
    PRINT_ERROR (error, CMD_ERR);
    PRINT_ERROR (error, CODE_ERR);
    PRINT_ERROR (error, LOGFILE_ERR);

    #undef PRINT_ERROR  

    printf("\n");
}

int SpuVerify(spu_t *spu)
{
    cmd_t *cmd       = &spu->cmd;
    int   *registers =  spu->registers;
    FILE  *code_file =  spu->code_file;
    ON_DEBUG(FILE *logfile = spu->logfile);

    int res_err = 0;

    if (spu == NULL)
        res_err |= SPU_PTR_ERR;
    
    if (cmd == NULL || cmd->code == NULL || cmd->ip > cmd->size)
        res_err |= CMD_ERR;

    if (registers == NULL)
        res_err |= REGISTERS_ERR;

    if (code_file == NULL)
        res_err |= CODE_ERR;

    ON_DEBUG(
        if (logfile == NULL)
            res_err |= LOGFILE_ERR;
    )

    return res_err;
}


void SpuDump(spu_t *spu, const char *file, int line, const char *func)
{
    int   *registers =  spu->registers;
    cmd_t *cmd       = &spu->cmd;
    FILE  *code_file =  spu->code_file;
    FILE  *logfile   =  spu->logfile;

    fprintf(logfile, "SPU_DUMP called from %s:%d  (%s)\n{\n", file, line, func);

    fprintf(logfile, "\tRegisters  [%p]:\n\t{\n", registers);

    for (size_t i = 0; i < REGISTERS_NUM; i++)
    {
        if (registers[i] == REGISTER_POISON)
            fprintf(logfile, "\t\t[%llu] = -REGISTOR_POISON-\n", i);

        else
            fprintf(logfile, "\t\t[%llu] = %d\n", i, registers[i]);
    }

    fprintf(logfile, "\t}\n\n");

    fprintf(logfile, "\tCMD  [%p]:\n\t{\n", cmd);

    fprintf(logfile, "\t\tip   = %llu\n", cmd->ip);
    fprintf(logfile, "\t\tsize = %llu\n\n", cmd->size);

    fprintf(logfile, "\t\tcode  [%p]\n\t\t{\n", cmd->code);
    for (size_t i = 0; i < cmd->size; i++)
        fprintf(logfile, "\t\t\t[%llu] \t=\t%d\n", i, cmd->code[i]);

    fprintf(logfile, "\t\t}\n");

    fprintf(logfile, "\t}\n");

    fprintf(logfile, "}\n\n\n");
}