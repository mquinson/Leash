#include "main.h"

void handlerQuit(int sig){
	command_exit();
}

Meta* meta;
int main(int argc,char* argv[]){
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
	

	command_cd(repertoire_level);

	meta = meta_init(repertoire_level);

	/* init structures, ... */

	/* Welcome message */
	printf("Bienvenue dans leaSh, le shell pour les nuls et les nulles :P\n");


	/* show authorised commands */
	meta_print(meta);

	/* prog loop */
	while(!find){
		

		char* ligne = NULL;


		while((ligne = readline("\033[31m$\033[0m:"))!=NULL) {
	        //enable auto-complete
	        rl_bind_key('\t',rl_complete);
	        
	        if (ligne[0]!=0){
	            add_history(ligne);
	        }

			ligne = trim(ligne);
			if(strlen(ligne)==0){

			}else{
				char resultat[1024];
				int compteur = 0;
				memset(resultat,0,1024);

				/* execute */

				Exec* exec = exec_init(meta,ligne);
				if(exec == NULL){
					printf("La commande entrée n'est pas autorisée.\n");
				}else{
					exec_execute(exec);


					char c[1];
					/*printf("--------- RESULT %s ---------\n",ligne);*/
					while(read(exec->fd_out,c,1)){
						if(compteur < 1024){
							resultat[compteur] = c[0];
						}
						printf("%c",c[0] );
						compteur++;	
					}
	    			/* replace \n return value by \0 */
					resultat[compteur-1]='\0';
				}

				/* check result */
				if(strcmp(resultat,meta->answer)==0){
					find = 1;
				}
			}
		}

	}
	printf("\nVous avez trouvé, BRAVO !!!\n");

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
	while( (name= ((char*)(liste_get(meta->allowed, list_index)->object)))  ){
		list_index++;
		if (strncmp (name, text, len) == 0){
			return (dupstr(name));
		}
	}
	return ((char *)NULL);

}




