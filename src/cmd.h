#ifndef CMD_H
#define CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


typedef struct{
 pid_t pid;
 char* nom;
 char* arguments;
 int backquoted;
 int fd_in;
 int fd_out;
}Cmd;

Cmd* cmd_init(pid_t pid, char* nom, char* args, int backquoted, int fd_in, int fd_out);
void cmd_dest(Cmd* cmd);

#endif
