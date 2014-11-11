#include "utils.h"


int checkWritingFolder(char* path){
	/* TODO improve check */
	DIR* dir;
	dir=opendir(path);
	return dir!=NULL;
}

int untar(char* path, char* untarPath){
	if(checkWritingFolder(untarPath)){
		char* tabargs[6] = {"tar","-zxvf",path,"-C",untarPath,NULL};
		int err = execSimple("tar",tabargs,NULL,EXEC_WAIT_SON|EXEC_SILENCE);
		if(err){
			printf("Error untar : %d",err);
			return 1;
		}
	}else{
		printf("folder read_only/not exists\n");
		return 3;
	}
	return 0;
}


int tarSize(char* path){
	/* TODO */
	/* tar tzvf archive.tar.gz | sed 's/ \+/ /g' | cut -f3 -d' ' | sed '2,$s/^/+ /' | paste -sd' ' | bc */
	return 0;
}


int execSimple(char* cmd, char* args[], int* fd, int flags){
	/* flags : 
	0 -> nothing
	1 -> branch pipe with stdio, stdout 
	2 -> wait son
	3 -> 1+2*/
	if(flags & EXEC_PIPE_SON){
		pipe(fd);	
	}
	int pid=fork();
	if(pid==-1){
		perror("Error fork execSimple");
		return 1;
	}
	int fd2[2][2];
	pipe(fd2[0]);
	pipe(fd2[1]);
	

	if(!pid){
		if(flags & EXEC_PIPE_SON){
			dup2(fd2[0][1],1);
			close(fd2[0][0]);

			dup2(fd2[1][0],0);
			close(fd2[1][1]);
		}
		if(flags & EXEC_SILENCE){
			close(1);
			close(2);
		}
		execvp(cmd,args);
		return 1;
	}else{
		if(flags & EXEC_PIPE_SON){
			dup2(fd2[0][0],fd[0]);
			dup2(fd2[1][1],fd[1]);
			close(fd2[0][1]);
			close(fd2[1][0]);
		}
		if(flags & EXEC_WAIT_SON){
			waitpid(pid,NULL,0);
		}
		return 0;
	}
}