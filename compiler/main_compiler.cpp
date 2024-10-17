#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "compiler_debug.h"

int main()
{
    compiler_t compiler = {};

    CompilerCtor(&compiler);

    // FILE *asm_file  = fopen(asm_file_name,  "r");    
    // FILE *code_file = fopen(code_file_name, "w");
    // ON_DEBUG(FILE *logfile = fopen(logfile_name, "w"));

    // trans_commands_t trans_commands = {};
    // GetCommands(trans_file_name, &trans_commands);

    // cmd_t cmd = {};
    // cmd.ip = 0;
    // cmd.size = GetCountOfWords(asm_file);
    // cmd.code = (int *) calloc(cmd.size, sizeof(int));

    // marklist_t marklist = {};
    // marklist.size = cmd.size;
    // marklist.ip   = 0;
    // marklist.list = (mark_t *) calloc(marklist.size, sizeof(mark_t));

    // for (size_t i = 0; i < marklist.size; i++)
    // {
    //     // marklist.list[i].name   = {};
    //     marklist.list[i].address = POISON;
    // }

    // fixup_t fixup = {};
    // fixup.ip = 0;
    // fixup.size = cmd.size;      // максимум меток = количество команд
    // fixup.data = (fixup_el_t *) calloc(fixup.size, sizeof(fixup_el_t));
    
    // for (size_t i = 0; i < fixup.size; i++)
    // {
    //     fixup.data[i].num_in_marklist  = POISON;        
    //     fixup.data[i].mark_ip = POISON;
    // }


    char cur_command_name[COMMAND_NAME_LEN] = {};

    while (fscanf(compiler.asm_file, "%s", cur_command_name) == 1)
    {
        if (IsMark(cur_command_name))
        {
            char mark_name[MARK_NAME_LEN] = {};
            sscanf(cur_command_name, "%[^" MARK_SYMBOL "]", mark_name);            

            mark_t *mark = FindMarkInList(mark_name, &compiler.marklist);

            if (mark == NULL)
            {    
                size_t list_ip = compiler.marklist.ip;
                compiler.marklist.list[list_ip].address = compiler.cmd.ip;
                strncat(compiler.marklist.list[list_ip].name, mark_name, MARK_NAME_LEN - 1);
                // sscanf(cur_command_name, "%[^" MARK_SYMBOL "]", marklist.list[marklist.ip].name);
                compiler.marklist.ip++;
            }

            else if (mark != NULL && mark->address == POISON)
            {
                mark->address = compiler.cmd.ip;
            }

            else if(mark->address != POISON)
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

    // free(trans_commands.commands);
    // free(cmd.code);
// 
    // ON_DEBUG(fclose(logfile));
    // fclose(asm_file);
    // fclose(code_file);

    return 0;
}
