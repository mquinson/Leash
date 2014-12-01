#include "cmd.h"



Cmd* cmd_init(char str[]){
	Cmd* cmd=(Cmd*)malloc(sizeof(Cmd));
	/*printf("str : %s\n",str );*/

	int i=0;

	cmd->fd_in=-1;
	cmd->fd_out=-1;

	cmd->nbArgs=1;
	cmd->arguments=(char**)malloc(sizeof(char*)*nbWords(str)+1);
	char s[2]=" ";
	char* token;
	i=0;
	int file_in=0;
	int file_out=0;
	token = strtok(str, s);
	int action=0;
	while( token != NULL ) {
		if(i==0){
			cmd->nom=(char*)malloc(sizeof(char)*(strlen(token)+1));
			strcpy(cmd->nom,token);
			cmd->arguments[0]=(char*)malloc(sizeof(char)*(strlen(token)+1));
			strcpy(cmd->arguments[0],token);
			i++;
		}else{
			if(file_in==1){
				action=1;
				if(cmd->fd_in==-1){
					cmd->fd_in=open(token,O_RDONLY);
					if(cmd->fd_in==-1){
						printf("erreur ouverture %s\n",token);
					}
				}else{
					printf("%s ignoré, il y a déjà un fichier d'éntré\n",token );
				}
				file_in=-1;
			}
			if(file_out==1){
				action=1;
				if(cmd->fd_out==-1){
					cmd->fd_out=open(token,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
					if(cmd->fd_out==-1){
						printf("erreur ouverture %s\n",token);
					}
				}else{
					printf("%s ignoré, il y a déjà un fichier de sortie\n",token );
				}
				file_out=-1;
			}
			if(file_out==2){
				action=1;
				if(cmd->fd_out==-1){
					int file=open(token,O_RDWR|O_CREAT,S_IRWXU);
					if(file==-1){
						printf("erreur ouverture %s\n",token);
					}
					lseek(file, 0, SEEK_END);
					cmd->fd_out=file;

				}else{
					printf("%s ignoré, il y a déjà un fichier de sortie\n",token );
				}
				file_out=-1;
			}
			if(!strcmp("<",token)){
				if(cmd->fd_in!=-1){
					printf("Seulement un seul < ,");
				}
				action=1;
				file_in=1;
			}
			if(!strcmp(">",token)){
				if(cmd->fd_out!=-1){
					printf("Seulement un seul > ,");
				}
				action=1;
				file_out=1;
			}
			if(!strcmp(">>",token)){
				if(cmd->fd_out!=-1){
					printf("Seulement un seul >> ,");
				}
				action=1;
				file_out=2;
			}
			if(!action){
				cmd->arguments[i]=(char*)malloc(strlen(token)+1);
				strcpy(cmd->arguments[i],token);
				cmd->nbArgs++;
				i++;
			}else{
				action=0;
			}

		}
		/*printf( "token :  %s\n", token );*/
		token = strtok(NULL, s);
	}
	cmd->arguments[i]=NULL;

	cmd->pid=-1;
	cmd->result=-1;
	cmd->backquoted=0;


	return cmd;
}




void cmd_dest(Cmd* cmd){
	/* TODO Test this*/
	close(cmd->fd_in);
	close(cmd->fd_out);
	free(cmd);
}

void cmd_print(Cmd* cmd){
	printf("%s  pid : %d\n",cmd->nom,cmd->pid );
	int i=0;
	for(i=0;i<cmd->nbArgs;i++){
		printf("\t%s\n", cmd->arguments[i]);
	}
	printf("in : %d, out : %d\n", cmd->fd_in,cmd->fd_out);
	printf("res : %d\n",cmd->result );
}

static int pid;
void handlerchld(int sig){
	kill(pid,SIGKILL);
}

void cmd_exec(Cmd* cmd){
	struct sigaction nvt,old;	
	memset(&nvt,0,sizeof(nvt));
		
	nvt.sa_handler = &handlerchld;
    sigaction(SIGINT,&nvt,&old);
	signal(SIGINT,&handlerchld);


	/*printf("%s\n",cmd->nom );*/


	/*int fdin[2]={cmd->fd_in,-1};
	int fdout[2]={-1,cmd->fd_out};*/
	int res=0;
	


	pid=fork();
	if(pid==-1){
		perror("Error fork execSimple");
		exit(EXIT_FAILURE);
	}
	if(!pid){
		int res=0;
		if(cmd->fd_in>=0){
			dup2(cmd->fd_in,0);
		}
		if(cmd->fd_out>=0){
			dup2(1,2);
			dup2(cmd->fd_out,1);
		}

		if(strcmp("pwd",cmd->nom) && strcmp("exit",cmd->nom) && strcmp("cd",cmd->nom)){
			execvp(cmd->nom,cmd->arguments);
			return;
		}else{
			if(strcmp("pwd",cmd->nom) == 0){
				res=command_pwd();
			}else if(strcmp("cd",cmd->nom) == 0){
				res=command_cd(cmd->arguments[1]);
			}
			exit(res);
		}

	}else{
		waitpid(pid,&(cmd->result),0);
		close(cmd->fd_in);
		if(cmd->result==0 && strcmp("cd",cmd->nom)==0){
			command_cd(cmd->arguments[1]);
		}else if(strcmp("exit",cmd->nom) == 0){
			res=command_exit();
		}
		
	}

	res=cmd->result;
	/* SIGKILL */
	/* A tester s'il s'agit d'un sigkill ou non */
	if(res == 2){
		res = 0;
		printf("CTRL-D arrêt du processus en cours\n");
	}else if(res== 9){
		res = 0;
		printf("CTRL-D arrêt du processus en cours\n");
	}else if(res){
		printf("res : %d\n",res );
		perror("Error exec");
	}
}




