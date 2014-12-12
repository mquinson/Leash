#include "main.h"



void handlerQuit(int sig){
	command_exit();
}

static char* getSetHome(char* h){
	static char* home=NULL;
	if(home==NULL && home!=NULL){
		home = (char*)leash_malloc(sizeof(char)*strlen(h)+1);
		strcpy(home,h);
	}
	return home;
}

static Meta* leashmeta;
int main(int argc,char* argv[]){

	if(argc==3){
		if(!strcmp(argv[1],"-C")){
			int err=meta_crypt(argv[2]);
			if(err){
				printf("Erreur cryptage du fichier meta [%s]\n",argv[2]);
				return EXIT_FAILURE;
			}else{
				printf("Fichier meta [%s] crypté avec succes\n",argv[2]);
				return EXIT_SUCCESS;
			}
		}
	}

	struct sigaction nvt,old;	
	memset(&nvt,0,sizeof(nvt));

	nvt.sa_handler = &handlerQuit;
	sigaction(SIGQUIT,&nvt,&old);
	signal(SIGQUIT,&handlerQuit);
	
	rl_attempted_completion_function = leash_completion;

	/* variables */
	int find=0;
	FILE* fichier_tar;
	
	/* check params */ 

	if(argc != 2){
		printf("Problem launching main \nUsage : ./run level.tgz\n");
		return EXIT_FAILURE;
	} 

	fichier_tar = fopen(argv[1],"r");

	if (fichier_tar == NULL){
		die("Problem with main, file to untar not exist");
	}
	fclose(fichier_tar);


	/* get fichier_tar name */
	
	char* rep_name = get_tar_name(argv[1]);
	
	/*printf("%s\n",rep_name);*/
	
	/* get home, repertoire_leash and repertoire_tmp path */

	char* home = getenv("HOME");
	char* repertoire_leash = get_env_leash(home);
	char* repertoire_level = get_env_level(repertoire_leash,rep_name);
	

	/*printf("%s\n",repertoire_level);*/
	/* Check / Create leash hidden directory in user home */

	create_leash_directory(home,repertoire_leash,repertoire_level);

	/* untar tar file */
	
	if(untar(argv[1],repertoire_level) == 0) {
		/*printf("fichier untar!\n");*/
	}
	

	getSetHome(repertoire_level);

	command_cd(repertoire_level);

	leashmeta = meta_init(repertoire_level);

	/* init structures, ... */

	/* Welcome message */
	printf("Bienvenue dans leaSh, le shell pour les nuls et les nulles :P\n");


	/* show authorised commands */
	meta_print(leashmeta);

	/* prog loop */
	while(!find){
		

		char* ligne = NULL;


		ligne = readline("\033[31m$\033[0m:");
        	rl_bind_key('\t',rl_complete);
        
		if(ligne==NULL){
			command_exit();
		}else{
			if (strlen(ligne)!=0){
            			add_history(ligne);
        		}

			ligne = trim(ligne);
			if(strlen(ligne)==0){
		
			}else{
				/*int lenResult = strlen(meta->answer);
				char* resultat=(char*)leash_malloc(sizeof(char*)*(lenResult+1));
				memset(resultat,0,lenResult+1);*/

				/* execute */

				int compteur = 0;
				Exec* exec = exec_init(leashmeta,ligne);
				if(exec == NULL){
					printf("La commande entrée n'est pas autorisée.\n");
				}else{
					exec_execute(exec);


					char c[1];
					int ok=0;
					int line=0;
					while(read(exec->fd_out,c,1)){
						if(c[0]=='\n'){
							line++;
							if(ok){
								find=1;
							}
							ok=0;
							compteur=0;
						}else{
							if(leashmeta->answer[compteur] != c[0] ){
								ok=0;
							}else{
								ok=1;
								compteur++;	
							}
							
						}
	
					}
		    			/* replace \n return value by \0 */
					/*resultat[compteur-1]='\0';*/
				}
	
				exec_dest(exec);

				/* check result */
				/*printf("res : [%s]\n",resultat );
				if(strcmp(resultat,meta->answer)==0){
					find = 1;
				}*/
			}
		}
		free(ligne);
		

	}
	printf("\n\033[32m>>>>>>>>>> Vous avez trouvé, BRAVO !!! <<<<<<<<<<\033[0m\n");



	/* Cleanning*/
	free(repertoire_level);


	return EXIT_SUCCESS;
}

static char** leash_completion( const char * text , int start,  int end){
	char **matches;

	matches = (char **)NULL;

	if (start == 0){
		matches = rl_completion_matches ((char*)text, &leash_generator);
	}else{
		rl_bind_key('\t',rl_abort);
	}
	return (matches);

}

char* leash_generator(const char* text, int state){
	static int list_index, len;
	char *name;

	if (!state) {
		list_index = 0;
		len = strlen (text);
	}
	

	while( (name= (liste_get(leashmeta->allowed, list_index)==NULL ? NULL : ((char*)(liste_get(leashmeta->allowed, list_index)->object))))){
		list_index++;
		if (strncmp (name, text, len) == 0){
			return (dupstr(name));
		}
	}

	return ((char *)NULL);

}




char* leashHome(){
	return getSetHome(NULL);
}

