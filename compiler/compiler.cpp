#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_FUNC_NAME = 20;
const char *trans_file_name = "txts/translator.txt";
const char *asm_file_name   = "txts/program.asm";
const char *code_file_name  = "txts/program_code.txt";

size_t GetCountOfLines(FILE *text);

int main()
{
    FILE *trans_file = fopen(trans_file_name, "r");

    size_t num_trans_lines = GetCountOfLines(trans_file);

    char *func_names = (char *) calloc(num_trans_lines * MAX_FUNC_NAME, sizeof(char));
    int  *func_codes = (int *)  calloc(num_trans_lines, sizeof(int));

    for (size_t i = 0; i < num_trans_lines; i++)
    {
        fscanf(trans_file, "%s", func_names + i * MAX_FUNC_NAME);
        fscanf(trans_file, "%d", func_codes + i);
    }

    fclose(trans_file);

    FILE *asm_file  = fopen(asm_file_name,  "r");    
    FILE *code_file = fopen(code_file_name, "w");

    char cur_func_name[MAX_FUNC_NAME] = {};

    while (fscanf(asm_file, "%s", cur_func_name) == 1)
    {
        for (size_t i = 0; i < num_trans_lines; i++)
        {
            if (strcmp(func_names + i * MAX_FUNC_NAME, cur_func_name) == 0)
            {
                fprintf(code_file, "%d ", func_codes[i]);
                if (strcmp(cur_func_name, "push") == 0)
                {
                    int elem = 0;
                    fscanf(asm_file, "%d", &elem);
                    fprintf(code_file, "%d ", elem);
                }   

                break;
            }
        }
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