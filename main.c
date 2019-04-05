#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

void passToDir(char* fileName, char* desiredDir) {
    long pos;
    int byte;
    char* filename = calloc(4 + strlen(fileName), sizeof(char));
    strcpy(filename, "./");
    strcat(filename, fileName);
    FILE* original = fopen(filename, "r");
    if (original == NULL) printf("file %s not loaded!\n", fileName);

    struct stat info;
    int e = stat(desiredDir, &info);
    char* dirname = calloc(4 + strlen(desiredDir), sizeof(char));
    strcpy(dirname, "./");
    strcat(dirname, desiredDir);
    if ((e != 0) && (errno = ENOENT)) {
        printf("folder %s created!\n", dirname);
        mkdir(dirname, S_IRWXU); //S_IRWXG
    }

    char* copyname = calloc(4 + strlen(dirname) + strlen(fileName), sizeof(char));
    strcpy(copyname, dirname);
    strcat(copyname, "/");
    strcat(copyname, fileName);
    FILE* copy = fopen(copyname, "w");

    fseek(original, 0L, SEEK_END);
    pos = ftell(original);
    fseek(original, 0L, SEEK_SET);
    while (pos--) {
        byte = fgetc(original);
        fputc(byte, copy);
    }

    fclose(original);
    fclose(copy);

    if (remove(filename) == 0) {
        printf("file %s deleted!\n", filename);
    }
}

void sortByFirstLetter(char* fileName) {
    int wasLetter = 0;
    for (int i = 0; i < strlen(fileName); ++i) {
        if (isalpha(fileName[i])) {
            wasLetter = 1;
            char *point = calloc(4, sizeof(char));
            point[0] = fileName[i];
            passToDir(fileName, point);
            return;
        }
    }
    if (!wasLetter) {
        printf("no letter in file name - file skipped!\n");
    }
}

void sortByFirstLetterAndNumber(char* fileName) {
    int wasLetter = 0;
    for (int i = 0; i < strlen(fileName); ++i) {
        if (isalpha(fileName[i])) {
            wasLetter = 1;
            char *point = calloc(4, sizeof(char));
            point[0] = fileName[i];
            passToDir(fileName, point);
        }
    }
    if (!wasLetter) {
        printf("no letter in file name - file skipped!\n");
    }
}

int main() {
    printf("for sorting by first letter press 1\n");
    printf("for sorting by first letter and number press 2\n");
    printf("for deleting files by char sequence press 3\n");
    printf("\n");

    int operation;
    scanf("%i", &operation);

    DIR *dir = opendir(".");
    if (dir) {
        struct  dirent *de = readdir(dir);
        while (de) {
            if ((strcmp(de->d_name, "main.c") != 0) && (strcmp(de->d_name, "CMakeLists.txt") != 0)) {
                if (de->d_type != DT_DIR) {
                    printf("file %s passed...\n", de->d_name);
                    if (operation == 1) {
                        sortByFirstLetter(de->d_name);
                    } else if (operation == 2) {

                    } else if (operation == 3) {

                    } else {
                        printf("no command, just looking...\n");
                    }
                }
            }
            de = readdir(dir);
        }
        closedir(dir);
    }
    return 0;
}
