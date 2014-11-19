#include "utils.h"


int checkWritingFolder(char* path){
	/* TODO improve check */
	DIR* dir;
	dir=opendir(path);
	return dir!=NULL;
}

void create_leash_directory(){

        /* get home and hidden leaSh directory for data storage */

        char* home = getenv("HOME");
        char* repertoire_leash = (char*)malloc((sizeof(home)) + 1000*(sizeof(char)));
        strcpy(repertoire_leash,home);
        strcat(repertoire_leash,"/.leaSh");
        char* repertoire_tmp = (char*)malloc((sizeof(home)) + 1000*(sizeof(char)));
        strcpy(repertoire_tmp,repertoire_leash);
        strcat(repertoire_tmp,"/tmp");

        /* Check / Create .leaSh directory in user home */

        if ((opendir(repertoire_leash))==NULL){
                mkdir(repertoire_leash,S_IRWXU| /* Gives user right RWX for the directory */
                                       S_IRGRP| /* Grants the group the ability to read */
                                       S_IXGRP| /* and execute */
                                       S_IROTH| /* Grants others the ability to read */
                                       S_IXOTH);/* and execute */
        } else {
                printf("Le dossier ~/.leaSh existe deja\n");
        }

        if((opendir(repertoire_tmp))==NULL){
                 mkdir(repertoire_tmp,S_IRWXU| /* Gives user right RWX for the directory */
                                      S_IRGRP| /* Grants the group the ability to read */
                                      S_IXGRP| /* and execute */
                                      S_IROTH| /* Grants others the ability to read */
                                      S_IXOTH);/* and execute */
        }else{
		printf("Le dossier ~/.leaSh/tmp existe deja\n");
	}
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
		printf("BOUHHHHH\n");
		return 1;
	}else{
		if(flags & EXEC_WAIT_SON){
			int status;
			waitpid(pid,&status,0);
			if(status){
				return status;
			}
			printf("fin : %d\n",status );
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


void parseCommand(char command[]){
	printf("%s\n",command );
	int i=0;
	int length=strlen(command);
	int nbCmd=1;
	for(i=0;i<length;i++){
		if((command[i]=='|' && command[i+1]!='|' && command[i-1]!='|') || (command[i]=='&' && command[i+1]=='&') || (command[i]=='|' && command[i+1]=='|') ){
			nbCmd++;
		}
	}
	printf("nb commandes : %d\n",nbCmd );
	char** tabcmd=(char**)malloc(sizeof(char*)*nbCmd);
	if(nbCmd==1){
		tabcmd[0]=command;
	}else{
		int* link=(int*)malloc(sizeof(int)*nbCmd-1);
		int next=0;
		tabcmd[0]=command;
		int nb=0;
		for(i=0;i<length;i++){
			if(next){
				printf("next\n");
				next=0;
				tabcmd[nb]=&command[i];
			}else if(command[i]=='|' && command[i+1]!='|' && command[i-1]!='|'){
				printf("|\n");
				command[i]='\0';
				link[nb]=1;
				nb++;
				next=1;
			}else if(command[i]=='&'){
				printf("&...\n");
				if(i+1<length && command[i+1]=='&'){
					printf("&&\n");
					next=1;
					command[i]='\0';
					command[i+1]='\0';
					link[nb]=2;
					nb++;
					i++;
				}
			}else if(command[i]=='|'){
				printf("|...\n");
				if(i+1<length && command[i+1]=='|'){
					printf("||\n");
					next=1;
					command[i]='\0';
					command[i+1]='\0';
					link[nb]=3;
					nb++;
					i++;
				}
			}
			
		}
	}
	for(i=0;i<nbCmd;i++){
		printf("%d : %s\n",i,tabcmd[i] );
	}

}
