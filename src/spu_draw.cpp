#include "spu.h"

extern StackElem_t RAM[];

void SpuCreateWindow()
{
    txCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y);

    txSetColor(TX_BLACK);
    txSetFillColor(TX_LIGHTBLUE);
}

void DrawInWindow(spu_t *spu)
{
    SPU_ASSERT(spu);
    fprintf(stderr, "NARISUY V OKNE!!\n");

    txSleep(100);

    for (size_t pos_y = 0; pos_y < RAM_SIZE_Y; pos_y++)
    {
        for (size_t pos_x = 0; pos_x < RAM_SIZE_X; pos_x++)
        {
            size_t el_num = pos_y * RAM_SIZE_X + pos_x;
            assert(el_num < RAM_SIZE);

            if (RAM[el_num] != 0)
            {
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

void DrawInConsole(spu_t *spu)
{
    SPU_ASSERT(spu);

    fprintf(stderr, "NARISUY V KONSOLI!!\n");

    for (size_t pos_y = 0; pos_y < RAM_SIZE_Y; pos_y++)
    {
        for (size_t pos_x = 0; pos_x < RAM_SIZE_X; pos_x++)
        {
            size_t el_num = pos_y * RAM_SIZE_X + pos_x;
            assert(el_num < RAM_SIZE);

            if (RAM[el_num] != 0)
                printf("##");
            else
                printf("**");
        }

        printf("\n");
    }

    SPU_ASSERT(spu);
}