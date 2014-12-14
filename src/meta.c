#include "meta.h"

/* Fonction permettant d'initialiser la structure meta */
Meta* meta_init(char* path){

	/* on alloue meta une quantite d'espace en memoire */
	Meta* meta = (Meta*)leash_malloc(sizeof(Meta));
        /* on met le chemin approprie */
	char* metaPath = (char*)leash_malloc(sizeof(char)*strlen(path)+strlen("/meta")+strlen(".crypt")+1);
	strcpy(metaPath,path),
	strcat(metaPath,"/meta");

      
	meta->answer=(char*)NULL;
	meta->allowed = liste_init();
        
        /* on regarde si le fichier meta a ete crypte ou non */         
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
        
        /* on initialise notre ligne et la variable str */
	char line[1024];
	char* str;
	while (fgets(line, sizeof(line), metaFile)) {
                /* on regarde si meta est crypte */
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
                /* on reprends une deuxieme ligne qui sera cette fois un char* afin de la stocker dans les differents champs de meta */
		char* line2;
		if(strlen(line)>0){
			switch(line[0]){
                                /* on switch suivant le debut de la ligne */
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


        /* On ajoute manuellement les commandes de bases que l'utilisateur pourra tout le temps faire, quelque soit le fichier meta */
	char* strexit = (char*)leash_malloc(sizeof(char) * 5);
	strcpy(strexit,"exit");
	char* strabout = (char*)leash_malloc(sizeof(char) * 6);
	strcpy(strabout,"about");
	/*char* strpwd = (char*)leash_malloc(sizeof(char) * 4);
	strcpy(strpwd,"pwd");
	char* strcd = (char*)leash_malloc(sizeof(char) * 3);
	strcpy(strcd,"cd");*/
	liste_add_last(meta->allowed,(void*)strexit);
	liste_add_last(meta->allowed,(void*)strabout);	
	/*liste_add_last(meta->allowed,(void*)strpwd);
	liste_add_last(meta->allowed,(void*)strcd);*/

	
        /* on close et on supprime meta, pour eviter la fraude et les fuites memoires */
        fclose(metaFile);
	remove(metaPath);
	free(metaPath);
	return meta;
        
	
}

/* Destruction du fichier meta */
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


/* Affichage du contenue du fichier meta */
void meta_print(Meta* meta){
	printf("Vous pouvez utiliser : \n");
	Elem* e=liste_tete(meta->allowed);
	printf("- %s\n", (char*)(e->object));
	while(liste_a_un_suivant(meta->allowed,e)){
		e=liste_suivant(meta->allowed,e);
		printf("- %s\n", (char*)(e->object));
	}
}

/* Fonction qui regarde si la commande est autorisee ou non */
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


/* Fonction de cryptage du fichier meta */
int meta_crypt(char* path){
	FILE* metaFile = fopen(path,"r");
	if(metaFile==NULL){
		printf("Erreur ouverture %s\n",path);
		return 1;
	}
        /* on initialise les chemins du fichier meta.crypt */
	char* extention = ".crypt";
	char* pathCrypt = (char*)leash_malloc(sizeof(char)*(strlen(path)+strlen(extention)+1));
	strcpy(pathCrypt,path);
	strcat(pathCrypt,extention);
	FILE* metaCryptFile = fopen(pathCrypt,"w+");
	if(metaCryptFile==NULL){
		printf("Erreur creation %s\n",pathCrypt);
		return 2;
	}

        /* on regarde chaque caractere du fichier jusqu'a trouver un end of file */
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


/* Fonction permettant de decrypter un char* rentre en parametre*/
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

}

