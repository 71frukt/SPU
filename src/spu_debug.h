#ifndef SPU_DEBUG_H
#define SPU_DEBUG_H

#include "spu.h"

#ifdef DEBUG
#define SPU_DUMP(logfile, registers, cmd_ptr)  SpuDump(logfile, registers, cmd_ptr, __FILE__, __LINE__, __func__)

#else
#define SPU_DUMP 
#endif

void SpuDump(FILE *logfile, int *registers, cmd_t *cmd, const char *file, int line, const char *func);

#endif