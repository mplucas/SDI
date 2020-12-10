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

char savedFileNames[100][MAX_FILE_NAME_LENGTH];

void saveFileName(char *fileName)
{
     int i = 0;
     while (strlen(savedFileNames[i]) != 0)
     {
          i++;
     }

     strcpy(savedFileNames[i], fileName);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *sendchat_1_svc(struct msg *a, struct svc_req *req)
{
     printf("\nRecebido mensagem de %s\n", a->nickname);

     // finding proper name
     char fileName[MAX_FILE_NAME_LENGTH] = "";
     char strIndex[10] = "";
     int msgIndex = getMessageIndex();

     sprintf(strIndex, "0%d", msgIndex);
     strcat(fileName, a->nickname);
     strcat(fileName, "-");
     strcat(fileName, substr(strIndex, strlen(strIndex) - 2, strlen(strIndex)));
     strcat(fileName, ".serv");

     saveContentInFile(a->content, fileName);
     iterateMessageIndex();

     printf("Salvo content em %s\n", fileName);
     saveFileName(fileName);

     static int ret = 1;
     return (&ret);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *getmsgindex_1_svc(void *a, struct svc_req *req)
{
     static int ret = 0;

     ret = getMessageIndex();

     return (&ret);
}

char *getFileNameWith(int messageIndex)
{
     char savedFileName[MAX_FILE_NAME_LENGTH] = "";

     if (messageIndex - 1 >= 0 && messageIndex - 1 <= 99)
     {
          strcpy(savedFileName, savedFileNames[messageIndex - 1]);
     }
     else
     {
          strcpy(savedFileName, "ID da mensagem requisitada invalido.");
     }

     char *p_savedFileName = savedFileName;
     return p_savedFileName;
}

char *getNicknameFromFileName(char *fileName)
{
     char nickname[MAX_FILE_NAME_LENGTH] = "";

     int i = 0;
     for (i = 0; i < strlen(fileName); i++)
     {
          if (fileName[i] == '-')
          {
               break;
          }
     }

     return substr(fileName, 0, i);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
struct msg *receivechat_1_svc(int *a, struct svc_req *req)
{
     printf("\nRecebido requisição da mensagem %d\n", *a);

     char fileName[MAX_FILE_NAME_LENGTH] = "";
     strcpy(fileName, getFileNameWith(*a));

     static char fileContent[MAX_FILE_SIZE] = "";
     strcpy(fileContent, readEntireFile(fileName));

     printf("Enviando content do arquivo %s\n", fileName);

     struct msg returnMsg;
     strcpy(returnMsg.nickname, getNicknameFromFileName(fileName));
     strcpy(returnMsg.content, fileContent);

     static struct msg p;
     p = returnMsg;
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