#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


int main(int argc, char *argv[]){
	int err=untar(argv[1],argv[2]);
	printf("err : %d\n", err);
	return 0;
}