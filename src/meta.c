#include "meta.h"

Meta* meta_init(char* path){
	Meta* meta = (Meta*)leash_malloc(sizeof(Meta));

	char* metaPath = (char*)leash_malloc(sizeof(char)*strlen(path)+strlen("/meta")+strlen(".crypt")+1);
	strcpy(metaPath,path),
	strcat(metaPath,"/meta");

	meta->answer=(char*)NULL;
	meta->allowed = liste_init();

	int crypted=0;
	FILE* metaFile = fopen(metaPath,"r");
	if(metaFile==NULL){
		strcat(metaPath,".crypt");
		metaFile = fopen(metaPath,"r");
		if(metaFile==NULL){
			printf("Erreur ouverture fichier meta\n");
			exit(EXIT_FAILURE);
		}
		crypted=1;
	}

	char line[1024];
	char* str;
	while (fgets(line, sizeof(line), metaFile)) {
		if(crypted){
			meta_decryptLine(line);
		}
		int i;
		for(i=0;i<strlen(line);i++){
			if(line[i]=='\n'){
				line[i]='\0';
				break;
			}
		}
		char* line2;
		if(strlen(line)>0){
			switch(line[0]){
				case '>':
					line2=trim(&line[1]);
					meta->answer=(char*)leash_malloc(sizeof(char)*(strlen(line2)+1));
					strcpy(meta->answer,line2);
					break;
				case '$':
					line2=trim(&line[1]);
					str=(char*)leash_malloc(sizeof(char)*(strlen(line2)+1));
					strcpy(str,line2);
					liste_add_last(meta->allowed,(void*)str);	
					break;
				case '#':
					break;
				default:
					printf("ERROR : '%s'\n",line);
					break;
			}
		}
	}

	char* strexit = (char*)leash_malloc(sizeof(char) * 5);
	strcpy(strexit,"exit");
	/*char* strpwd = (char*)leash_malloc(sizeof(char) * 4);
	strcpy(strpwd,"pwd");
	char* strcd = (char*)leash_malloc(sizeof(char) * 3);
	strcpy(strcd,"cd");*/
	liste_add_last(meta->allowed,(void*)strexit);	
	/*liste_add_last(meta->allowed,(void*)strpwd);
	liste_add_last(meta->allowed,(void*)strcd);*/

	fclose(metaFile);
	remove(metaPath);
	free(metaPath);
	return meta;
	
}

void meta_dest(Meta* meta){
	free(meta->answer);
	Elem* e=liste_tete(meta->allowed);
	Elem* ePrec=e;
	do{
		e=liste_suivant(meta->allowed,e);
		free(ePrec->object);
	}while(liste_a_un_suivant(meta->allowed,e));
	free(meta);
}

void meta_print(Meta* meta){
	printf("Vous pouvez utiliser : \n");
	Elem* e=liste_tete(meta->allowed);
	printf("- %s\n", (char*)(e->object));
	while(liste_a_un_suivant(meta->allowed,e)){
		e=liste_suivant(meta->allowed,e);
		printf("- %s\n", (char*)(e->object));
	}
}


int meta_is_allowed(Meta* meta, char* cmd){
	Elem* e=liste_tete(meta->allowed);
	if( strcmp((char*)e->object,cmd) == 0){
		return 1;
	}
	while(liste_a_un_suivant(meta->allowed,e)){
		e=liste_suivant(meta->allowed,e);
		if( strcmp((char*)e->object,cmd) == 0){
			return 1;
		}
	}
	return 0;

}


int meta_crypt(char* path){
	FILE* metaFile = fopen(path,"r");
	if(metaFile==NULL){
		printf("Erreur ouverture %s\n",path);
		return 1;
	}

	char* extention = ".crypt";
	char* pathCrypt = (char*)leash_malloc(sizeof(char)*(strlen(path)+strlen(extention)+1));
	strcpy(pathCrypt,path);
	strcat(pathCrypt,extention);
	FILE* metaCryptFile = fopen(pathCrypt,"w+");
	if(metaCryptFile==NULL){
		printf("Erreur creation %s\n",pathCrypt);
		return 2;
	}

	char c;
	while( (c=fgetc(metaFile))!= EOF ){
		if(c>='a' && c<='z'){
			c=97+ (((c-97)+ROT)%26);
		}else if(c>='A' && c<='Z'){	
			c=65+ (((c-65)+ROT)%26);
		}else if(c>='0' && c<='9'){	
			c=48+ (((c-48)+ROT)%26);
		}
		fputc(c,metaCryptFile);
	}
	fclose(metaFile);
	fclose(metaCryptFile);
	free(pathCrypt);
	return 0;

}

void meta_decryptLine(char* line){
	int len = strlen(line);
	int i;
	char c;
	for(i=0;i<len;i++){
		c=line[i];
		if(c>='a' && c<='z'){
			c=97+ (((c-97)-ROT+26)%26);
		}else if(c>='A' && c<='Z'){	
			c=65+ (((c-65)-ROT+26)%26);
		}else if(c>='0' && c<='9'){	
			c=48+ (((c-48)-ROT+26)%26);
		}
		line[i]=c;
	}
	printf("res '%s'\n",line );

}

