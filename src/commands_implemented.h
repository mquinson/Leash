#ifndef COMMANDS_IMPLEMENTED
#define COMMANDS_IMPLEMENTED

#include <stdio.h>
#include <stdlib.h>

#include <linux/limits.h>
#include <unistd.h>

#include "utils.h"

int command_pwd();
int command_cd(char* path);
int command_exit();
/*int commands_is_implemented(char* cmd);*/
#endif