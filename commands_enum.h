#define DEF_CMD_(name, num)     \
    name = num

enum FuncCodes
{
    #include "commands.h"
};

#undef DEF_CMD_
// TODO: SPU_(name)