#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"




void test_3_read_writeFD(){

	int fdfic = open("fichiers_test/petite_taille.txt",O_RDONLY);	
	readWriteFD(fdfic,1);

}

void test_2_read_writeFD(){

	int fdfic = open("fichiers_test/grande_taille.txt",O_RDONLY);	
	readWriteFD(fdfic,1);
}

void test_fonction_ecriture_lecture_readWriteFD(){
	int send;
	char buffer_out[1024];
	memset(buffer_out,0,sizeof buffer_out);
	strcpy(buffer_out,"TEST A ENVOYER\n");

	int pipefd[2];
	pipe(pipefd);
	
	if((send=write(pipefd[1],buffer_out,sizeof buffer_out))==-1){
		printf("test>Probleme ecriture buffer_out\n");
	}
	readWriteFD(pipefd[0],1);
	char buffer_in[1024];
	memset(buffer_in,0,1024);
	if((read(pipefd[0],&buffer_in,sizeof buffer_in))==-1){
		printf("test>Problème lecture buffer_in\n");
	}

	printf("|%s|\n",buffer_in);

}

int main(int argc, char *argv[]){
	
	int err=untar("zada/dza/azdazd/dazgreger/gregre/ger/lol.tar.gz");
	printf("err : %d\n", err);
	/*
	test_fonction_ecriture_lecture_readWriteFD();
	printf("TEST2\n");
	
	test_2_read_writeFD();
	test_3_read_writeFD();
	*/
	printf("%s\n",get_current_dir_name());	
	return 0;
}
