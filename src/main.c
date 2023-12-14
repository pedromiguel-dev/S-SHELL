#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAIN_SHELL_BUFSIZE 1024
#define MAIN_SHELL_TOKEN_BUFFSIZE 64

char* main_shell_get_line () 
{   
    int position = 0;
    int c;
    int buffer_size = MAIN_SHELL_BUFSIZE;
    char* buffer = malloc(sizeof(char)*buffer_size);

    if(buffer == NULL)
    {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // get a character
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        if (position >= buffer_size) {
            buffer_size += MAIN_SHELL_BUFSIZE;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return buffer;
}

char** main_shell_split_line(char* line)
{
    const char* MAIN_SHELL_TOKEN_DELIM = " \t\r\n\a";
    int buffer_size = MAIN_SHELL_TOKEN_BUFFSIZE, position = 0;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token;

    if (tokens == NULL)
    {
        fprintf(stderr,"Unable to allocate for tokens");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, MAIN_SHELL_TOKEN_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size)
        {
            buffer_size += MAIN_SHELL_TOKEN_BUFFSIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if (tokens = NULL)
            {
                fprintf(stderr,"Unable to grow token size");
                exit(EXIT_FAILURE);
            }
        }
        // when null it continues where it left
        token = strtok(NULL, MAIN_SHELL_TOKEN_DELIM);
    }   
    tokens[position] = NULL;
    return tokens;
}

// TODO: launch shell commands function
// TODO: execute function

void main_shell_loop()
{
    char* line;
    char** args;
    int status = 1;

    do {
        printf("simple_shell: ");
        line = main_shell_get_line();
        args = main_shell_split_line(line);

        printf(line);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char *argv[])
{
    main_shell_loop();
    return 0;
}