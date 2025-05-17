#include <stdio.h>
#include <string.h>

#include "compiler.h"
#include "compiler_debug.h"

int main(const int argc, const char *argv[])
{
    fprintf(stderr, "START COMPILER\n\n");
   
    compiler_t compiler = {};

    CompilerCtor(&compiler, argv[1]);

    Compile(&compiler);

    MakeFixUp(&compiler);

    PrintCMD(&compiler);

    COMPILER_DUMP(&compiler);

    CompilerDtor(&compiler);

    fprintf(stderr, "END COMPILER\n\n");

    return 0;
}
