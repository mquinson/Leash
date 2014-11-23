#include "meta.h"

Meta* meta_init(char* path){
	Meta* meta = (Meta*)malloc(sizeof(Meta));

	char* metaPath = (char*)malloc(sizeof(char)*strlen(path)+1+strlen("/meta"));
	strcpy(metaPath,path),
	strcat(metaPath,"/meta");

	meta->answer=(char*)NULL;
	meta->allowed = liste_init();


	FILE* metaFile = fopen(metaPath,"r");
	
	char line[1024];
	char* str;
	while (fgets(line, sizeof(line), metaFile)) {
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
				meta->answer=(char*)malloc(sizeof(char)*(strlen(line2)+1));
				strcpy(meta->answer,line2);
				break;
				case '$':
				line2=trim(&line[1]);
				str=(char*)malloc(sizeof(char)*(strlen(line2)+1));
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

	fclose(metaFile);
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

int isAllowed(Meta* meta,char* cmd){
	
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
