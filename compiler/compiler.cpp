#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "compiler_debug.h"

#define MARK_SYMBOL ":"

const char *trans_file_name = "txts/translator.txt";
const char *asm_file_name   = "txts/program.asm";
const char *code_file_name  = "txts/program_code.txt";
const char *logfile_name    = "txts/logs/compiler_logs.txt";


int main()
{
    FILE *asm_file  = fopen(asm_file_name,  "r");    
    FILE *code_file = fopen(code_file_name, "w");
    ON_DEBUG(FILE *logfile = fopen(logfile_name, "w"));

    trans_commands_t trans_commands = {};
    GetCommands(trans_file_name, &trans_commands);

    cmd_t cmd = {};
    cmd.ip = 0;
    cmd.size = GetCountOfWords(asm_file);
    cmd.code = (int *) calloc(cmd.size, sizeof(int));

    marklist_t marklist = {};
    marklist.size = cmd.size;
    marklist.ip   = 0;
    marklist.list = (mark_t *) calloc(marklist.size, sizeof(mark_t));

    for (size_t i = 0; i < marklist.size; i++)
    {
        // marklist.list[i].name   = {};
        marklist.list[i].address = POISON;
    }

    char cur_command_name[COMMAND_NAME_LEN] = {};

    while (fscanf(asm_file, "%s", cur_command_name) == 1)
    {
        if (IsMark(cur_command_name))
        {
            marklist.list[marklist.ip++].address = cmd.ip;
            sscanf(cur_command_name, "%[^" MARK_SYMBOL "]", marklist.list->name);
            
            continue;
        }

        WriteCommandCode(cur_command_name, &marklist, asm_file, &cmd);

        COMPILER_DUMP(logfile, &cmd, &marklist);
    }
    
    PrintCMD(&cmd, code_file);

    COMPILER_DUMP(logfile, &cmd, &marklist);

    free(trans_commands.commands);
    free(cmd.code);

    ON_DEBUG(fclose(logfile));
    fclose(asm_file);
    fclose(code_file);

    return 0;
}

void WriteCommandCode(char *cur_command_name, marklist_t *mark_list, FILE *asm_file, cmd_t *cmd)    
{
    if (strcmp(cur_command_name, "push") == 0)
    {
        cmd->code[cmd->ip++] = PUSH;

        int elem = POISON;
        fscanf(asm_file, "%d", &elem);
        cmd->code[cmd->ip++] = elem;
    }

    else if (strcmp(cur_command_name, "PUSHR") == 0)
    {
        cmd->code[cmd->ip++] = PUSHR;

        char reg_name[REG_NAME_LEN] = {};
        fscanf(asm_file, "%s", reg_name);

        int elem = ReadRegister(reg_name);

        cmd->code[cmd->ip++] = elem;
    }

    else if (strcmp(cur_command_name, "POPR") == 0)
    {
        cmd->code[cmd->ip++] = POPR;

        char reg_name[REG_NAME_LEN] = {};
        fscanf(asm_file, "%s", reg_name);

        int elem = ReadRegister(reg_name);

        cmd->code[cmd->ip++] = elem;
    }

    else if (strcmp(cur_command_name, "JUMP") == 0)
    {
        cmd->code[cmd->ip++] = JUMP;

        char arg_str[MARK_NAME_LEN] = {};

        fscanf(asm_file, "%s", arg_str);

        if (IsMark(arg_str))
        {
            sscanf(arg_str, "%[^" MARK_SYMBOL "]", arg_str);    // TODO: mark verify

            mark_t *mark = FindMarkInList(arg_str, mark_list);

            if (mark == NULL)
            {
                fprintf(stderr, "ZALUPA\n");
                return;
            }
            cmd->code[cmd->ip++] = (int) mark->address;

            return;
        }

        else
        {
            int elem = POISON;
            sscanf(arg_str, "%d", &elem);
            cmd->code[cmd->ip++] = elem;
        }
    }

    else if (strcmp(cur_command_name, "add") == 0)
        cmd->code[cmd->ip++] = ADD;
    
    else if (strcmp(cur_command_name, "sub") == 0)
        cmd->code[cmd->ip++] = SUB;

    else if (strcmp(cur_command_name, "mul") == 0)
        cmd->code[cmd->ip++] = MUL;

    else if (strcmp(cur_command_name, "div") == 0)
        cmd->code[cmd->ip++] = DIV;

    else if (strcmp(cur_command_name, "out") == 0)
        cmd->code[cmd->ip++] = OUT;

    else if (strcmp(cur_command_name, "hlt") == 0)
        cmd->code[cmd->ip++] = HLT;

    else
        fprintf(stderr, "COMPILE ERROR: Unknown command: '%s'\n", cur_command_name);
}

