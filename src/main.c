#include "main.h"

int main(int argc,char* argv[]){
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
	
	printf("%s\n",rep_name);
	
	/* get home, repertoire_leash and repertoire_tmp path */

	char* home = getenv("HOME");
	char* repertoire_leash = get_env_leash(home);
	char* repertoire_level = get_env_level(repertoire_leash,rep_name);
	

	printf("%s\n",repertoire_level);
	/* Check / Create leash hidden directory in user home */

	create_leash_directory(home,repertoire_leash,repertoire_level);

	/* untar tar file */
	
	if(untar(argv[1],repertoire_level) == 0) {
		printf("fichier untar!\n");
	}
	
		
	cd(repertoire_level);

	Meta* meta = meta_init(repertoire_level);

	/* init structures, ... */

	/* Welcome message */
	printf("Bienvenue dans leaSh, le shell pour les nuls et les nulles :P\n");


	/* show authorised commands */
	meta_print(meta);

	/* prog loop */
	while(!find){
	
		char* ligne = NULL;
		size_t read;
		/* show $ */
		printf("$");
		/* collect commands */
		while(getline(&ligne,&read,0) == -1);
		line=trim(ligne);

		/* execute */

	        Exec* exec = exec_init(ligne);
       	 	exec_execute(exec);

        	char c[1];
        	printf("--------- RESULT %s ---------\n",ligne);
        	while(read(exec->fd_out,c,1)){
                	printf("%c",c[0] );
        
		}
		/* check result */

	

	
	}
	printf("Vous avez trouvé, BRAVO !!!\n");

	return EXIT_SUCCESS;
}
