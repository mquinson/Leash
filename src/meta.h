#ifndef META_H
#define META_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "customlinkedlist.h"
#include "utils.h"



typedef struct{
	List* allowed;
	char* answer;
}Meta;

Meta* meta_init(char* path);
void meta_dest(Meta* meta);

void meta_print(Meta* meta);
int isAllowed(char* cmd);

#endif