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

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int *func4_1_svc(struct msg *a, struct svc_req *req) {
     
     printf ("\nRecebido mensagem de %s\n", a->nickname);

     // finding proper name
     char fname[200];
     int i = 0;
     char strIndex[10];
     do{
          
          strcpy(strIndex, "");
          sprintf(strIndex, "%d", i);

          strcat(fname, a->nickname);
          strcat(fname, "-");
          strcat(fname, strIndex);
          strcat(fname, ".serv");

          i++;

     }while(access(fname, F_OK) != -1);

     FILE *filewrite;
     filewrite = fopen(fname, "w");
     if (filewrite == NULL) {
		printf("Error: Na abertura do arquivo (%s).", fname);
		exit(1);
	}
     writeline(a->content, filewrite);
     fclose(filewrite);

     printf ("\nSalvo content em %s\n", fname);
     
     static int ret=1;
     return (&ret);
}