#include <rpc/rpc.h>
#include <unistd.h>
#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char **func0_1_svc(void *a, struct svc_req *req) {
	static char msg[256];
	static char *p;

	printf("FUNC0 (sem parâmetros)\n");
	strcpy(msg, "Hello Func0!");
	p = msg;

	return(&p);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func1_1_svc(char **a, struct svc_req *req) {
    static int ret = 1;

    printf ("FUNC1 (%s)\n", *a);

    return (&ret);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func2_1_svc(int *a, struct svc_req *req) {
     static int ret = 10;

     printf ("FUNC2 (%d)\n", *a);
     return (&ret);

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func3_1_svc(struct param *a, struct svc_req *req) {
     static int ret=0;

     printf ("FUNC3 (%d/%d)\n", a->arg1, a->arg2);
     ret = a->arg1 * a->arg2;
     return (&ret);
}

char SAVED_FILE_NAMES[15] = "savedFileNames";

void saveFileName(char* fileName){

     FILE *filewrite;
     filewrite = fopen(SAVED_FILE_NAMES, "a+");
     if (filewrite == NULL) {
		printf("Error: Na abertura do arquivo (%s).", SAVED_FILE_NAMES);
		exit(1);
	}

     strcat(fileName, ";");
     writeline(fileName, filewrite);
     fclose(filewrite);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *sendchat_1_svc(struct msg *a, struct svc_req *req) {
     
     printf ("\nRecebido mensagem de %s\n", a->nickname);

     // finding proper name
     char fileName[200];
     int i = 0;
     char strIndex[10];
     
     sprintf(strIndex, "%d", getMessageCount());
     strcat(fileName, a->nickname);
     strcat(fileName, "-");
     strcat(fileName, strIndex);
     strcat(fileName, ".serv");

     FILE *filewrite;
     filewrite = fopen(fileName, "w");
     if (filewrite == NULL) {
		printf("Error: Na abertura do arquivo (%s).", fileName);
		exit(1);
	}
     writeline(a->content, filewrite);
     fclose(filewrite);

     printf ("\nSalvo content em %s\n", fileName);
     iterateMessageCount();
     saveFileName(fileName);
     
     static int ret=1;
     return (&ret);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *getmsgindex_1_svc(void *a, struct svc_req *req) {
     
     static int ret = 0;

     ret = getMessageCount();

     return (&ret);
}

char* getFileNameWith(int messageIndex){

     char savedFileNamesCSV[MAX_FILE_SIZE];
     strcpy(savedFileNamesCSV, readEntireFile(SAVED_FILE_NAMES));

     int i = 0;
     
     while(i < messageIndex){
          strcpy(savedFileNamesCSV, strchr(savedFileNamesCSV, ';'));
     }

     int endIndex = strchr(savedFileNamesCSV, ';') - savedFileNamesCSV;
     savedFileNamesCSV[endIndex] = '\0';

     char* p_savedFileNamesCSV = savedFileNamesCSV;
     return p_savedFileNamesCSV;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char** receivechat_1_svc(int *a, struct svc_req *req) {
     
     printf ("\nRecebido requisição da mensagem %d\n", *a);

     static char fileContent[MAX_FILE_SIZE];
     strcpy(fileContent, readEntireFile(getFileNameWith(*a)));
     
     static char* p;
     p = fileContent;
     return (&p);
}