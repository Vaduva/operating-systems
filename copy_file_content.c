/**
 * Copies current script's binary content:
 * - digits to given file (first argument)
 * - letters to given file (second argument)
 * - other characters to given file (third argument) and to screen
 * Additionally, outputs on screen data on the four files
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    int self, f1, f2, f3;
    
    if (argc != 4) {
        printf("Not enough arguments");
        exit(1);    
    }
    
    self = open(argv[0], O_RDONLY);
    if (self == -1) {
       printf("Executable file not valid");
       exit(1);
    }
    
    f1 = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
    if (f1 == -1) {
        printf("f1 file not valid");
        exit(1);
    }
        
    f2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
    if (f2 == -1) {
        printf("f2 file not valid");
        exit(1);
    }
        
    f3 = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);
    if (f3 == -1) {
        printf("f3 file not valid");
        exit(1);
    }
    
    char byte;
    while (read(self, &byte, 1)) {
        if (isdigit(byte)) {
            if (write(f1, &byte, 1) == -1) {
                printf("Could not write in f1");
                exit(1);
            }
        } else if (isalpha(byte)) {
            if (write(f2, &byte, 1) == -1) {
                printf("Could not write in f2");
                exit(1);
            }   
        } else {
            if (write(f3, &byte, 1) == -1) {
                printf("Could not write in f3");
                exit(1);
            }
            write(1, &byte, 1);
        }
    }
    
    int i;
    struct stat *s = malloc(sizeof(struct stat));
    for (i = 0; i < argc; i++) {
        stat(argv[i], s);
        printf("\nSize of %s is %lldB", argv[i], s->st_size);
        printf("\nNumber of blocks for %s is %lldB", argv[i], s->st_blocks);
        printf("\nBlock size of %s is %dB", argv[i], s->st_blksize);
    }
    
    close(self);
    close(f1);
    close(f2);
    close(f3);
    free(s);
    
    return 0;
}
