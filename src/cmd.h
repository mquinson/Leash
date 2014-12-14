#ifndef CMD_H
#define CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <glob.h>
#include <string.h>




#include "customlinkedlist.h"
#include "utils.h"
#include "commands_implemented.h"

typedef struct{
	/* pid permettant le kill lors d'un ctrl-C */
	pid_t pid;
	char* nom;
	int nbArgs;
	char** arguments;
	int result;
	/* booleen a vrai s'il y a une commande avec des backquote a l'interieur*/
	int backquoted;
	/* descripteur de fichier d'entre */
	int fd_in;
	/* descripteur de fichier de sortie */
	int fd_out;
	/* booleen a vrai si la sortie doit etre ecrit sur l'ecran */
	int print;
	/* index de la commande backquote dans le tableau d'argument */
	int backquoted_index;
	/* commande backquote */
	char* backquoted_cmd;
}Cmd;


Cmd* cmd_init(char str[]);
void cmd_dest(Cmd* cmd);
void cmd_exec(Cmd* cmd);
void cmd_print(Cmd* cmd);
#endif

