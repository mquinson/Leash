#ifndef MAIN_H
#define MAIN_H
/*#define _GNU_SOURCE*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <sys/stat.h>
#include <signal.h>

#include <sys/types.h>
#include <linux/limits.h>

#include "exec.h"
#include "cmd.h"
#include "utils.h"
#include "commands_implemented.h"
#include "customlinkedlist.h"
#include "meta.h"

static char** leash_completion(const char*, int ,int);
char* leash_generator(const char*,int);


#endif
