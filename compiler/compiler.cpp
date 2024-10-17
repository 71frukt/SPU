#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "compiler_debug.h"

const char *trans_file_name = "txts/translator.txt";
const char *asm_file_name   = "txts/program.asm";
const char *code_file_name  = "txts/program_code.txt";
const char *logfile_name    = "txts/logs/compiler_logs.txt";

// TODO: make errno and compiler_assert

void CompilerCtor(compiler_t *compiler)
{
    compiler->asm_file  = fopen(asm_file_name,  "r");
    compiler->code_file = fopen(code_file_name, "w");
    ON_DEBUG(compiler->logfile = fopen(logfile_name, "w"));

    GetCommands(trans_file_name, &compiler->trans_commands);

    compiler->cmd.ip = 0;
    compiler->cmd.size = GetCountOfWords(compiler->asm_file);
    compiler->cmd.code = (int *)  calloc(compiler->cmd.size, sizeof(int));

    compiler->marklist.size = compiler->cmd.size;
    compiler->marklist.ip   = 0;
    compiler->marklist.list = (mark_t *) calloc(compiler->marklist.size, sizeof(mark_t));

    for (size_t i = 0; i < compiler->marklist.size; i++)
    {
        // marklist.list[i].name   = {};
        compiler->marklist.list[i].address = POISON;
    }

    compiler->fixup.ip = 0;
    compiler->fixup.size = compiler->cmd.size;      // максимум меток = количество команд
    compiler->fixup.data = (fixup_el_t *) calloc(compiler->fixup.size, sizeof(fixup_el_t));
    
    for (size_t i = 0; i < compiler->fixup.size; i++)
    {
        compiler->fixup.data[i].num_in_marklist  = POISON;        
        compiler->fixup.data[i].mark_ip = POISON;
    }
}

void WriteCommandCode(char *cur_command_name, compiler_t *compiler)    
{
    FILE       *asm_file =  compiler->asm_file;
    cmd_t      *cmd      = &compiler->cmd;
    fixup_t    *fixup    = &compiler->fixup;
    marklist_t *marklist = &compiler->marklist;

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
            sscanf(arg_str, "%[^" MARK_SYMBOL "]", arg_str);   

            mark_t *mark = FindMarkInList(arg_str, marklist);

            if (mark == NULL)
            {
                mark = &marklist->list[marklist->ip++];
                mark->address = POISON;      //создать метку с ядовитым значением чтобы потом доопределить
                sscanf(arg_str, "%[^" MARK_SYMBOL "]", mark->name);
            }

            if (mark->address == POISON)
            {                
                fixup->data[fixup->ip].mark_ip         = cmd->ip;
                fixup->data[fixup->ip].num_in_marklist = marklist->ip - 1;

                fixup->ip++;
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

void PrintCMD(compiler_t *compiler)
{
    for (size_t i = 0; i < compiler->cmd.ip; i++)
        fprintf(compiler->code_file, "%d ", compiler->cmd.code[i]);
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

mark_t *FindMarkInList(char *mark_name, marklist_t *marklist)
{
    for (size_t i = 0; i < marklist->ip; i++)
    {
        if (strcmp(mark_name, marklist->list[i].name) == 0)
            return &(marklist->list[i]);
    }

    return NULL;
}

/*
fixup_el_t *FindMarkInFixup(char mark_name, fixup_t *fixup)
{
    for (size_t i = 0; i < fixup->ip; i++)
    {
        if ()
    }
}
*/

void MakeFixUp(compiler_t *compiler)    //fixup_t *fixup, cmd_t *cmd, marklist_t *marklist
{
    cmd_t      *cmd      = &compiler->cmd;
    fixup_t    *fixup    = &compiler->fixup;
    marklist_t *marklist = &compiler->marklist;

    for (size_t i = 0; i < fixup->ip; i++)
    {
        fixup_el_t cur_fixup_el = fixup->data[i]; 
        
        cmd->code[cur_fixup_el.mark_ip] = (int) marklist->list[cur_fixup_el.num_in_marklist].address;
    }
}