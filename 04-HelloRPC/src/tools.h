#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 1050000
#define MAX_FILE_NAME_LENGTH 200

// externos a biblioteca tools
double wtime();

// Biblioteca p/ vetores
int writeline(char *msg, FILE *file);

char *readline(FILE *file);

int getMessageCount();

void iterateMessageCount();

char *readEntireFile(char *fileName);

void saveMessageInFile(char *nickName, char *content, char *fileName);

char *substr(char str[], int start, int end);