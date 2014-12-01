#include "commands_implemented.h"

int pwd(){
	char* cwd;
    char buff[PATH_MAX + 1];
    cwd = getcwd( buff, PATH_MAX + 1 );
    if( cwd != NULL ) {
        printf( "My working directory is %s\n", cwd );
    }
    return 0;
}

int cd(char* path){
	/* TODO verifier chemin */
	int res=chdir(path);
	return res;
}

int make_exit(){
	die("Fin du programme LeaSh");
	return EXIT_SUCCESS;
}



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
}
