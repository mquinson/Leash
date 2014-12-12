#ifndef UTILS_H
#define UTILS_H

/*#define _GNU_SOURCE*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include <sys/types.h>
#include <signal.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>

#include "cmd.h"

void die(const char* message);
int checkWritingFolder(char* path);
void create_leash_directory(char* home,char* repertoire_leash,char* repertoire_tmp);
int untar(char* tarPath,char* untarPath);
int tarSize(char* path);
void readWriteFD (int fdin,int fdout);
int nbWords(char* str);
char* trim(char* str);
char* get_env_leash(char* home);
char* get_env_level(char* repertoire_leash,char* name);
char* get_tar_name(char* tar);
void handlerchld(int sig);

char* dupstr (char*);
void* leash_malloc (int);
int leash_close(int fd);

#endif
