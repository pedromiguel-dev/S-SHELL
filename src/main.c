#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAIN_SHELL_BUFSIZE 1024
#define MAIN_SHELL_TOKEN_BUFFSIZE 64
#define MAIN_SHELL_TOKEN_DELIM " \t\r\n\a\""

/**
 * sshel's built in functions
*/
int shell_main_cd(char **args);
int shell_main_help(char **args);
int shell_main_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int shell_main_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int (*builtin_func[]) (char **) = {
  &shell_main_cd,
  &shell_main_help,
  &shell_main_exit
};

int shell_main_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "sshell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0)
        {
            perror("sshell");
        }
    }
    return 1;
}

int shell_main_help(char **args)
{
  int i;
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < shell_main_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int shell_main_exit(char **args)
{
  return 0;
}

/**
 * sshel's inner workings
*/

char* shell_main_get_line () 
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

char** shell_main_split_line(char* line)
{
    enum { OUTSIDE, INSIDE } state = OUTSIDE;
    int buffer_size = MAIN_SHELL_TOKEN_BUFFSIZE, position = 0;
    char** tokens = malloc(buffer_size * sizeof(char*));
    char* token;

    if (tokens == NULL)
    {
        fprintf(stderr,"Unable to allocate for tokens");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, MAIN_SHELL_TOKEN_DELIM );
    while (token != NULL)
    {
        if (token != NULL && token[0] == '"') {
            // Token starts with a double quote, move to INSIDE state
            state = INSIDE;
        } else {
            tokens[position] = token;
            position++;
        }

        if (state == INSIDE && token != NULL && token[strlen(token) - 1] == '"') {
            // Token ends with a double quote, move back to OUTSIDE state
            tokens[position] = token;
            position++;
            state = OUTSIDE;
        }

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

int shell_main_launch(char** args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    { // child
        if ( execvp(args[0], args) == -1 )
        {
            perror("sshell exit child");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0)
    {
        perror("sshell fork failure");
    } else { // parent
        do {
            wpid = waitpid(pid, &status, WUNTRACED); 
        } while (WIFEXITED(status) == NULL && WIFSIGNALED(status == NULL)); // parent will wait for child while is not errored or exited
    }
    return 1;
}

int shell_main_execute(char** args)
{
    int i;

    if (args[0] == NULL)
    {
        return 1;
    }

    for (i = 0; i < shell_main_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
        
    }
    
    return shell_main_launch(args);
    
}

int shell_main_loop()
{
    char* line;
    char** args;
    int status = 1;

    do {
        printf("simple_shell: ");
        line = shell_main_get_line();
        args = shell_main_split_line(line);
        status = shell_main_execute (args);

        free(line);
        free(args);
    } while (status);

    return status;
}

int main(int argc, char *argv[])
{
    return shell_main_loop();
}