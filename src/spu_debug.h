#ifndef SPU_DEBUG_H
#define SPU_DEBUG_H

#include "spu.h"

#ifdef DEBUG
#define SPU_DUMP(registers, cmd_ptr, logfile)  SpuDump(registers, cmd_ptr, logfile, __FILE__, __LINE__, __func__)

#else
#define SPU_DUMP 
#endif

void SpuDump(int *registers, cmd_t *cmd, FILE *logfile, const char *file, int line, const char *func);

#endif