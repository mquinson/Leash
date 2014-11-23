#include "commands_implemented.h"

void pwd(){
	char* cwd;
    char buff[PATH_MAX + 1];
    cwd = getcwd( buff, PATH_MAX + 1 );
    if( cwd != NULL ) {
        printf( "My working directory is %s.\n", cwd );
    }
}

int cd(char* path){
	/* TODO verifier chemin */
	int res=chdir(path);
	return res;
}