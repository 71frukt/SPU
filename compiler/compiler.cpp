#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "compiler.h"
#include "compiler_debug.h"

const char *trans_file_name = "txts/translator.txt";
const char *asm_file_name   = "txts/program.asm";
const char *code_file_name  = "txts/program_code.txt";
ON_DEBUG(const char *logfile_name = "txts/logs/compiler_logs.log");

static int CompilerError_val = 0;

// TODO: make errno and compiler_assert

void CompilerCtor(compiler_t *compiler)
{
    ON_DEBUG(FILE **logfile =  &compiler->logfile);
    FILE       **code_file  =  &compiler->code_file;
    FILE       **asm_file   =  &compiler->asm_file;

    trans_commands_t *trans_commands = &compiler->trans_commands;
    cmd_t            *cmd            = &compiler->cmd;
    fixup_t          *fixup          = &compiler->fixup;
    marklist_t       *marklist       = &compiler->marklist;

    *asm_file  = fopen(asm_file_name,  "r");
    *code_file = fopen(code_file_name, "w");
    ON_DEBUG(*logfile = fopen(logfile_name, "w"));

    GetCommands(trans_file_name, &compiler->trans_commands);

    cmd->ip = 0;
    cmd->size = GetCountOfWords(*asm_file);
    cmd->code = (int *)  calloc(cmd->size, sizeof(int));
    
    for (size_t i = 0; i < cmd->size; i++)
        cmd->code[i] = CMD_POISON;

    marklist->size = cmd->size;
    marklist->ip   = 0;
    marklist->list = (mark_t *) calloc(marklist->size, sizeof(mark_t));

    for (size_t i = 0; i < marklist->size; i++)
        marklist->list[i].address = MARK_POISON;

    fixup->ip = 0;
    fixup->size = cmd->size;      // максимум меток = количество команд
    fixup->data = (fixup_el_t *) calloc(fixup->size, sizeof(fixup_el_t));
    
    for (size_t i = 0; i < fixup->size; i++)
    {
        fixup->data[i].num_in_marklist  = MARK_POISON;        
        fixup->data[i].mark_ip = MARK_POISON;
    }

    COMPILER_ASSERT(compiler);
}

void CompilerDtor(compiler_t *compiler)
{
    COMPILER_ASSERT(compiler);  

    ON_DEBUG(FILE *logfile =  compiler->logfile);
    FILE       *code_file  =  compiler->code_file;
    FILE       *asm_file   =  compiler->asm_file;

    trans_commands_t *trans_commands = &compiler->trans_commands;
    cmd_t            *cmd            = &compiler->cmd;
    fixup_t          *fixup          = &compiler->fixup;
    marklist_t       *marklist       = &compiler->marklist;

    ON_DEBUG(fclose(logfile));
    fclose(code_file);
    fclose(asm_file);

    free(trans_commands->commands);
    free(cmd->code);
    free(fixup->data);
    free(marklist->list);

    *compiler = {};
}

