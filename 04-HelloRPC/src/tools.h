#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 1050000

// externos a biblioteca tools
double wtime();

// Biblioteca p/ vetores
int writeline(char *msg, FILE *file);

char *readline(FILE *file);

int getMessageCount();

void iterateMessageCount();

char *readEntireFile(char *fileName);
