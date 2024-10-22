#ifndef SPU_DEBUG_H
#define SPU_DEBUG_H

#include "spu.h"

enum SPU_Error
{
    SPU_OK = 0,

    SPU_PTR_ERR   = (1 << 0),
    REGISTERS_ERR = (1 << 1),
    RAM_ERR       = (1 << 2),
    CMD_ERR       = (1 << 3),
    CODE_ERR      = (1 << 4),
    LOGFILE_ERR   = (1 << 5),
    SYNTAX_ERR    = (1 << 6)
};

#define SPU_DUMP(spu_ptr)  SpuDump(spu_ptr, __FILE__, __LINE__, __func__)


#ifdef SPU_DEBUG

#define ON_SPU_DEBUG(...)  __VA_ARGS__


#define SPU_ASSERT(spu)                                                     \
{                                                                           \
    SpuAssert(spu, __FILE__, __LINE__, __func__);                           \
    /*SPU_DUMP(spu);*/                                                      \
}                                                                           \

#else
#define ON_SPU_DEBUG(...)
#define SPU_ASSERT(spu)
#endif

#endif