#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>



#define EXEC_NO_FLAG	0
#define EXEC_PIPE_SON 	1
#define EXEC_WAIT_SON	2


void die(const char* message);
int checkWritingFolder(char* path);
void create_leash_directory(char* home,char* repertoire_leash,char* repertoire_tmp);
int untar(char* tarPath,char* untarPath);
int tarSize(char* path);
int execSimple(char* cmd, char* args[], int in[2], int out[2], int flags);
void readWriteFD (int fdin,int fdout);
int nbWords(char* str);

char* trim(char* str);

#endif
