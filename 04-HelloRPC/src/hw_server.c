#include <rpc/rpc.h>
#include <unistd.h>
#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char **func0_1_svc(void *a, struct svc_req *req)
{
     static char msg[256];
     static char *p;

     printf("FUNC0 (sem parâmetros)\n");
     strcpy(msg, "Hello Func0!");
     p = msg;

     return (&p);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func1_1_svc(char **a, struct svc_req *req)
{
     static int ret = 1;

     printf("FUNC1 (%s)\n", *a);

     return (&ret);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func2_1_svc(int *a, struct svc_req *req)
{
     static int ret = 10;

     printf("FUNC2 (%d)\n", *a);
     return (&ret);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func3_1_svc(struct param *a, struct svc_req *req)
{
     static int ret = 0;

     printf("FUNC3 (%d/%d)\n", a->arg1, a->arg2);
     ret = a->arg1 * a->arg2;
     return (&ret);
}

#define SAVED_FILE_NAMES "savedFileNames"

void saveFileName(char *fileName)
{
     FILE *filewrite;
     filewrite = fopen(SAVED_FILE_NAMES, "a+");
     if (filewrite == NULL)
     {
          printf("Error: Na abertura do arquivo (%s).", SAVED_FILE_NAMES);
          exit(1);
     }

     strcat(fileName, ";");
     writeline(fileName, filewrite);
     fclose(filewrite);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *sendchat_1_svc(struct msg *a, struct svc_req *req)
{
     printf("\nRecebido mensagem de %s\n", a->nickname);

     // finding proper name
     char fileName[MAX_FILE_NAME_LENGTH] = "";
     char strIndex[10] = "";
     int msgCount = getMessageCount();

     sprintf(strIndex, "%d", msgCount);
     strcat(fileName, a->nickname);
     strcat(fileName, "-");
     strcat(fileName, strIndex);
     strcat(fileName, ".serv");

     saveMessageInFile(a->nickname, a->content, fileName);

     printf("Salvo content em %s\n", fileName);
     saveFileName(fileName);

     static int ret = 1;
     return (&ret);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *getmsgindex_1_svc(void *a, struct svc_req *req)
{
     static int ret = 0;

     ret = getMessageCount();

     return (&ret);
}

char *getFileNameWith(int messageIndex)
{
     char savedFileNamesCSV[MAX_FILE_SIZE] = "";
     char *entireFileContent = readEntireFile(SAVED_FILE_NAMES);
     strcpy(savedFileNamesCSV, entireFileContent);

     int i = 1;
     while (i < messageIndex)
     {
          strcpy(savedFileNamesCSV, strchr(savedFileNamesCSV, ';'));
          strcpy(savedFileNamesCSV, substr(savedFileNamesCSV, 1, strlen(savedFileNamesCSV)));
          i++;
     }

     int endIndex = strchr(savedFileNamesCSV, ';') - savedFileNamesCSV;
     savedFileNamesCSV[endIndex] = '\0';

     char *p_savedFileNamesCSV = savedFileNamesCSV;
     return p_savedFileNamesCSV;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char **receivechat_1_svc(int *a, struct svc_req *req)
{
     printf("\nRecebido requisição da mensagem %d\n", *a);

     char fileName[MAX_FILE_NAME_LENGTH] = "";
     strcpy(fileName, getFileNameWith(*a));

     static char fileContent[MAX_FILE_SIZE] = "";
     strcpy(fileContent, readEntireFile(fileName));

     static char *p;
     p = fileContent;
     return (&p);
}

int NUMBER_OF_CONNECTIONS = 0;
char CLIENT_NAMES[100][100];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *connect_1_svc(char **a, struct svc_req *req)
{
     int clientExists = 0;

     for (int i = 0; i < 100; i++)
     {
          if (strcmp(CLIENT_NAMES[i], *a) == 0)
          {
               clientExists = 1;
               break;
          }
     }

     int ret;
     if (clientExists)
     {
          ret = -1;
          printf("\nConexão recusada de %s. Já existe outro cliente com esse nickname conectado.\n", *a);
     }
     else
     {
          strcpy(CLIENT_NAMES[NUMBER_OF_CONNECTIONS], *a);
          NUMBER_OF_CONNECTIONS++;
          ret = NUMBER_OF_CONNECTIONS;
          printf("\nConexão criada para %s. ID criado: %d.\n", *a, ret);
     }

     int *p_ret = &ret;
     return (p_ret);
}