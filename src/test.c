#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "cmd.h"
#include "utils.h"
#include "exec.h"




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

void createCmd(){
	
	
    char reponse[]="      2      18     129";

	char str1[]="ls ";
	char str2[]="grep f";
	char str3[]="wc" ;


	printf("-----------------------\n");
	Cmd* cmd1 = cmd_init(str1);
	int fd1[2];
	pipe(fd1);


	cmd1->fd_out=fd1[1];

	cmd_exec(cmd1);

	close(fd1[1]);
	printf("-----------------------\n");

	
	int fd2[2];
	pipe(fd2);

	Cmd* cmd2=cmd_init(str2);
	cmd2->fd_in=fd1[0];
	cmd2->fd_out=fd2[1];

	cmd_exec(cmd2);
	close(fd2[1]);
	printf("-----------------------\n");


	int fd3[2];
	pipe(fd3);

	Cmd* cmd3=cmd_init(str3);
	cmd3->fd_in=fd2[0];
	cmd3->fd_out=fd3[1];

	cmd_exec(cmd3);
	close(fd3[1]);

	char c[1];
	int i=0;
	int length=strlen(reponse);
	int ok=1;
	while(read(fd3[0],c,1) && ok){
		if(i<length){
			if(reponse[i]!=c[0]){
				ok=0;
			}
		}
		i++;
	}
	if(ok){
		printf("ok\n");
	}else{
		printf("ko\n");
	}
}

void testExec(char* str){
	Exec* exec = exec_init(str);
	exec_execute(exec);

	char c[1];
	printf("--------- RESULT %s ---------\n",str);
	while(read(exec->fd_out,c,1)){
		printf("%c",c[0] );
	}
}

int main(int argc, char *argv[]){
	/*char str[] = "ls -la | grep ocucou && wc";*/
	/*testExec("pwd && ls | wc >> testTruc");*/
	testExec("cd test");
	/*createCmd();*/
	/*
	int err=untar("zada/dza/azdazd/dazgreger/gregre/ger/lol.tar.gz");
	printf("err : %d\n", err);
	*/
	/*
	test_fonction_ecriture_lecture_readWriteFD();
	printf("TEST2\n");
	
	test_2_read_writeFD();
	test_3_read_writeFD();
	*/
	/*
	printf("%s\n",get_current_dir_name());	
	*/
	return 0;
}
