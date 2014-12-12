#include "utils.h"


void die(const char* message){
	perror(message);
	exit(EXIT_FAILURE);
}

char* get_tar_name(char* tar){
	char* str = (char*)leash_malloc(strlen(tar)+1);
	char* svg = str;
	strcpy(str,tar);
	char* token;
	token = strtok(str,"./");
	char* res=(char*)leash_malloc(sizeof(char)*strlen(token)+1);
	strcpy(res,token);
	free(svg);
	return res;
}


char* get_env_leash(char* home){
	char* repertoire_leash = (char*)leash_malloc((strlen(home))+(1000*sizeof(char)));
	strcpy(repertoire_leash,home);
	strcat(repertoire_leash,"/.leaSh");
	return repertoire_leash;
}

char* get_env_level(char* repertoire_leash,char* name){
	char* repertoire_level = (char*)leash_malloc((strlen(repertoire_leash)+strlen("/")+strlen(name)+1) * sizeof(char));
	strcpy(repertoire_level,repertoire_leash);
	strcat(repertoire_level,"/");
	strcat(repertoire_level,name);
	return repertoire_level;
}



int checkWritingFolder(char* path){
	DIR* dir;
	dir=opendir(path);
	int res =dir!=NULL;
	closedir(dir);
	return res;
}

void create_leash_directory(char* home,char* repertoire_leash,char* repertoire_level){
	DIR* dir =opendir(repertoire_leash); 
	if (dir==NULL){
        mkdir(repertoire_leash,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	}else{
		closedir(dir);
	}

	dir=opendir(repertoire_level);
	if(dir==NULL){
        mkdir(repertoire_level,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	}else{
		closedir(dir);
	}
}

int untar(char* path,char* untarPath){
	
	if(checkWritingFolder(untarPath)){
		char* str = (char*)leash_malloc(strlen("tar -zxf ")+strlen(path)+strlen(" -C ")+strlen(untarPath)+10);
		sprintf(str,"tar -zxf %s -C %s",path, untarPath);
		Cmd* cmdUntar = cmd_init(str);
		free(str);
		cmd_exec(cmdUntar);
		if(cmdUntar->result){
			printf("Erreur untar\n");
			exit(1);
		}
		cmd_dest(cmdUntar);
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


char * dupstr (char* s) {
  char* r;
  r = (char*) leash_malloc ((strlen (s) + 1));
  strcpy (r, s);
  return (r);
}
 
void* leash_malloc (int size){
    void *buf;
 
    buf = malloc (size);
    if (!buf) {
        fprintf (stderr, "Error: Out of memory. Exiting.'n");
        exit (1);
    }
    return buf;
}

int leash_close(int fd){
	int res=0;
	if(fd>=0){
		res=close(fd);
	}
	return res;
}