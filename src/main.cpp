#include <stdio.h>
#include <string.h>

#include "stack.h"

const int CMD_SIZE = 50;

void Run();

int main()
{
//gbgzj
}

void Run()
{
    StackID stk = {};
    STACK_CTOR(&stk, 0);

    while (true)
    {
        char cmd[CMD_SIZE] = {};

        scanf("%s", cmd);

        if (strcmp(cmd, "push") == 0)
        {
            int arg = 0;
            scanf("%d", &arg);
            StackPush(&stk, arg);
            continue;
        }
    }
}