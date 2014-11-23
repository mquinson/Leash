#include "utils.h"


void die(const char* message){
	perror(message);
	exit(EXIT_FAILURE);
}

char* get_tar_name(char* tar){
	char* token;
	token = strtok(tar,"./");
	printf("%s\n",token);
	return token;
}


char* get_env_leash(char* home){
	char* repertoire_leash = (char*)malloc(strlen(home)+1000*sizeof(char));
	strcpy(repertoire_leash,home);
	strcat(repertoire_leash,"/.leaSh");
	return repertoire_leash;
}

char* get_env_level(char* repertoire_leash,char* name){
	char* repertoire_level = (char*)malloc(strlen(repertoire_leash)+1000*sizeof(char));
	strcpy(repertoire_level,repertoire_leash);
	strcat(repertoire_level,"/");
	strcat(repertoire_level,name);
	return repertoire_level;
}
	


int checkWritingFolder(char* path){
	/* TODO improve check */
	DIR* dir;
	dir=opendir(path);
	return dir!=NULL;
}

void create_leash_directory(char* home,char* repertoire_leash,char* repertoire_tmp){

        /* Check / Create .leaSh directory in user home */

	if ((opendir(repertoire_leash))==NULL){
                mkdir(repertoire_leash,S_IRWXU| /* Gives user right RWX for the directory */
                                       S_IRGRP| /* Grants the group the ability to read */
                                       S_IXGRP| /* and execute */
                                       S_IROTH| /* Grants others the ability to read */
                                       S_IXOTH);/* and execute */
	}

	if((opendir(repertoire_tmp))==NULL){
                 mkdir(repertoire_tmp,S_IRWXU| /* Gives user right RWX for the directory */
                                      S_IRGRP| /* Grants the group the ability to read */
                                      S_IXGRP| /* and execute */
                                      S_IROTH| /* Grants others the ability to read */
                                      S_IXOTH);/* and execute */
	}

}


int untar(char* path,char* untarPath){
	
	if(checkWritingFolder(untarPath)){
		char* tabargs[6] = {"tar","-zxf",path,"-C",untarPath,NULL};
		int err = execSimple("tar",tabargs,NULL,NULL,EXEC_WAIT_SON);
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


int execSimple(char* cmd, char* args[], int in[2], int out[2], int flags){



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
				dup2(1,2);
				dup2(out[1],1);
			}
		}else{
			close(0);
			close(1);
			close(2);
		}

		execvp(cmd,args);
		printf("ERROR EXEC SIMPLE !\n");
		return 1;
	}else{
		int status=0;
		if(flags & EXEC_WAIT_SON){
			
			waitpid(pid,&status,0);
			if(status){
				return status;
			}
			if(in){
				close(in[0]);
			}
			if(out){
				/*close(out[1]);*/
			}
		}
		return status;

	}
}

void readWriteFD (int fdin,int fdout) {
	char message[8];
	int r;	

	while((r=read(fdin,message,8))>0){
		write(fdout,message,r);
	}
}

int nbWords(char* str){
	int nbWords=0;
	int len = strlen(str);
	int car=str[0]!=' ';
	if(car){
		nbWords++;
	}
	int i;
	for(i=1;i<len;i++){
		int tmp=(str[i]!=' ');
		if(tmp!=car && !car && tmp){
			nbWords++;
		}
		car=tmp;
	}
	return nbWords;
}

char* trim(char *str){
	char *end;

	while(isspace(*str)){
		str++;
	} 
	if(*str == 0){
		return str;
	}
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)){
		end--;	
	} 
	*(end+1) = '\0';
	return str;
}
