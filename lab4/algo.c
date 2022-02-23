#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "algo.h"

int is_number(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

void list_processes() {
    DIR *d;
    struct dirent *dir;

    d = opendir("/proc");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (is_number(dir->d_name)) {
                char* filename = strcat3("/proc/", dir->d_name, "/comm");
                FILE* f = fopen(filename, "r");
                free(filename);
                if (f == NULL) {
                    printf("Error opening process info\n");
                    return;
                }

                char* comm = malloc(sizeof(char) * 32);
                fscanf(f, "%s", comm);
                fclose(f);
                printf("%s\t%s\n", dir->d_name, comm);
            }
        }
        closedir(d);
    }
}

void process_info(char pid[]) {
    char *filename = strcat3("/proc/", pid, "/status");
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        printf("Process with provided PID not found");
        return;
    }
    
    char* line = malloc(sizeof(char) * 255);
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, f)) != -1) {
        printf("%s", line);
    }

    fclose(f);
    if (line)
        free(line);
}

char* strcat3(char* a, char* b, char* c) {
    char* temp = malloc(sizeof(char) * 255);
    strcpy(temp, a);
    strcat(temp, b);
    strcat(temp, c);
    return temp;
}