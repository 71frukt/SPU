#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int   COMAND_NAME_LEN = 20;
const int   REG_NAME_LEN    = 2;
const int   REGISTERS_NUM   = 5;
const int   POISON          = 0xDEB41C;
const char *trans_file_name = "txts/translator.txt";
const char *asm_file_name   = "txts/program.asm";
const char *code_file_name  = "txts/program_code.txt";

typedef struct Comand
{
    char name[COMAND_NAME_LEN];
    int key;
} comand_t;

typedef struct TranstationComands
{
    comand_t *comands;
    size_t size;
} trans_comands_t;

typedef struct CMD
{
    size_t pi;
    size_t size;
    int *code;
} cmd_t;

void   GetComands      (const char *file_name, trans_comands_t *trans_comands);
size_t GetCountOfLines (FILE *text);
size_t GetCountOfWords (FILE *text);
int    ReadRegister    (char *reg_name);
void   PrintCMD        (cmd_t *cmd, FILE *file);


int main()
{
    trans_comands_t trans_comands = {};
    GetComands(trans_file_name, &trans_comands);

    FILE *asm_file  = fopen(asm_file_name,  "r");    
    FILE *code_file = fopen(code_file_name, "w");

    cmd_t cmd = {};
    cmd.pi = 0;
    cmd.size = GetCountOfWords(asm_file);
    cmd.code = (int *) calloc(cmd.size, sizeof(int));

    char cur_comand_name[COMAND_NAME_LEN] = {};

    while (fscanf(asm_file, "%s", cur_comand_name) == 1)
    {
        bool code_is_founded = false;

        for (size_t i = 0; i < trans_comands.size; i++)
        {
            if (strcmp(trans_comands.comands[i].name, cur_comand_name) == 0)
            {
                code_is_founded = true;

                cmd.code[cmd.pi++] = trans_comands.comands[i].key;

                if (strcmp(cur_comand_name, "push") == 0 || strcmp(cur_comand_name, "JUMP") == 0)
                {
                    int elem = POISON;
                    fscanf(asm_file, "%d", &elem);
                    cmd.code[cmd.pi++] = elem;
                }   

                if (strcmp(cur_comand_name, "PUSHR") == 0 || strcmp(cur_comand_name, "POPR") == 0)
                {
                    char reg_name[REG_NAME_LEN] = {};
                    fscanf(asm_file, "%s", reg_name);

                    int elem = ReadRegister(reg_name);

                    cmd.code[cmd.pi++] = elem;
                }  

                break;
            }     
        }

        if (!code_is_founded)
            fprintf(stderr, "Unknown comand: '%s'\n", cur_comand_name);
    }
    
    PrintCMD(&cmd, code_file);

    fclose(asm_file);
    fclose(code_file);

    return 0;
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

    char cur_word[COMAND_NAME_LEN] = {};

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

void GetComands(const char *file_name, trans_comands_t *trans_comands)
{
    FILE *trans_file = fopen(file_name, "r");

    size_t num_trans_lines = GetCountOfLines(trans_file);

    trans_comands->size = num_trans_lines;

    comand_t *comands_tmp = (comand_t *) calloc(num_trans_lines, sizeof(comand_t));

    for (size_t i = 0; i < num_trans_lines; i++)
    {
        fscanf(trans_file, "%s", comands_tmp[i].name);
        fscanf(trans_file, "%d", &comands_tmp[i].key);
    }

    trans_comands->comands = comands_tmp;

    fclose(trans_file);
}