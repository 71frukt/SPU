#ifndef SPU_DEBUG_H
#define SPU_DEBUG_H

#include "spu.h"

enum SPU_Error
{
    SPU_OK = 0,

    SPU_PTR_ERR   = (1 << 0),
    REGISTERS_ERR = (1 << 1),
    CMD_ERR       = (1 << 2),
    CODE_ERR      = (1 << 3),
    LOGFILE_ERR   = (1 << 4)
};

#ifdef DEBUG

#define SPU_DUMP(spu_ptr)    SpuDump  (spu_ptr, __FILE__, __LINE__, __func__)

#define SPU_ASSERT(spu)                                                     \
{                                                                           \
    SpuAssert(spu, &SpuErr_val, __FILE__, __LINE__, __func__);              \
    SPU_DUMP(spu);                                                          \
}                                                                           \

#else
#define SPU_DUMP 
#endif

void SpuAssert   (spu_t *compiler, int *spu_err, const char *file, int line, const char *func);
int  SpuVerify   (spu_t *spu);
void SpuDump     (spu_t *spu, const char *file, int line, const char *func);
void PrintSpuErr (int error);

#endif