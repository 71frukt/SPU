#ifndef COMPILER_DEBUG_H
#define COMPILER_DEBUG_H

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

#ifdef CMP_DEBUG
#define ON_COMPILER_DEBUG(...)  __VA_ARGS__
#define COMPILER_ASSERT(compiler)  CompilerAssert(compiler, __FILE__, __LINE__, __func__)
#define COMPILER_DUMP(compiler)    CompilerDump(compiler, __FILE__, __LINE__, __func__)

#else
#define ON_COMPILER_DEBUG(...)
#define COMPILER_DUMP
#endif

#endif