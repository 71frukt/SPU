#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "compiler_debug.h"

int main()
{
    compiler_t compiler = {};

    CompilerCtor(&compiler);

    char cur_command_name[COMMAND_NAME_LEN] = {};

    while (fscanf(compiler.asm_file, "%s", cur_command_name) == 1)
    {
        if (strchr(cur_command_name, COMMENT_SYMBOL) != NULL)
        {
            fscanf(compiler.asm_file, "%*[^\n]");
            continue;
        }

        if (IsMark(cur_command_name))
        {
            char mark_name[MARK_NAME_LEN] = {};
            sscanf(cur_command_name, "%[^" MARK_SYMBOL "]", mark_name);            

            // mark_t *mark = FindMarkInList(mark_name, &compiler.marklist);
            int num_in_marklist = FindMarkInList(mark_name, &compiler.marklist);
            mark_t *mark = &compiler.marklist.list[num_in_marklist];

// fprintf(stderr, "main: mark name = '%s'   num in marklist = %d\n\n", mark_name, num_in_marklist);

            if (num_in_marklist < 0)
            {    
                size_t list_ip = compiler.marklist.ip;
                compiler.marklist.list[list_ip].address = compiler.cmd.ip;
                strncat(compiler.marklist.list[list_ip].name, mark_name, MARK_NAME_LEN - 1);
                // sscanf(cur_command_name, "%[^" MARK_SYMBOL "]", marklist.list[marklist.ip].name);
                compiler.marklist.ip++;
            }

            else if (num_in_marklist >= 0 && mark->address == MARK_POISON)
            {
                mark->address = compiler.cmd.ip;
            }

            else if(mark->address != MARK_POISON)
            {
                fprintf(stderr, "ERROR: redefinition of mark '%s'\n", cur_command_name);
                ON_DEBUG(fprintf(compiler.logfile, "ERROR: redefinition of mark '%s'\n", cur_command_name));
            }
            
            continue;
        }

        WriteCommandCode(cur_command_name, &compiler);

        COMPILER_DUMP(&compiler);
    }
    
    MakeFixUp(&compiler);

    PrintCMD(&compiler);

    COMPILER_DUMP(&compiler);

    CompilerDtor(&compiler);

    return 0;
}
