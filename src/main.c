#include "main.h"

int main(int argc,char* argv[]){
	/* variables */
	int find=0;
	FILE* fichier_tar;

	/* check params */ 

	if(argc != 2){
		die("Problem launching main \nUsage : ./run level.tgz");
	} 

	printf("%s\n",argv[1]);
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
	
	/* init structures, ... */

	/* Welcome message */
	printf("Bienvenue dans leaSh, le shell pour les nuls et les nulles :P\n");


	/* show authorised commands */
	printf("Vous pouvez utiliser : ...\n");


	/* prog loop */
	while(!find){
		/* show $ */

		/* collect commands */

		/* execute */

		/* check result */

		find=1;
	}
	printf("Vous avez trouvé, BRAVO !!!\n");

	return EXIT_SUCCESS;
}
