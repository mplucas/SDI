#include <stdio.h>
#include <rpc/rpc.h>
#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

int main(int argc, char *argv[])
{

	//setbuf(stdout, NULL);

	// Verificação dos parâmetros oriundos da console
	if (argc != 4)
	{
		printf("ERRO: ./client <hostname> <nickname> <filename/'poll'>\n");
		exit(1);
	}

	// Estrutura RPC de comunicação
	CLIENT *cl;

	// Conexão com servidor RPC
	cl = clnt_create(argv[1], PROG, VERS, "tcp");
	if (cl == NULL)
	{
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	if (strcmp(argv[3], "poll") != 0)
	{ // se for envio de arquivo

		// Parâmetros das funçcões
		struct msg par_sendchat;

		// Retorno das funções
		int *ret_sendchat = NULL;

		// Atribuições de valores para os parâmetros
		strcpy(par_sendchat.nickname, argv[2]);
		strcpy(par_sendchat.content, readEntireFile(argv[3]));

		// Chamadas das funções remotas
		printf("\nEnviando mensagem de %s...\n", par_sendchat.nickname);
		ret_sendchat = sendchat_1(&par_sendchat, cl);
		if (ret_sendchat == NULL)
		{
			clnt_perror(cl, argv[1]);
			exit(1);
		}
		printf("Enviado com sucesso!\n");
	}
	else
	{ // se for polling

		// Parâmetros das funçcões
		struct msg par_getmsgindex;

		// Retorno das funções
		int *ret_getmsgindex = NULL;
		ret_getmsgindex = getmsgindex_1(&par_getmsgindex, cl);

		int localMessageCount = getMessageCount();
		int serverMessageCount = *ret_getmsgindex;

		for (int i = localMessageCount + 1; i <= serverMessageCount; i++)
		{
			// Retorno das funções
			char **ret_receivechat = NULL;
			ret_receivechat = receivechat_1(&i, cl);

			printf("\nRecebido mensagem %d do server.", i);

			char fileName[200] = "";

			printf("\nSalvo content em %s\n", fileName);
		}
	}

	return 0;
}
