#include "spu.h"
#include "stack.h"
#include "spu_debug.h"

extern int SpuErr_val;

StackElem_t RAM[EXTRA_RAM_SIZE] = {};

int main()
{
    spu_t spu;
    SpuCtor(&spu);
    SPU_DUMP(&spu);

    cmd_t   *cmd       = &spu.cmd;
    StackID  data_stk  = spu.data_stk;
    StackID  func_stk  = spu.func_stk;

    ON_SPU_DEBUG(FILE *logfile = spu.logfile);

    int mask = GetMaskForFunc();   // mask = 111..110001111111111111

    bool keep_doing = true;
// fprintf(stderr, "-1 & mask: %d\n", (-1) & mask); // 100..001

    #define DEF_CMD_PP_(command_name, command_num, ...)                                         \
        case command_num : SPU_ASSERT(&spu); __VA_ARGS__; SPU_ASSERT(&spu); break;

    #define DEF_CMD_JMP_(command_name, command_num, ...)                                        \
        case command_num : SPU_ASSERT(&spu); __VA_ARGS__; SPU_ASSERT(&spu); break;

    #define DEF_CMD_(command_name, command_num, ...)                                            \
        case command_num : SPU_ASSERT(&spu); __VA_ARGS__; SPU_ASSERT(&spu); break;

    while (cmd->ip < cmd->size && keep_doing)
    {
// fprintf(stderr, "cur_com = %d\n", cur_command);
        int cur_command = cmd->code[cmd->ip] & mask;

        switch (cur_command)    // команда без управл€ющих битов
        {
            #include "../commands.h"

            default:
                fprintf(stderr, "Syntax error: '%d'\n", cmd->code[cmd->ip]);
                ON_SPU_DEBUG(fprintf(logfile, "Syntax error: '%d'\n\n", cmd->code[cmd->ip]));
                
                SPU_ASSERT(&spu);
                break;
        }
    }

    #undef DEF_CMD_PP_
    #undef DEF_CMD_JMP_
    #undef DEF_CMD_
    
    SPU_ASSERT(&spu);

    SpuDtor(&spu);

    return 0;
}