/*
bool WriteCommandCode(char *cur_command_name, FILE *asm_file, trans_commands_t *trans_commands, cmd_t *cmd)    
{
    bool command_is_founded = false;

    for (size_t i = 0; i < trans_commands->size; i++) // TODO: multiple if
    {
        if (strcmp(trans_commands->commands[i].name, cur_command_name) == 0)
        {
            command_is_founded = true;

            cmd->code[cmd->ip++] = trans_commands->commands[i].key;

            if (strcmp(cur_command_name, "push") == 0 || strcmp(cur_command_name, "JUMP") == 0)
            {
                int elem = POISON;
                fscanf(asm_file, "%d", &elem);
                cmd->code[cmd->ip++] = elem;
            }   

            if (strcmp(cur_command_name, "PUSHR") == 0 || strcmp(cur_command_name, "POPR") == 0)
            {
                char reg_name[REG_NAME_LEN] = {};
                fscanf(asm_file, "%s", reg_name);

                int elem = ReadRegister(reg_name);

                cmd->code[cmd->ip++] = elem;
            }  

            break;
        }     
    }

    return command_is_founded;
}
*/

int ReadRegister(char *reg_name)
{
    int elem = POISON;

    if (reg_name[1] != 'X' || reg_name[0] < 'A' || reg_name[0] > 'A' + REGISTERS_NUM)
    {
        fprintf(stderr, "ERROR: incorrect register: '%s'\n", reg_name);
        return POISON;
    }

    elem = reg_name[0] - 'A' + 1;       // AX - первый регистр

    return elem;
}

size_t GetCountOfLines(FILE *text)
{
    size_t num_lines = 0;

    char cur_ch = 0;
    while ((cur_ch = (char) getc(text)) != EOF)
    {
        if (cur_ch == '\n')
        num_lines++;
    }

    num_lines++;    //in the end of file is no \n
    
    fseek(text, 0, SEEK_SET);
    return num_lines;
}

size_t GetCountOfWords(FILE *text)
{
    size_t num_words = 0;

    char cur_word[COMMAND_NAME_LEN] = {};

    while (fscanf(text, "%s", cur_word) == 1)
        num_words++;

    fseek(text, 0, SEEK_SET);
    return num_words;
}

void PrintCMD(cmd_t *cmd, FILE *file)
{
    for (size_t i = 0; i < cmd->size; i++)
        fprintf(file, "%d ", cmd->code[i]);
}

void GetCommands(const char *file_name, trans_commands_t *trans_commands)
{
    FILE *trans_file = fopen(file_name, "r");

    size_t num_trans_lines = GetCountOfLines(trans_file);

    trans_commands->size = num_trans_lines;

    command_t *commands_tmp = (command_t *) calloc(num_trans_lines, sizeof(command_t));

    for (size_t i = 0; i < num_trans_lines; i++)
    {
        fscanf(trans_file, "%s", commands_tmp[i].name);
        fscanf(trans_file, "%d", &commands_tmp[i].key);
    }

    trans_commands->commands = commands_tmp;

    fclose(trans_file);
}

bool IsMark(char *str)
{
    return (strstr(str, MARK_SYMBOL) ? true : false);
}

void MarkVerify(char *mark)
{
    if (strlen(mark) == 0)
        fprintf(stderr, "ERROR: invalid mark: '%s'\n", mark);
}

mark_t *FindMarkInList(char *mark_name, marklist_t *marklist)
{
    for (size_t i = 0; i < marklist->ip; i++)
    {
        if (strcmp(mark_name, marklist->list[i].name) == 0)
            return &(marklist->list[i]);
    }

    return NULL;
}