void WriteCommandCode(char *cur_command_name, compiler_t *compiler)    
{
    assert(cur_command_name);
    COMPILER_ASSERT(compiler);

    FILE       *asm_file =  compiler->asm_file;
    cmd_t      *cmd      = &compiler->cmd;
    fixup_t    *fixup    = &compiler->fixup;
    marklist_t *marklist = &compiler->marklist;

    if (strstr("push pop", cur_command_name) != NULL)
    {
        int command_code = 0;

        if (strcmp(cur_command_name, "push") == 0)
            command_code |= PUSH;
        else
            command_code |= POP;
        
        int res_imm  = CMD_POISON;
        int res_reg  = CMD_POISON;

        char  arg_str[MAX_ARG_NAME_LEN] = {};
        char  tmp_str[MAX_ARG_NAME_LEN] = {};        
        char *cur_ptr = NULL;

        fscanf(asm_file, "%[^\n]", arg_str);

        cur_ptr = strchr(arg_str, '[');
        
        if (cur_ptr != NULL)
        {
            cur_ptr++;  //на следующий символ после '['
            command_code |= RAM_BIT;
        }
        else
            cur_ptr = arg_str + 1;      // первый символ всегда пробел


        if (sscanf(cur_ptr, "%[A-Z]", tmp_str) == 1)     // есть регистр
        {
            if (IsRegister(tmp_str))
            {
                command_code |= REG_BIT;
                int elem = ReadRegister(tmp_str);
                res_reg = elem;
            }
            
            else 
                fprintf(stderr, "ERROR: invalid register in push: '%s'\n", tmp_str);
        }

        char *ptr; 
        if ((ptr = strchr(cur_ptr, '+')) != NULL)
            cur_ptr = ptr + 1;

        if (sscanf(cur_ptr, "%d", &res_imm) == 1)    // есть immediate const
            command_code |= IMM_BIT;

        cmd->code[cmd->ip++] = command_code;

        fprintf(stderr, "command code = %d\n", command_code);

        if (res_reg != CMD_POISON)
            cmd->code[cmd->ip++] = res_reg;
        
        if (res_imm != CMD_POISON)
            cmd->code[cmd->ip++] = res_imm;
    }

    // else if (strcmp(cur_command_name, "PUSHR") == 0)
    // {
    //     cmd->code[cmd->ip++] = PUSHR;

    //     char reg_name[REG_NAME_LEN] = {};
    //     fscanf(asm_file, "%s", reg_name);

    //     int elem = ReadRegister(reg_name);

    //     cmd->code[cmd->ip++] = elem;
    // }

    // else if (strcmp(cur_command_name, "POP") == 0)
    // {
    //     cmd->code[cmd->ip++] = POP;

    //     char reg_name[REG_NAME_LEN] = {};
    //     fscanf(asm_file, "%s", reg_name);

    //     int elem = ReadRegister(reg_name);

    //     cmd->code[cmd->ip++] = elem;
    // }


    else if (strstr( "jump call JA JEA JB JEB JE JNE", cur_command_name) != NULL)
    {
        if(strcmp(cur_command_name, "call") == 0)
            cmd->code[cmd->ip++] = CALL;

        else if (strcmp(cur_command_name, "JUMP") == 0)
            cmd->code[cmd->ip++] = JUMP;

        else if (strcmp(cur_command_name, "JA") == 0)
            cmd->code[cmd->ip++] = JA;

        else if (strcmp(cur_command_name, "JAE") == 0)
            cmd->code[cmd->ip++] = JAE;

        else if (strcmp(cur_command_name, "JB") == 0)
            cmd->code[cmd->ip++] = JB;
        
        else if (strcmp(cur_command_name, "JBE") == 0)
            cmd->code[cmd->ip++] = JBE;

        else if (strcmp(cur_command_name, "JE") == 0)
            cmd->code[cmd->ip++] = JE;

        else if (strcmp(cur_command_name, "JNE") == 0)
            cmd->code[cmd->ip++] = JNE;


        char arg_str[MARK_NAME_LEN] = {};

        fscanf(asm_file, "%s", arg_str);

        if (IsMark(arg_str))
        {
            sscanf(arg_str, "%[^" MARK_SYMBOL "]", arg_str);   

            mark_t *mark = FindMarkInList(arg_str, marklist);

            if (mark == NULL)
            {
                mark = &marklist->list[marklist->ip++];
                mark->address = MARK_POISON;      //создать метку с ядовитым значением чтобы потом доопределить
                sscanf(arg_str, "%[^" MARK_SYMBOL "]", mark->name);
            }

            if (mark->address == MARK_POISON)
            {                
                fixup->data[fixup->ip].mark_ip         = cmd->ip;
                fixup->data[fixup->ip].num_in_marklist = marklist->ip - 1;

                fixup->ip++;
            }

            cmd->code[cmd->ip++] = (int) mark->address;
        }

        else
        {
            int elem = 0;

            if (sscanf(arg_str, "%d", &elem) == 1)
                cmd->code[cmd->ip++] = elem;
                
            else
                fprintf(stderr, "COMPILE ERROR: Invalid mark: '%s'\n", arg_str);
        }
    }

    else if (strcmp(cur_command_name, "RET") == 0)
        cmd->code[cmd->ip++] = RET;

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

    COMPILER_ASSERT(compiler);
}


int ReadRegister(char *reg_name)
{
    int elem = REGISTER_POISON;

    if (!IsRegister(reg_name))
    {
        fprintf(stderr, "ERROR: incorrect register: '%s'\n", reg_name);
        return REGISTER_POISON;
    }

    elem = reg_name[0] - 'A' + 1;       // AX - первый регистр

    return elem;
}

bool IsRegister(char *reg_name)
{
    return (strlen(reg_name) >= 2 && reg_name[1] == 'X' && reg_name[0] >= 'A' && reg_name[0] <= 'A' + REGISTERS_NUM);
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
    COMPILER_ASSERT(compiler);

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
        fscanf(trans_file, "%s",  commands_tmp[i].name);
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

void MakeFixUp(compiler_t *compiler)    //fixup_t *fixup, cmd_t *cmd, marklist_t *marklist
{
    COMPILER_ASSERT(compiler);

    cmd_t      *cmd      = &compiler->cmd;
    fixup_t    *fixup    = &compiler->fixup;
    marklist_t *marklist = &compiler->marklist;

    for (size_t i = 0; i < fixup->ip; i++)
    {
        fixup_el_t cur_fixup_el = fixup->data[i]; 
        
        cmd->code[cur_fixup_el.mark_ip] = (int) marklist->list[cur_fixup_el.num_in_marklist].address;
    }

    COMPILER_ASSERT(compiler);
}