#include "main.h"

int main(int argc,char* argv[]){
	/* variables */
	int find=0;
	FILE* fichier_tar;

	/* get home, repertoire_leash and repertoire_tmp path */

	char* home = getenv("HOME");
        char* repertoire_leash = (char*)malloc((sizeof(home)) + 1000*(sizeof(char)));
        strcpy(repertoire_leash,home);
        strcat(repertoire_leash,"/.leaSh");
        char* repertoire_tmp = (char*)malloc((sizeof(home)) + 1000*(sizeof(char)));
        strcpy(repertoire_tmp,repertoire_leash);
        strcat(repertoire_tmp,"/tmp");

	/* check params */ 

	if(argc != 2){
                die("Problem launching main \nUsage : ./run level.tgz");
        } 

	printf("%s\n",argv[1]);
	fichier_tar = fopen(argv[1],"r");
 
	if (fichier_tar == NULL){
      		die("Problem with main, file to untar not exist");
	}


	/* Check / Create leash hidden directory in user home */
	create_leash_directory(home,repertoire_leash,repertoire_tmp);

	

	
	/* untar tar file */
	if(untar(argv[1],repertoire_tmp) == 0) {
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
