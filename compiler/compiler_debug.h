#ifndef COMPILER_DEBUG_H
#define COMPILER_DEBUG_H

#include "compiler.h"

#ifdef DEBUG
#define COMPILER_DUMP(compiler)  CompilerDump(compiler, __FILE__, __LINE__, __func__)


#else
#define COMPILER_DUMP
#endif

void CompilerDump(compiler_t *compiler, const char *file, int line, const char *func);

#endif