#ifndef COMMANDS_IMPLEMENTED
#define COMMANDS_IMPLEMENTED

#include <stdio.h>
#include <stdlib.h>

#include <linux/limits.h>
#include <unistd.h>

#include "utils.h"

int pwd();
int cd(char* path);
int make_exit();
int commands_is_implemented(char* cmd);
#endif