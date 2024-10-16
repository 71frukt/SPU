#ifndef COMPILER_DEBUG_H
#define COMPILER_DEBUG_H

#include "compiler.h"

#ifdef DEBUG
#define ON_DEBUG(...)  __VA_ARGS__
#define COMPILER_DUMP(logfile, cmd, marklist)  CompilerDump(logfile, cmd, marklist, __FILE__, __LINE__, __func__)


#else
#define ON_DEBUG(...)
#endif

void CompilerDump(FILE *logfile, cmd_t *cmd, marklist_t *marklist, const char *file, int line, const char *func);

#endif