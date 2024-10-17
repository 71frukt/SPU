#include <stdio.h>
#include "compiler.h"
#include "compiler_debug.h"

void CompilerDump(FILE *logfile, cmd_t *cmd, marklist_t *marklist, fixup_t *fixup, const char *file, int line, const char *func)
{
    fprintf(logfile, "SPU_DUMP called from %s:%d  (%s)\n{\n", file, line, func);

    fprintf(logfile, "\tCMD  [%p]:\n\t{\n", cmd);

    fprintf(logfile, "\t\tip   = %llu\n", cmd->ip);
    fprintf(logfile, "\t\tsize = %llu\n\n",   cmd->size);

    fprintf(logfile, "\t\tcode  [%p]\n\t\t{\n", cmd->code);
    for (size_t i = 0; i < cmd->size; i++)
    {
        if (cmd->code[i] == POISON)
            fprintf(logfile, "\t\t\t[%llu] \t=\t-POIZON-\n", i);

        else
            fprintf(logfile, "\t\t\t[%llu] \t=\t%d\n", i, cmd->code[i]);
    }

    fprintf(logfile, "\t\t}\n");
    fprintf(logfile, "\t}\n\n");

    fprintf(logfile, "\tMark list  [%p]:\n\t{\n", marklist);

    fprintf(logfile, "\t\tip   = %llu\n", marklist->ip);
    fprintf(logfile, "\t\tsize = %llu\n\n",   marklist->size);

    fprintf(logfile, "\t\tlist  [%p]\n\t\t{\n", marklist->list);
    for (size_t i = 0; i < marklist->ip; i++)
    {
        if (marklist->list[i].address == POISON)
            fprintf(logfile, "\t\t\t[%llu] : '%s'\t=\t-POISON-\n", i, marklist->list[i].name);
        else
            fprintf(logfile, "\t\t\t[%llu] : '%s'\t=\t%llu\n", i, marklist->list[i].name, marklist->list[i].address);
    }

    fprintf(logfile, "\t\t}\n");
    fprintf(logfile, "\t}\n");

    fprintf(logfile, "\tFixup  [%p]:\n\t{\n", fixup);

    fprintf(logfile, "\t\tip   = %llu\n",   fixup->ip);
    fprintf(logfile, "\t\tsize = %llu\n\n", fixup->size);

    fprintf(logfile, "\t\tdata  [%p]\n\t\t{\n", fixup->data);
    for (size_t i = 0; i < fixup->ip; i++)
    {
        fprintf(logfile, "\t\t\tnum_in_marklist | mark_ip\n");
        

        if (fixup->data[i].mark_ip == POISON)
            fprintf(logfile, "\t\t\t\t%llu\t\t\t\t-POIZON_", fixup->data[i].num_in_marklist);
        else
            fprintf(logfile, "\t\t\t\t%llu\t\t\t\t%llu", fixup->data[i].num_in_marklist, fixup->data[i].mark_ip);
    }

    fprintf(logfile, "\n\t\t}\n");
    fprintf(logfile, "\t}\n");
    
    fprintf(logfile, "}\n\n");
}
