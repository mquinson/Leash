#include "commands_implemented.h"

int command_pwd(){
	char* cwd;
	char buff[PATH_MAX + 1];
	cwd = getcwd( buff, PATH_MAX + 1 );
	if( cwd != NULL ) {
		printf( "%s\n", cwd );
	}
	return 0;
}

int command_cd(char* path){
	static char* home=NULL;
    char* cwd;
    char buff[PATH_MAX+1];
	if(home==NULL){
		home=leash_malloc(sizeof(char)*(strlen(path)+1));
		strcpy(home,path);
	}
	
	int res=1;
	if(path==NULL){
		res=chdir(home);
	}else{ 

		char* str = trim(path);
		if(strlen(str)==0 || strcmp(str,"~")==0){
			res=chdir(home);
		}else{
			if(strlen(str)>0 && strcmp(str,"~")!=0){

                cwd = getcwd(buff,PATH_MAX+1);
        
                    if((res = chdir(path)) == 0) {
                        cwd = getcwd (buff,PATH_MAX+1);
                        if((strlen(cwd) >= strlen(home)) && (strncmp(home,cwd,strlen(home)) == 0)){
                        }else {
                            res = chdir(home);
                        }

                    }else{
                        printf("Le chemin n'est pas accessible\n");
                    }
			    
                cwd = getcwd(buff,PATH_MAX+1);
			}
		}
	}
	return res;
}

int command_exit(){
	printf("Fin du programme LeaSh\n");
	exit(EXIT_SUCCESS);
}


/*
int commands_is_implemented(char* cmd){

    if(strcmp("pwd",cmd) == 0){
    	return 1;
    }

    if(strcmp("exit",cmd) == 0){
    	return 1;
    }

    if(strcmp("cd",cmd) == 0){
    	return 1;
    }
	return 0;
}*/
