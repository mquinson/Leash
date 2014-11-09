#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>




int checkWritingFolder(char* path);
int untar(char* tarPath, char* untarPath);


#endif