#define DEF_CMD_PP_(name, num, ...)      \
    name = num,

#define DEF_CMD_JMP_(name, num, ...)     \
    name = num,

#define DEF_CMD_(name, num, ...)         \
    name = num,

enum FuncCodes
{
    #include "commands.h"
};

#undef DEF_CMD_PP_
#undef DEF_CMD_JMP_
#undef DEF_CMD_