#include "spu.h"

extern StackElem_t RAM[];

void SpuCreateWindow()
{
    txCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y);

    txSetColor(TX_BLACK);
    txSetFillColor(TX_LIGHTBLUE);
}

void DrawInWindow()
{
    // fprintf(stderr, "NARISUY V OKNE!!\n");

    txSleep(SLEEP_TIME);
    
    txSetFillColor(TX_BLACK);
    txRectangle(0, 0, RAM_SIZE_X * PIXEL_SIZE, RAM_SIZE_Y * PIXEL_SIZE);
    txSetFillColor(TX_LIGHTBLUE);

    for (size_t pos_y = 0; pos_y < RAM_SIZE_Y; pos_y++)
    {
        for (size_t pos_x = 0; pos_x < RAM_SIZE_X; pos_x++)
        {
            size_t el_num = pos_y * RAM_SIZE_X + pos_x;
            assert(el_num < RAM_SIZE);

            if (RAM[el_num] != 0)
            {
                txSetFillColor(TX_LIGHTBLUE);

                double x0 = (double) pos_x * PIXEL_SIZE;
                double y0 = (double) pos_y * PIXEL_SIZE;

                double x1 = x0 + PIXEL_SIZE;
                double y1 = y0 + PIXEL_SIZE;
// fprintf(stderr, "%f %f %f %f\n", x0, x1, y0, y1)
                txRectangle(x0, y0, x1, y1);
            }
        }
    }
}

void DrawInConsole()
{   
    fprintf(stderr, "\tRAM  [%p]:\n\t{\t\t\t", RAM);

    for (size_t i = 0; i < RAM_SIZE; i++)
    {
        if (i % (int) RAM_SIZE_X == 0)
            fprintf(stderr, "\n\t\t");
        
        fprintf(stderr, "%3d ", RAM[i]); 
    }

    fprintf(stderr, "\n\t}\n");

    fprintf(stderr, "}\n\n\n");


    // for (size_t i = 0; i < RAM_SIZE; i++)
    // {
    //     if (i % (int) RAM_SIZE_X == 0)
    //         printf("\n\t\t");
        
    //     printf("%3d ", RAM[i]); 
    // }
    // printf("\n");
}