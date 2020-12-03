#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include "tools.h"

// Interface gerada pelo RPCGen a partir da IDL (hw.x) especificada
#include "hw.h"

int fileExists(char *fileName)
{
	FILE *file;
	file = fopen(fileName, "r");

	if (file != NULL)
	{
		fclose(file);
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char *argv[])
{

	// setbuf(stdout, NULL);

	// Verificação dos parâmetros oriundos da console
	if (argc != 3)
	{
		printf("ERRO: ./client <hostname> <nickname>\n");
		exit(1);
	}

	char localHost[200] = "";
	strcpy(localHost, argv[1]);
	char nickname[200] = "";
	strcpy(nickname, argv[2]);

	// Estrutura RPC de comunicação
	CLIENT *cl;

	// Conexão com servidor RPC
	cl = clnt_create(localHost, PROG, VERS, "tcp");
	if (cl == NULL)
	{
		clnt_pcreateerror(localHost);
		exit(1);
	}

	// Parâmetros das funçcões
	char *par_connect;
	par_connect = nickname;

	// Retorno das funções
	int *ret_connect = NULL;
	ret_connect = connect_1(&par_connect, cl);
	int clientID = *ret_connect;

	if (clientID == -1)
	{
		printf("\nConexão recusada. Deve haver um outro cliente com o mesmo nickname já conectado.\n");
		return 0;
	}
	else
	{
		printf("\nConexão realizada. ClientID recebido: %d\nEnvie mensagens ao server a começar pela adição de um arquivo %s-01.chat no repositório.\n", clientID, nickname);
	}

	int sendChatIndex = 1;
	while (1)
	{
		//sending .chat messages --------------------------------------------------------------------------------
		printf("\nVerificando existencia de mensagens para envio...\n");

		int hasSendMessages = 1;
		while (hasSendMessages)
		{
			char sendFileName[MAX_FILE_NAME_LENGTH] = "";
			char strSendIndex[10] = "";

			sprintf(strSendIndex, "0%d", sendChatIndex);
			strcat(sendFileName, nickname);
			strcat(sendFileName, "-");
			strcat(sendFileName, substr(strSendIndex, strlen(strSendIndex) - 2, strlen(strSendIndex)));
			strcat(sendFileName, ".chat");

			if (fileExists(sendFileName))
			{
				// Parâmetros das funçcões
				struct msg par_sendchat;

				// Retorno das funções
				int *ret_sendchat = NULL;

				// Atribuições de valores para os parâmetros
				strcpy(par_sendchat.nickname, nickname);
				strcpy(par_sendchat.content, readEntireFile(sendFileName));

				// Chamadas das funções remotas
				printf("Enviando mensagem arquivo %s...\n", sendFileName);
				ret_sendchat = sendchat_1(&par_sendchat, cl);
				if (ret_sendchat == NULL)
				{
					clnt_perror(cl, localHost);
					exit(1);
				}
				printf("Enviado %s com sucesso!\n", sendFileName);
				sendChatIndex++;
			}
			else
			{
				printf("Nenhuma mensagem para envio encontrada com o nome %s.\n", sendFileName);
				hasSendMessages = 0;
			}
		}
		//sending .chat messages end ----------------------------------------------------------------------------

		// Polling ----------------------------------------------------------------------------------------------
		printf("Realizando polling...\n");
		// Parâmetros das funçcões
		struct msg par_getmsgindex;

		// Retorno das funções
		int *ret_getmsgindex = NULL;
		ret_getmsgindex = getmsgindex_1(&par_getmsgindex, cl);

		int localMessageIndex = getMessageIndex();
		int serverMessageIndex = *ret_getmsgindex;

		if (localMessageIndex >= serverMessageIndex)
		{
			printf("Nenhuma nova mensagem do server encontrada.\n");
		}

		for (int i = localMessageIndex; i < serverMessageIndex; i++)
		{
			// Retorno das funções
			struct msg *ret_receivechat = NULL;
			ret_receivechat = receivechat_1(&i, cl);

			printf("Recebido mensagem %d do server.\n", i);

			if (strcmp(ret_receivechat->nickname, nickname) == 0)
			{
				printf("Mensagem %d é uma mensagem enviada, não salvando a mensagem.\n", i);
			}
			else
			{

				// finding proper name
				char receiveFileName[MAX_FILE_NAME_LENGTH] = "";
				char strReceiveIndex[10] = "";
				int msgIndex = getMessageIndex();

				sprintf(strReceiveIndex, "0%d", msgIndex);
				strcat(receiveFileName, nickname);
				strcat(receiveFileName, "-");
				strcat(receiveFileName, substr(strReceiveIndex, strlen(strReceiveIndex) - 2, strlen(strReceiveIndex)));
				strcat(receiveFileName, ".client");
				char strClientID[10] = "";
				sprintf(strClientID, "0%d", clientID);
				strcat(receiveFileName, substr(strClientID, strlen(strClientID) - 2, strlen(strClientID)));

				saveContentInFile(ret_receivechat->content, receiveFileName);
				printf("Salvo content em %s\n", receiveFileName);
			}

			iterateMessageIndex();
		}
		// Polling end ------------------------------------------------------------------------------------------

		sleep(5);
		printf("\nRefazendo envios de arquivos \".chat\" e polling do server. (5s)\n");
	}

	return 0;
}
