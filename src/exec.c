#include "exec.h"

Exec* exec_init(Meta* meta,char* exec){

	char* execCpy=(char*)malloc(sizeof(char)*(strlen(exec)+1));
	strcpy(execCpy,exec);

	int i=0;
	int length=strlen(execCpy);
	int nbCmd=1;
	for(i=0;i<length;i++){
		if((execCpy[i]=='|' && execCpy[i+1]!='|' && execCpy[i-1]!='|') || (execCpy[i]=='&' && execCpy[i+1]=='&') || (execCpy[i]=='|' && execCpy[i+1]=='|') ){
			nbCmd++;
		}
	}
	char** tabCmdStr=(char**)malloc(sizeof(char*)*nbCmd);
	int* link=(int*)malloc(sizeof(int)*nbCmd);
	
	int next=0;
	tabCmdStr[0]=execCpy;
	int nb=0;
	for(i=0;i<length;i++){
		if(next){
			next=0;
			tabCmdStr[nb] = &execCpy[i];
		}else if(execCpy[i]=='|' && execCpy[i+1]!='|' && execCpy[i-1]!='|'){
			execCpy[i]='\0';
			link[nb]=EXEC_PIPE;
			nb++;
			next=1;
		}else if(execCpy[i]=='&'){
			if(i+1<length && execCpy[i+1]=='&'){
				next=1;
				execCpy[i]='\0';
				execCpy[i+1]='\0';
				link[nb]=EXEC_AND;
				nb++;
				i++;
			}
		}else if(execCpy[i]=='|'){
			if(i+1<length && execCpy[i+1]=='|'){
				next=1;
				execCpy[i]='\0';
				execCpy[i+1]='\0';
				link[nb]=EXEC_OR;
				nb++;
				i++;
			}
		}

	}
	link[nb]=EXEC_NONE;



	Exec* execut=(Exec*)malloc(sizeof(Exec));
	execut->commands=(Cmd**)malloc(sizeof(Cmd*)*nbCmd);
	for(i=0;i<nbCmd;i++){

	

		Cmd* cmd = cmd_init(tabCmdStr[i]);
		
		if(meta_is_allowed(meta,cmd->nom)){
			execut->commands[i]=cmd;
		}else{
			return NULL;
		}
	}
	execut->link=link;
	execut->nbCommands=nbCmd;


	free(tabCmdStr);
	free(execCpy);

	return execut;

}

void exec_dest(Exec* exec){
	int i=0;
	for(i=0;i<exec->nbCommands;i++){
		cmd_dest(exec->commands[i]);
	}
	free(exec->commands);
	free(exec->link);
	free(exec);

}


void exec_execute(Exec* exec){
	int fd[2];
	int i=0;
	pipe(fd);

	exec->fd_out=fd[0];
	for(i=0;i<exec->nbCommands;i++){
		if(exec->link[i]==EXEC_AND || exec->link[i]==EXEC_OR || exec->link[i]==EXEC_NONE ){
			if(exec->commands[i]->fd_out==-1){
				exec->commands[i]->fd_out=fd[1];
			}
		}
	}


	int fdPipe[2];
	for(i=0;i<exec->nbCommands;i++){
		if(i>0 && exec->link[i-1]==EXEC_PIPE && exec->commands[i]->fd_in==-1){
			exec->commands[i]->fd_in=fdPipe[0];
		}

		switch(exec->link[i]){
			case EXEC_NONE:
				cmd_exec(exec->commands[i]);
				break;
			case EXEC_PIPE:
				pipe(fdPipe);
				if(exec->commands[i]->fd_out==-1){
					exec->commands[i]->fd_out=fdPipe[1];
				}
				cmd_exec(exec->commands[i]);
				close(fdPipe[1]);
				break;
			case EXEC_OR:
				cmd_exec(exec->commands[i]);
				if(exec->commands[i]->result==0){
					close(fd[1]);
					return;
				}
				break;
			case EXEC_AND:
				cmd_exec(exec->commands[i]);
				if(exec->commands[i]->result!=0){
					close(fd[1]);
					return;
				}

				break;
			default:
				return;
		}
	}
	close(fd[1]);

}
