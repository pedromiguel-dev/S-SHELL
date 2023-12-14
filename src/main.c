#include <stdio.h>
#include <stdlib.h>

char* main_shell_get_line () 
{   
    unsigned long size = sizeof(char)*200;
    char* input = (char*)malloc(size);

    if(input == NULL)
    {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    if (fgets(input, size, stdin) == NULL)
    {
        free(input);
        return NULL;
    }

    return input;
}

void main_shell_loop()
{
    char* line;
    char** args;
    int status = 1;

    do {
        printf("simple_shell: ");
        line = main_shell_get_line();

        printf(line);

        free(args);
        free(line);
    } while (status);
}

int main(int argc, char *argv[])
{
    main_shell_loop();
    return 0;
}