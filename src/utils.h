#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>

#define EXEC_NO_FLAG	0
#define EXEC_PIPE_SON 	1
#define EXEC_WAIT_SON	2



int checkWritingFolder(char* path);
int untar(char* tarPath, char* untarPath);
int tarSize(char* path);
int execSimple(char* cmd, char* args[], int in[2], int out[2], int flags);
void readWriteFD (int fdin,int fdout);

#endif
