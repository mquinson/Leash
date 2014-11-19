#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cmd.h"

typedef enum{
	EXEC_NONE,
	EXEC_PIPE,
	EXEC_OR,
	EXEC_AND
}EXEC_MODE;

typedef struct {
	Cmd** commands;
	int* link;
	int nbCommands;
	int fd_out;
}Exec;


Exec* exec_init(char* exec);
void exec_dest(Exec* exec);
void exec_execute(Exec* exec);

#endif