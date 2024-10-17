#include <stdio.h>
#include "compiler.h"
#include "compiler_debug.h"

void CompilerAssert(compiler_t *compiler, int *cmp_err, const char *file, int line, const char *func)
{
    *cmp_err = CompilerVerify(compiler);

    if (*cmp_err != 0)
    {
        fprintf(stderr, "myassertion failed in\t%s:%d\nErrors:\t", file, line);
        PrintCompilerErr(*cmp_err);
    }
}

void PrintCompilerErr(int error)
{   
    #define PRINT_ERROR(err, code)                      \
    if (err & code)                                     \
    {                                                   \
        fprintf(stderr, #code);                         \
        fprintf(stderr, " ");                           \
    }                                                   
    
    PRINT_ERROR (error, CMP_PTR_ERR);
    PRINT_ERROR (error, ASM_FILE_ERR);
    PRINT_ERROR (error, CODE_FILE_ERR);
    PRINT_ERROR (error, TRANS_ERR);
    PRINT_ERROR (error, CMD_ERR);
    PRINT_ERROR (error, FIXUP_ERR);
    PRINT_ERROR (error, MARKLIST_ERR);

    #undef PRINT_ERROR  

    printf("\n");
}

int CompilerVerify(compiler_t *compiler)
{
    FILE             *asm_file       =  compiler->asm_file;
    FILE             *code_file      =  compiler->code_file;
    cmd_t            *cmd            = &compiler->cmd;
    fixup_t          *fixup          = &compiler->fixup;
    marklist_t       *marklist       = &compiler->marklist;
    trans_commands_t *trans_commands = &compiler->trans_commands;

    int res_err = 0;

    if (compiler == NULL)
    {
        res_err |= CMP_PTR_ERR;
        return res_err;
    }

    if (asm_file == NULL)
        res_err |= ASM_FILE_ERR;
    
    if (code_file == NULL)
        res_err |= CODE_FILE_ERR;

    if (cmd == NULL || cmd->code == NULL || cmd->ip > cmd->size)
        res_err |= CMD_ERR;

    if (fixup == NULL || fixup->data == NULL || fixup->ip > fixup->size)
        res_err |= FIXUP_ERR;

    if (marklist == NULL || marklist->list == NULL || marklist->ip > marklist->size)
        res_err |= MARKLIST_ERR;

    if (trans_commands == NULL || trans_commands->commands == NULL)
        res_err |= TRANS_ERR;

    return res_err;
}

void CompilerDump(compiler_t *compiler, const char *file, int line, const char *func)
{
    ON_DEBUG(FILE *logfile =  compiler->logfile);
    FILE       *asm_file   =  compiler->asm_file;
    cmd_t      *cmd        = &compiler->cmd;
    fixup_t    *fixup      = &compiler->fixup;
    marklist_t *marklist   = &compiler->marklist;

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
