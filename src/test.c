#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


int main(int argc, char *argv[]){
	int err=untar(argv[1],argv[2]);
	printf("err : %d\n", err);
	int send;


	char buffer_out[1024];
	char buffer_in[1024];
	memset(buffer_out,0,sizeof buffer_out);
	memset(buffer_in,0,sizeof buffer_in);
	strcpy(buffer_out,"TEST A ENVOYER\n");

	int pipefd[2];
	if(pipe(pipefd)==0){
		printf("succes pipe -%d-%d\n",pipefd[0],pipefd[1]);	
	}
        
	printf("%s-%d-%d\n",buffer_out,pipefd[0],sizeof buffer_out);
        if((send=write(pipefd[1],buffer_out,sizeof buffer_out))==-1){
		printf("%d\n",send);
		printf("test>Probleme ecriture buffer_out\n");
	}

        readWriteFD(pipefd[0],pipefd[1]);

        if((read(pipefd[0],&buffer_in,sizeof buffer_in))==-1){
		printf("test>Problème lecture buffer_in\n");
	}

	printf("|%s|\n",buffer_in);
	
	return 0;
}
