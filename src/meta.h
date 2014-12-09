#ifndef META_H
#define META_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "customlinkedlist.h"
#include "utils.h"
#include "meta.h"

#define ROT 13

typedef struct{
	List* allowed;
	char* answer;
}Meta;

Meta* meta_init(char* path);
void meta_dest(Meta* meta);

void meta_print(Meta* meta);

int meta_is_allowed(Meta* meta, char* cmd);

int meta_crypt(char* path);
void meta_decryptLine(char* line);
#endif
