#include "commands_implemented.h"
#include "ananas.h"

int command_pwd(){
	char* cwd;
	char buff[PATH_MAX + 1];
	cwd = getcwd( buff, PATH_MAX + 1 );
	if( cwd != NULL ) {
		printf( "%s\n", cwd );
	}
	return 0;
}

int command_cd(char* path){
	static char* home=NULL;
	char* cwd;
	char buff[PATH_MAX+1];
	if(home==NULL){
		home=leash_malloc(sizeof(char)*(strlen(path)+1));
		strcpy(home,path);
	}
	
	int res=1;
	if(path==NULL){
		res=chdir(home);
	}else{ 

		char* str = trim(path);
		if(strlen(str)==0 || strcmp(str,"~")==0){
			res=chdir(home);
		}else{
			if(strlen(str)>0 && strcmp(str,"~")!=0){

				cwd = getcwd(buff,PATH_MAX+1);

				if((res = chdir(path)) == 0) {
					cwd = getcwd (buff,PATH_MAX+1);
					if((strlen(cwd) >= strlen(home)) && (strncmp(home,cwd,strlen(home)) == 0)){
					}else {
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

static WINDOW* printAbout(int starty, int startx){
	WINDOW *leash_win_about;
	leash_win_about = newwin(11, 40, 0, startx);
	mvwprintw(leash_win_about, 1,0,"%s",leash);
	mvwprintw(leash_win_about, 7,5,"GIANNINI && PERLINSKI");
	mvwprintw(leash_win_about, 9,8,"Projet RS 2014");
	wrefresh(leash_win_about);
	return leash_win_about;
}


static WINDOW* printAnanas(int i){
	WINDOW *leash_win_ananas;
	leash_win_ananas = newwin(35, 60, 2, (COLS/2)-25);

	int nb= i%6;
	nb = (nb==4 ? 2 : (nb==5 ? 1 : nb));

	mvwprintw(leash_win_ananas, 0,0,"%s",ananasTab[nb]);
	wrefresh(leash_win_ananas);
	return leash_win_ananas;
}


static void destroy_win(WINDOW *local_win){	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

int command_about(char* arg){
	WINDOW* leash_win_about;
	WINDOW* leash_win_ananas;
	char c=0;
	initscr();
	timeout(0);

	int i=0;

	struct timespec tim,tim2;
	if(strcmp(arg,"ananas")==0){
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



