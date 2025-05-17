#include "compiler.h"
#include "compiler_debug.h"
#include "assert.h"

void CompilerAssert(compiler_t *compiler, const char *file, int line, const char *func)
{
    compiler->compiler_err |= CompilerVerify(compiler);

    if (compiler->compiler_err != 0)
    {
        fprintf(stderr, "my assertion failed in\t%s:%d  (%s)\nErrors:\t", file, line, func);
        PrintCompilerErr(compiler->compiler_err);
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
    FILE       *asm_file  =  compiler->asm_file;
    FILE       *code_file =  compiler->code_file;
    cmd_t      *cmd       = &compiler->cmd;
    fixup_t    *fixup     = &compiler->fixup;
    marklist_t *marklist  = &compiler->marklist;

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

    return res_err;
}

void CompilerDump(compiler_t *compiler, const char *file, int line, const char *func)
{
    ON_COMPILER_DEBUG(FILE *logfile =  compiler->logfile);

    // FILE       *asm_file   =  compiler->asm_file;
    cmd_t      *cmd        = &compiler->cmd;
    fixup_t    *fixup      = &compiler->fixup;
    marklist_t *marklist   = &compiler->marklist;
assert(logfile);
    fprintf(logfile, "Compiler called from %s:%d  (%s)\n{\n", file, line, func);

    fprintf(logfile, "\tCMD  [%p]:\n\t{\n", cmd);

    fprintf(logfile, "\t\tip   = %lu\n", cmd->ip);
    fprintf(logfile, "\t\tsize = %lu\n\n",   cmd->size);

    fprintf(logfile, "\t\tcode  [%p]\n\t\t{\n", cmd->code);
    for (size_t i = 0; i < cmd->ip; i++)
    {
        fprintf(logfile, "\t\t\t[%lu] \t=\t%d", i, cmd->code[i]);


        if (cmd->code[i] == MARK_POISON)
            fprintf(logfile, "\t(MARK_POISON)");

        else if (cmd->code[i] == CMD_POISON)
            fprintf(logfile, "\t(CMD_POISON)");

        else if (cmd->code[i] > 0)  
        {
            if (cmd->code[i] & RAM_BIT)
                fprintf(logfile, " mem_bit ");

            if (cmd->code[i] & REG_BIT)
                fprintf(logfile, " reg_bit ");

            if (cmd->code[i] & IMM_BIT)
                fprintf(logfile, " imm_bit ");        
        }

        fprintf(logfile, "\n");
    }

    fprintf(logfile, "\t\t}\n");
    fprintf(logfile, "\t}\n\n");

    fprintf(logfile, "\tMark list  [%p]:\n\t{\n", marklist);

    fprintf(logfile, "\t\tip   = %lu\n", marklist->ip);
    fprintf(logfile, "\t\tsize = %lu\n\n",   marklist->size);

    fprintf(logfile, "\t\tlist  [%p]\n\t\t{\n", marklist->list);
    for (size_t i = 0; i < marklist->ip; i++)
    {
        if (marklist->list[i].address == MARK_POISON)
            fprintf(logfile, "\t\t\t[%lu] : '%s'\t=\t-MARK_POISON-\n", i, marklist->list[i].name);
        else
            fprintf(logfile, "\t\t\t[%lu] : '%s'\t=\t%lu\n", i, marklist->list[i].name, marklist->list[i].address);
    }

    fprintf(logfile, "\t\t}\n");
    fprintf(logfile, "\t}\n\n");

    fprintf(logfile, "\tFixup  [%p]:\n\t{\n", fixup);

    fprintf(logfile, "\t\tip   = %lu\n",   fixup->ip);
    fprintf(logfile, "\t\tsize = %lu\n\n", fixup->size);

    fprintf(logfile, "\t\tdata  [%p]\n\t\t{\n", fixup->data);

    fprintf(logfile, "\t\t\tnum_in_marklist | mark_ip\n");
    for (size_t i = 0; i < fixup->ip; i++)
    {    
        if (fixup->data[i].mark_ip == MARK_POISON)
            fprintf(logfile, "\t\t\t\t%lu\t\t\t\t-MARK_POISON-\n", fixup->data[i].num_in_marklist);
        else
            fprintf(logfile, "\t\t\t\t%lu\t\t\t\t%lu\n", fixup->data[i].num_in_marklist, fixup->data[i].mark_ip);
    }

    fprintf(logfile, "\n\t\t}\n");
    fprintf(logfile, "\t}\n");
    
    fprintf(logfile, "}\n\n\n");
}
