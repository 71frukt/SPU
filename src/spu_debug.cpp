#include <stdio.h>

#include "spu.h"
#include "spu_debug.h"

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