#include "all_magic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE_INCREMENT 1024

extern int file_main( int argc, char *argv[] );
int run_file(char **buff, int *buff_size, int *len, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int ret = EXIT_FAILURE;

    int buff_size = 1024;
    char *buff = malloc(buff_size);
    if (NULL == buff) {
        return ret;
    }

    int len = 0;                /* data length in buffer */
    memset(buff, 0, buff_size);
    ret = run_file(&buff, &buff_size, &len, argc, argv);
    free(buff);

    /* TODO: deal with the output of file */
    printf("buff_size=%d,len=%d\n%s", buff_size, len, buff);

    return ret;
}

int run_file(char **buff, int *buff_size, int *len, int argc, char *argv[])
{
    int fd[2];

    /* extract all_magic to MAGIC directory */
    if (0 == access(MAGIC, 0)) {
        printf("Magic file:"MAGIC" already exists. Ignore creation.\n");
    }
    else {
        FILE *magic_fd = fopen(MAGIC, "w");
        if (NULL == magic_fd) {
            printf("Failed to create magic file:"MAGIC"\n");
            return EXIT_FAILURE;
        }

        fwrite(all_magic, all_magic_len, 1, magic_fd);
        fclose(magic_fd);

        printf("Magic file:"MAGIC" created.\n");
    }

    /* fd[0] is set up for reading,  fd[1] is set up for writing */
    if (pipe(fd)) {
        fprintf(stderr, "Pipe failed!\n");
        return EXIT_FAILURE;
    }

    /* backup stdout */
    int stdout_fd = dup(STDOUT_FILENO);

    /* create child process to call file_main */
    pid_t pid = fork();
    if (pid <  (pid_t) 0) { /* fork failed */
        fprintf(stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    } else if (pid == (pid_t) 0) { /* the child process */
        /* redirect stdout to fd[1] and close the other end first */
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);

        /* call file_main */
        file_main(argc, argv);

        /* close restore stdout */
        fclose(stdout);
        dup2(stdout_fd, STDOUT_FILENO);

        return EXIT_SUCCESS;
    }
    else { 
        /* close the other end first */
        close(fd[1]);

        /* read from child process */
        FILE *stream = fdopen(fd[0], "r");
        if (NULL == stream) {
            return EXIT_FAILURE;
        }

        int c;
        while(EOF != (c = fgetc(stream))) {
            if ((*len) + 1 >= (*buff_size)) {
                void *new_buff = realloc(*buff, (*buff_size) + BUFF_SIZE_INCREMENT);
                if (NULL == new_buff) {
                    printf("Failed in realloc!\n");
                    break;
                }

                (*buff) = new_buff;
                (*buff_size) = (*buff_size) + BUFF_SIZE_INCREMENT;
            }

            (*buff)[*len] = c;
            (*len) = (*len) + 1;
        }

        fclose(stream);
    }

    return EXIT_SUCCESS;
}
