#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int COMAND_NAME_LEN = 20;
const char *trans_file_name = "txts/translator.txt";
const char *asm_file_name   = "txts/program.asm";
const char *code_file_name  = "txts/program_code.txt";

size_t GetCountOfLines(FILE *text);

typedef struct Comand
{
    char name[COMAND_NAME_LEN];
    int code;
} comand_t;

typedef struct TranstationComands
{
    comand_t *comands;
    size_t size;
} trans_comands_t;

void GetComands(const char *file_name, trans_comands_t *trans_comands);

int main()
{
    trans_comands_t trans_comands = {};
    GetComands(trans_file_name, &trans_comands);

    FILE *asm_file  = fopen(asm_file_name,  "r");    
    FILE *code_file = fopen(code_file_name, "w");

    char cur_comand_name[COMAND_NAME_LEN] = {};

    while (fscanf(asm_file, "%s", cur_comand_name) == 1)
    {
        bool code_is_founded = false;

        for (size_t i = 0; i < trans_comands.size; i++)
        {
            if (strcmp(trans_comands.comands[i].name, cur_comand_name) == 0)
            {
                fprintf(code_file, "%d ", trans_comands.comands[i].code);

                if (strcmp(cur_comand_name, "push") == 0)
                {
                    int elem = 0;
                    fscanf(asm_file, "%d", &elem);
                    fprintf(code_file, "%d ", elem);
                }   

                code_is_founded = true;
                break;
            }     
        }

        if (!code_is_founded)
            fprintf(stderr, "Unknown comand: '%s'\n", cur_comand_name);
    }
    
    fclose(asm_file);
    fclose(code_file);
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

void GetComands(const char *file_name, trans_comands_t *trans_comands)
{
    FILE *trans_file = fopen(file_name, "r");

    size_t num_trans_lines = GetCountOfLines(trans_file);

    trans_comands->size = num_trans_lines;

    comand_t *comands_tmp = (comand_t *) calloc(num_trans_lines, sizeof(comand_t));

    for (size_t i = 0; i < num_trans_lines; i++)
    {
        fscanf(trans_file, "%s", comands_tmp[i].name);
        fscanf(trans_file, "%d", &comands_tmp[i].code);
    }

    trans_comands->comands = comands_tmp;

    fclose(trans_file);
}