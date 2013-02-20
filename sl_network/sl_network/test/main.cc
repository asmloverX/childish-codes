#include <stdio.h>

#include "./sl_network_test/sl_network_test.h"
#include "./sl_temp_test/sl_temp_test.h"



static void 
__sl_network_help(void)
{
    fprintf(stdout, 
        "*************************************************\n"
        "   [0] - quit\n"
        "   [1] - help\n"
        "   [2] - sl_network\n"
        "   [3] - sl_temp\n"
        "*************************************************\n"
        );
}

int 
main(int arg, char* argv[], char* envp[])
{
    int sel = 0;

    fprintf(stdout, "welcome to Testing application, please type \'0\' to quit or \'1\' for help\n");
    while (true)
    {
        fprintf(stdout, "> ");
        fscanf(stdin, "%d", &sel);

        if (0 == sel)
            break;
        switch (sel)
        {
        case 1:
            __sl_network_help();
            break;
        case 2:
            sl_test::sl_network_test();
            break;
        case 3:
            sl_temp::sl_temp_test();
            break;
        default:
            fprintf(stdout, "you typed wrong selection, please type \'0\' to quit or \'1\' for help\n");
        }
        fprintf(stdout, "\n");
    }

    return 0;
}