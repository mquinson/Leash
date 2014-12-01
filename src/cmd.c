#include "cmd.h"



Cmd* cmd_init(char str[]){
	Cmd* cmd=(Cmd*)malloc(sizeof(Cmd));
	printf("str : %s\n",str );

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
				printf(">\n");
				if(cmd->fd_out!=-1){
					printf("Seulement un seul > ,");
				}
				action=1;
				file_out=1;
			}
			if(!strcmp(">>",token)){
				printf(">>\n");
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

void cmd_exec(Cmd* cmd){
	printf("%s\n",cmd->nom );
	int fdin[2]={cmd->fd_in,-1};
	int fdout[2]={-1,cmd->fd_out};
	int res=0;
	if(!commands_is_implemented(cmd->nom)){
		res=execSimple(cmd->nom, cmd->arguments,fdin,fdout,EXEC_WAIT_SON|EXEC_PIPE_SON);
	}else{
		 if(strcmp("pwd",cmd->nom) == 0){
    		res=pwd();
	    }

	    if(strcmp("exit",cmd->nom) == 0){
	    	res=make_exit();
	    }

	    if(strcmp("cd",cmd->nom) == 0){
	    	res=cd(cmd->arguments[1]);
	    }
	}

	cmd->result=res;
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
		exit(1);
	}
}




