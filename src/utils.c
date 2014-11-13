#include "utils.h"


int checkWritingFolder(char* path){
	/* TODO improve check */
	DIR* dir;
	dir=opendir(path);
	return dir!=NULL;
}

int untar(char* path){
	

	char* str = (char*)malloc(strlen(path)+ 1);
	strcpy(str,path);
	char* token =  strtok(str,"/");
	char* tmptoken = token;
	while(token != NULL){
		printf("%s\n",token);
		tmptoken = token;
		token=strtok(NULL,"/");
		
	}
	
	char* name = strtok(tmptoken,".");	
	printf("%s\n",name);
	
	char* untarPath = (char*)malloc(1 + strlen(path) + strlen(name));
	strcpy(untarPath,"/.leaSH/tmp/");
	strcat(untarPath,name);
	printf("%s\n",untarPath);

	
/*
	if(checkWritingFolder(untarPath)){
		char* tabargs[6] = {"tar","-zxvf",path,"-C",untarPath,NULL};
		int err = execSimple("tar",tabargs,NULL,NULL,EXEC_WAIT_SON);
		if(err){
			printf("Error untar : %d",err);
			return 1;
		}
	}else{
		printf("folder read_only/not exists\n");
		return 3;
	}

*/
	return 0;
}


int tarSize(char* path){
	/* TODO */
	/* tar tzvf archive.tar.gz | sed 's/ \+/ /g' | cut -f3 -d' ' | sed '2,$s/^/+ /' | paste -sd' ' | bc */
	return 0;
}


int execSimple(char* cmd, char* args[], int in[2], int out[2], int flags){
	/* flags : 
	   0 -> nothing
	   1 -> branch pipe with stdio, stdout 
	   2 -> wait son
	   3 -> 1+2*/


	   int pid=fork();
	   if(pid==-1){
	   	perror("Error fork execSimple");
	   	return -1;
	   }

	   if(!pid){
	   	if(flags & EXEC_PIPE_SON){
	   		if(in){
	   			dup2(in[0],0);
	   			close(in[1]);
	   		}
	   		if(out){
	   			dup2(out[1],1);
	   		}
	   	}else{
	   		close(0);
	   		close(1);
	   		close(2);
	   	}

	   	execvp(cmd,args);
	   	return -1;
	   }else{
	   	if(flags & EXEC_WAIT_SON){
	   		waitpid(pid,NULL,0);

	   		if(in){
	   			close(in[0]);
	   		}
	   		if(out){
	   			close(out[1]);
	   		}
	   	}
	   	return 0;

	   }
	}

	void readWriteFD (int fdin,int fdout) {
		char message[8];
		int r;	

		while((r=read(fdin,message,8))>0){
			write(fdout,message,r);
		}
	}
