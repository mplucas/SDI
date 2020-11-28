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

	char localHost[200] = "";
	strcpy(localHost, argv[1]);
	char nickname[200] = "";
	strcpy(nickname, argv[2]);
	char filename_poll[200] = "";
	strcpy(filename_poll, argv[3]);

	// Estrutura RPC de comunicação
	CLIENT *cl;

	// Conexão com servidor RPC
	cl = clnt_create(localHost, PROG, VERS, "tcp");
	if (cl == NULL)
	{
		clnt_pcreateerror(localHost);
		exit(1);
	}

	if (strcmp(filename_poll, "poll") != 0)
	{ // se for envio de arquivo

		// Parâmetros das funçcões
		struct msg par_sendchat;

		// Retorno das funções
		int *ret_sendchat = NULL;

		// Atribuições de valores para os parâmetros
		strcpy(par_sendchat.nickname, nickname);
		strcpy(par_sendchat.content, readEntireFile(filename_poll));

		// Chamadas das funções remotas
		printf("\nEnviando mensagem de %s...\n", par_sendchat.nickname);
		ret_sendchat = sendchat_1(&par_sendchat, cl);
		if (ret_sendchat == NULL)
		{
			clnt_perror(cl, localHost);
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

			// finding proper name
			char fileName[200] = "";
			char strIndex[10] = "";
			int msgCount = getMessageCount();

			sprintf(strIndex, "%d", msgCount);
			strcat(fileName, nickname);
			strcat(fileName, "-");
			strcat(fileName, strIndex);
			strcat(fileName, ".client");

			saveMessageInFile(nickname, *ret_receivechat, fileName);

			printf("\nSalvo content em %s\n", fileName);
		}
	}

	return 0;
}
