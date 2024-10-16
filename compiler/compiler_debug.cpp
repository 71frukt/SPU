#include <stdio.h>
#include "compiler.h"
#include "compiler_debug.h"

void CompilerDump(FILE *logfile, cmd_t *cmd, marklist_t *marklist, const char *file, int line, const char *func)
{
    fprintf(logfile, "SPU_DUMP called from %s:%d  (%s)\n{\n", file, line, func);

    fprintf(logfile, "\tCMD  [%p]:\n\t{\n", cmd);

    fprintf(logfile, "\t\tpi   = %llu\n", cmd->ip);
    fprintf(logfile, "\t\tsize = %llu\n\n",   cmd->size);

    fprintf(logfile, "\t\tcode  [%p]\n\t\t{\n", cmd->code);
    for (size_t i = 0; i < cmd->size; i++)
        fprintf(logfile, "\t\t\t[%llu] \t=\t%d\n", i, cmd->code[i]);

    fprintf(logfile, "\t\t}\n");
    fprintf(logfile, "\t}\n\n");

    fprintf(logfile, "\tMark list  [%p]:\n\t{\n", marklist);

    fprintf(logfile, "\t\tpi   = %llu\n", marklist->ip);
    fprintf(logfile, "\t\tsize = %llu\n\n",   marklist->size);

    fprintf(logfile, "\t\tlist  [%p]\n\t\t{\n", marklist->list);
    for (size_t i = 0; i < marklist->size; i++)
    {
        if (marklist->list[i].address == POISON)
            fprintf(logfile, "\t\t\t[%llu] : %s\t=\t-POISON-\n", i, marklist->list[i].name);
        else
            fprintf(logfile, "\t\t\t[%llu] : %s\t=\t%llu\n", i, marklist->list[i].name, marklist->list[i].address);
    }

    fprintf(logfile, "\t\t}\n");
    fprintf(logfile, "\t}\n");
    
    fprintf(logfile, "}\n");
}
