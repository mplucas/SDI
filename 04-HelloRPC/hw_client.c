#include <stdio.h>
#include <rpc/rpc.h>
#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

int main (int argc, char *argv[]) {
	
	setbuf(stdout, NULL);
	// Estrutura RPC de comunicação
	CLIENT *cl;

	// Parâmetros das funçcões
	char        *par_f1 = (char *) malloc(256*sizeof(char));
	int          par_f2;
	struct param par_f3;
	struct msg   par_f4;

	// Retorno das funções
	char **ret_f0 = NULL;
	int   *ret_f1 = NULL;
	int   *ret_f2 = NULL;
	int   *ret_f3 = NULL;
	int   *ret_f4 = NULL;

	// Verificação dos parâmetros oriundos da console	
	if (argc != 5) {
		printf("ERRO: ./client <hostname> <msg> <nickname> <filename>\n");
		exit(1);
	}

	// Conexão com servidor RPC
	cl = clnt_create(argv[1], PROG, VERS, "tcp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	// Atribuições de valores para os parâmetros
	strcpy (par_f1, argv[2]);
	par_f2 = 1;
	par_f3.arg1 = 5;
	par_f3.arg2 = 4;
	strcpy (par_f4.nickname, argv[3]);
	FILE *fileread;
	char *msg = NULL;
	fileread = fopen(argv[4],"r");
	if (fileread == NULL) {
		printf("Error: Na abertura dos arquivos (%s).", argv[4]);
		exit(1);
	}
	do{
		msg = readline(fileread);
		strcat (par_f4.content, msg);
	}while(strlen(msg) == 256);
	fclose(fileread);	

	// Chamadas das funções remotas
	printf ("Chamando func0 (sem parâmetros)\n");
	ret_f0 = func0_1(NULL, cl);
	if (ret_f0 == NULL) {
	    clnt_perror(cl,argv[1]);
	    exit(1);
	}
	printf ("Retorno func0 (%s)\n", *ret_f0);

	printf ("Chamando func1 (%s)\n", par_f1);
	ret_f1 = func1_1(&par_f1, cl);
	if (ret_f1 == NULL) {
	    clnt_perror(cl,argv[1]);
	    exit(1);
	}
	printf ("Retorno func1 (%d)\n", *ret_f1);

	printf ("Chamando func2 (%d)\n", par_f2);
	ret_f2 = func2_1(&par_f2, cl);
	if (ret_f2 == NULL) {
	    clnt_perror(cl,argv[1]);
	    exit(1);
	}
	printf ("Retorno func2 (%d)\n", *ret_f2);

	printf ("Chamando func3 (%d/%d)\n", par_f3.arg1, par_f3.arg2);
	ret_f3 = func3_1(&par_f3, cl);
	if (ret_f3 == NULL) {
	    clnt_perror(cl,argv[1]);
	    exit(1);
	}
	printf ("Retorno func3 (%d)\n", *ret_f3);

	printf ("\nChamando func4: enviando...\n\nnickname: %s\ncontent: %s\n\n", par_f4.nickname, par_f4.content);
	ret_f4 = func4_1(&par_f4, cl);
	if (ret_f4 == NULL) {
	    clnt_perror(cl,argv[1]);
	    exit(1);
	}
	printf ("Retorno func4 (%d)\n", *ret_f4);

	return 0;
}
