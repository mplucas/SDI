#include <time.h>
#include <sys/time.h>
#include "tools.h"

// %%%%%%%%%%%%%%% FERRAMENTAS p/ VETORES %%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
double wtime()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + (double)t.tv_usec / 1000000;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int writeline(char *msg, FILE *file)
{
  fprintf(file, "%s", msg);
  return 0;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char *readline(FILE *file)
{
  int cont = 0;
  char *getLine = NULL;
  char c;
  int linha = 0;

  getLine = (char *)malloc(256 * sizeof(char));
  //fseek(file, cont, SEEK_SET);
  for (;; cont++)
  {
    c = fgetc(file);
    if ((linha == 256) || (c == EOF))
    {
      return (getLine);
    }
    getLine[linha++] = c;
  }
  return NULL;
}

int MESSAGE_COUNT = 0;

int getMessageCount()
{
  return MESSAGE_COUNT;
}

void iterateMessageCount()
{
  MESSAGE_COUNT++;
}

char *readEntireFile(char *fileName)
{

  char entireFile[MAX_FILE_SIZE] = "";
  FILE *fileread;
  fileread = fopen(fileName, "r");
  if (fileread == NULL)
  {
    printf("Error: Na abertura dos arquivos (%s).", fileName);
    exit(1);
  }

  char c = fgetc(fileread);
  int i = 0;
  while (c != EOF)
  {
    entireFile[i] = c;
    i++;
    c = fgetc(fileread);
  }
  fclose(fileread);

  char *p_entireFile = entireFile;
  return p_entireFile;
}

void saveContentInFile(char *content, char *fileName)
{
  FILE *filewrite;
  filewrite = fopen(fileName, "w");
  if (filewrite == NULL)
  {
    printf("Error: Na abertura do arquivo (%s).", fileName);
    exit(1);
  }
  writeline(content, filewrite);
  fclose(filewrite);
}

char *substr(char str[], int start, int end)
{
  int i, j;
  char *sub;

  // Verifica valores incompatíveis e
  // retorna NULL
  if (start >= end || end > strlen(str))
  {
    return NULL;
  }

  // Aloca memória para a substring
  sub = (char *)malloc(sizeof(char) * (end - start + 1));

  // Copia a substring para a variável
  for (i = start, j = 0; i < end; i++, j++)
  {
    sub[j] = str[i];
  }

  // Terminador de string
  sub[j] = '\0';

  return sub;
}