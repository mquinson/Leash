#include "main.h"

static Meta* leashmeta;


/* Handler permettant d'associer la commande_exit a un signal */
void handlerQuit(int sig){
	command_exit();
}


/* Fonction permettant de mettre en place le home du programme */
static char* getSetHome(char* h){
	/* on définit la variable home en tant que variable statique */
	static char* home=NULL;
	if(home==NULL && home!=NULL){
		home = (char*)leash_malloc(sizeof(char)*strlen(h)+1);
		strcpy(home,h);
	}
	return home;
}


int main(int argc,char* argv[]){



	/* Cryptage */
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



	/* mise en place et instanciation des structures sigactions pour remplacer le comportements des signaux qui arrive */

	struct sigaction nvt,old;	
	memset(&nvt,0,sizeof(nvt));
	nvt.sa_handler = &handlerQuit;
	sigaction(SIGQUIT,&nvt,&old);
	signal(SIGQUIT,&handlerQuit);
	

	/* on associe leash_completion a readline */
	rl_attempted_completion_function = leash_completion;

	/* variables */
        /* la variable find permet de voir si la reponse suite a une commande de l'user est correcte */
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
	
	/* get home, repertoire_leash and repertoire_tmp path */

	char* home = getenv("HOME");
	char* repertoire_leash = get_env_leash(home);
	char* repertoire_level = get_env_level(repertoire_leash,rep_name);
	
	/* Check / Create leash hidden directory in user home */

	create_leash_directory(home,repertoire_leash,repertoire_level);

	/* untar tar file */
	
	if(untar(argv[1],repertoire_level)) {
		printf("Erreur lors de l'extraction de l'archive\n");
		return EXIT_FAILURE;
	}
	
	/* on note le home du programme via la fonction prevue plus haut*/
	getSetHome(repertoire_level);
        /* on change ensuite de repertoire pour se mettre dans le home */
	command_cd(repertoire_level);
        /* on initialise ensuite leashmeta en lisant le meta present dans le home du programme */
	leashmeta = meta_init(repertoire_level);

	/* Welcome message */
	printf("Bienvenue dans leaSh, le shell pour les nuls et les nulles :P\n");


	/* show authorised commands */
	meta_print(leashmeta);

	/* prog loop */
	while(!find){
		
                /* ligne represente la ligne tapee par un utilisateur */
		char* ligne = NULL;

                /* on utilise la fonction readline pour recuperer la ligne ecrite sur l'entree standard */
		ligne = readline("\033[31m$\033[0m:");
                /* on associe la touche tabulation a l'action d'autocompletion */
        	rl_bind_key('\t',rl_complete);
        

                /* si readline() renvoie null, rien n'a ete tappe par l'user et un ctrl-d a ete detecte */
		if(ligne==NULL){
                        /* on ferme donc le programme */
			command_exit();
		}else{
                        /* si la longueur de la ligne est differente de 0, l'user a ecrit quelque chose et c'est donc ajoute a l'historique */
			if (strlen(ligne)!=0){
            			add_history(ligne);
        		}

                        /* on enleve les espaces en trop */
			ligne = trim(ligne);
                        
			if(strlen(ligne)>0){

				/* on instancie Exec et on execute */

				int compteur = 0;
				Exec* exec = exec_init(leashmeta,ligne);
				if(exec == NULL){
					printf("La commande entrée n'est pas autorisée.\n");
				}else{
					exec_execute(exec);

                                        /* on recupere le resultat */
					char c[1];
					int ok=0;
					int line=0; 
                                        
                                        /* tant que le buffer n'est pas vide */
					while(read(exec->fd_out,c,1)){
						if(c[0]=='\n'){
                                                        /* on teste si la reponse fournie est en multiligne */
							line++;
							if(ok){
								compteur++;
							}					
							compteur++;
							
						}else{
							if(leashmeta->answer[compteur] != c[0] ){
								ok=0;
							}else{
								ok=1;
								compteur++;	
							}
							
						}
	
					}

                                        /* Si cela correspond */
					if(ok) {
						find = 1;
					}
		    			/* find passe a 1 */
				}

				/* on libere la variable exec et donc toutes les commandes stockees dans exec */
				exec_dest(exec);
			}
		}
                /*on libere la ligne */
		free(ligne);
		

	}
        /* si find est passe a 1 on a trouve le resultat */
	printf("\n\033[32m>>>>>>>>>> Vous avez trouvé, BRAVO !!! <<<<<<<<<<\033[0m\n");



	/* Cleaning*/
	free(repertoire_level);


	return EXIT_SUCCESS;
}



/* Fonctions permettant de mettre en place l'autocompletion */
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

