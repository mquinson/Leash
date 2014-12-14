#include "commands_implemented.h"
#include "ananas.h"

/* commande personnalisee pwd */
int command_pwd(){
	char* cwd;
	char buff[PATH_MAX + 1];
	cwd = getcwd( buff, PATH_MAX + 1 );
	if( cwd != NULL ) {
		printf( "%s\n", cwd );
	}
	return 0;
}

/* commande personnalisee cd */
int command_cd(char* path){
        /* la variable home est definie en tant que static pour ne pas qu'elle change au cours de l'utilisation du programme */
	static char* home=NULL;
        /* on get le current working directory */
	char* cwd;
	char buff[PATH_MAX+1];
	if(home==NULL){
		home=leash_malloc(sizeof(char)*(strlen(path)+1));
		strcpy(home,path);
	}
	
	int res=1;
        /* si le path est null, la commande tappee par l'user est "cd" sans arguments donc on redirige vers le home */
	if(path==NULL){
		res=chdir(home);
	}else{ 

		char* str = trim(path);
                /* on trim le path pour plus de precautions et on regarde si la longueur est toujours de 0 ou si l'user a tappe cd ~ pour retourner dans le home du programme */
		if(strlen(str)==0 || strcmp(str,"~")==0){
			res=chdir(home);
		}else{
			if(strlen(str)>0 && strcmp(str,"~")!=0){

				cwd = getcwd(buff,PATH_MAX+1);
                                /* on get le cwd */
				if((res = chdir(path)) == 0) {
					cwd = getcwd (buff,PATH_MAX+1); 
                                        /* on regarde si le path tappe est contenu dans le home suivant */
					if((strlen(cwd) >= strlen(home)) && (strncmp(home,cwd,strlen(home)) == 0)){
					}else {
                                                /* sinon on revient vers le home */
						res = chdir(home);
					}

				}else{
					printf("Le chemin n'est pas accessible\n");
				}

				cwd = getcwd(buff,PATH_MAX+1);
			}
		}
	}
	return res;
}

int command_exit(){
	printf("Fin du programme LeaSh\n");
	exit(EXIT_SUCCESS);
}


/*****************/
/* ABOUT NCURSES */
/*****************/

static WINDOW* printAbout(int starty, int startx);
static WINDOW* printAnanas(int i);
static void destroy_win(WINDOW *local_win);


static char* leash = 
"  _                 _____ _\n"
" | |               / ____| |\n"
" | |     ___  __ _| (___ | |__\n"
" | |    / _ \\/ _` |\\___ \\| '_ \\\n"
" | |___|  __/ (_| |____) | | | |\n"
" |______\\___|\\__,_|_____/|_| |_|";

/* permet d'afficher LeaSh avec un deffilement horizontal */
static WINDOW* printAbout(int starty, int startx){
	WINDOW *leash_win_about;
	leash_win_about = newwin(11, 40, 0, startx);
	mvwprintw(leash_win_about, 1,0,"%s",leash);
	mvwprintw(leash_win_about, 7,5,"GIANNINI && PERLINSKI");
	mvwprintw(leash_win_about, 9,8,"Projet RS 2014");
	wrefresh(leash_win_about);
	return leash_win_about;
}

/* permet d'afficher un ananas dansant, easter egg :D */
static WINDOW* printAnanas(int i){
	WINDOW *leash_win_ananas;
	leash_win_ananas = newwin(35, 60, 2, (COLS/2)-25);

	int nb= i%6;
	nb = (nb==4 ? 2 : (nb==5 ? 1 : nb));

	mvwprintw(leash_win_ananas, 0,0,"%s",ananasTab[nb]);
	wrefresh(leash_win_ananas);
	return leash_win_ananas;
}

/* detruit la fenetre cree precedement afin de faire le deplacement */
static void destroy_win(WINDOW *local_win){	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}


/* 
	commande personnalise about
	utilise NCURSES pour faire une ptite animation
*/
int command_about(char* arg){
	WINDOW* leash_win_about;
	WINDOW* leash_win_ananas;
	char c=0;
	initscr();
	timeout(0);

	int i=0;

	struct timespec tim,tim2;
	if(strcmp(arg,"ananas")==0){
		/* si l'argument passe est 'ananas' alors on joue l'easter egg */
		while(1){
			c=getch();
			leash_win_ananas = printAnanas(i);
			tim.tv_sec  = 0;
			tim.tv_nsec = 150000000L;
			if(nanosleep(&tim , &tim2) < 0 ){
				break;
			}
			destroy_win(leash_win_ananas);
			i++;
			if(i==30 || c>0){
				fprintf(stdout, "ANANAS\n");
				break;
			}
		}
	}else{
		/* sinon on joue le definlement du leash*/
		while(1){
			c=getch();
			leash_win_about = printAbout( 0,i);
			tim.tv_sec  = 0;
			tim.tv_nsec = 150000000L;
			if(nanosleep(&tim , &tim2) < 0 ){
				break;
			}
			destroy_win(leash_win_about);
			i++;
			if(i==COLS-34 || c>0){
				break;
			}
		}
	}
	refresh();
	endwin();
	return 0;
}



