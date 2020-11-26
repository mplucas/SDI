#include <stdio.h>
#include <rpc/rpc.h>
#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

int main (int argc, char *argv[]) {
	
	// setbuf(stdout, NULL);
	// Estrutura RPC de comunicação
	CLIENT *cl;

	// Parâmetros das funçcões
	struct msg   par_f4;

	// Retorno das funções	
	int   *ret_f4 = NULL;

	// Verificação dos parâmetros oriundos da console	
	if (argc != 4) {
		printf("ERRO: ./client <hostname> <nickname> <filename/'poll'>\n");
		exit(1);
	}

	// Conexão com servidor RPC
	cl = clnt_create(argv[1], PROG, VERS, "tcp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	// Atribuições de valores para os parâmetros
	strcpy (par_f4.nickname, argv[2]);
	FILE *fileread;
	char *msg = NULL;
	fileread = fopen(argv[3],"r");
	if (fileread == NULL) {
		printf("Error: Na abertura dos arquivos (%s).", argv[3]);
		exit(1);
	}
	do{
		msg = readline(fileread);
		strcat (par_f4.content, msg);
	}while(strlen(msg) == 256);
	fclose(fileread);	

	// Chamadas das funções remotas
	printf ("\nEnviando mensagem de %s...\n", par_f4.nickname);
	ret_f4 = func4_1(&par_f4, cl);
	if (ret_f4 == NULL) {
	    clnt_perror(cl,argv[1]);
	    exit(1);
	}
	printf ("Enviado com sucesso!\n");

	return 0;
}
