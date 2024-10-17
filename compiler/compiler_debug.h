#ifndef COMPILER_DEBUG_H
#define COMPILER_DEBUG_H

#include "compiler.h"

enum CompilerError
{
    CMP_OK = 0,

    CMP_PTR_ERR   = (1 << 0),
    ASM_FILE_ERR  = (1 << 1),
    CODE_FILE_ERR = (1 << 2),
    TRANS_ERR     = (1 << 3),
    CMD_ERR       = (1 << 4),
    FIXUP_ERR     = (1 << 5),
    MARKLIST_ERR  = (1 << 6)
};

#ifdef DEBUG
#define COMPILER_ASSERT(compiler)  CompilerAssert(compiler, &CompilerError_val, __FILE__, __LINE__, __func__)
#define COMPILER_DUMP(compiler)    CompilerDump(compiler, __FILE__, __LINE__, __func__)


#else
#define COMPILER_DUMP
#endif

void CompilerAssert   (compiler_t *compiler, int *cmp_err, const char *file, int line, const char *func);
int  CompilerVerify   (compiler_t *compiler);
void CompilerDump     (compiler_t *compiler, const char *file, int line, const char *func);
void PrintCompilerErr (int error);

#endif