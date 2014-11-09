#include "utils.h"


int checkWritingFolder(char* path){
	/* TODO improve check */
	DIR* dir;
	dir=opendir(path);
	return dir!=NULL;
}

int untar(char* path, char* untarPath){
	if(checkWritingFolder(untarPath)){
		int pid=fork();
		if(pid==-1){
			return 1;
		}else if(!pid){
			printf("%s : %s\n", path, untarPath);
			execlp("tar","tar","-zxvf",path,"-C",untarPath,NULL);
			return 4;
		}else{
			int res=waitpid(pid,NULL,0);
			if(res==-1){
				perror("error waitpid");
				return 2;
			}
			return 0;
		}
	}else{
		printf("folder read_only/not exists\n");
		return 3;
	}
	return 0;
}


int tarSize(char* path){
	//TODO
	//tar tzvf archive.tar.gz | sed 's/ \+/ /g' | cut -f3 -d' ' | sed '2,$s/^/+ /' | paste -sd' ' | bc
	return 0;
}