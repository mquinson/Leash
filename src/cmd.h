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





#include "utils.h"
#include "commands_implemented.h"

typedef struct{
	pid_t pid;
	char* nom;
	int nbArgs;
	char** arguments;
	int result;
	int backquoted;
	int fd_in;
	int fd_out;
	int print;
}Cmd;


Cmd* cmd_init(char str[]);
void cmd_dest(Cmd* cmd);
void cmd_exec(Cmd* cmd);
void cmd_print(Cmd* cmd);
#endif

