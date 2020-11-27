#include <time.h>
#include <sys/time.h>
#include "tools.h"

// %%%%%%%%%%%%%%% FERRAMENTAS p/ VETORES %%%%%%%%%%%%%%%
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + (double) t.tv_usec / 1000000;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int writeline (char *msg, FILE *file){
  fprintf(file,"%s", msg);
  return 0;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
char *readline (FILE *file){
  static int cont=0;
  char *getLine = NULL;
  char c;
  int linha=0;

  getLine = (char *) malloc (256*sizeof(char));
  fseek( file, cont, SEEK_SET );
  for (;;cont++) {
    c = fgetc( file );
    if ((linha == 256) || (c ==  EOF)){
      //printf ("%d(%c)\n",cont, c);
      return (getLine);
    }
    getLine[linha++] = c;
    //printf ("%d[%d](%c)\n",cont, linha, c);
  }
  return NULL;
}

char MESSAGE_COUNT_FILE_NAME[13] = "messageCount";

int getMessageCount(){
     
     FILE* fileRead = fopen(MESSAGE_COUNT_FILE_NAME, "r");
     
     if (fileRead == NULL) {
		return 0;
	}

     char *strCount = readline(fileRead);

     if(strlen(strCount) <= 0){
          return 0;
     }

     fclose(fileRead);
     
     return atoi(strCount);
}

void iterateMessageCount(){

     int currentMessageCount = getMessageCount();

     FILE *filewrite;
     filewrite = fopen(MESSAGE_COUNT_FILE_NAME, "w");
     if (filewrite == NULL) {
		printf("Error: Na abertura do arquivo (%s).", MESSAGE_COUNT_FILE_NAME);
		exit(1);
	}

     char strIndex[10];
     sprintf(strIndex, "%d", currentMessageCount + 1);
     writeline(strIndex, filewrite);

     fclose(filewrite);
}

char* readEntireFile(char* fileName){

  char entireFile[MAX_FILE_SIZE];
  char *msg = NULL;
  FILE *fileread;
  fileread = fopen(fileName,"r");
  if (fileread == NULL) {
    printf("Error: Na abertura dos arquivos (%s).", fileName);
    exit(1);
  }
  do{
    msg = readline(fileread);
    strcat (entireFile, msg);
  }while(strlen(msg) == 256);
  fclose(fileread);	

  char* p_entireFile = entireFile;
  return p_entireFile;
}