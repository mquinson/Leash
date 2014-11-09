#definition du compilateur
CC = gcc

DIR = 
#$(shell pwd)/

SRC = $(DIR)src/

DEST = $(DIR)bin/

#specification des options du compilateur
CFLAGS = -ansi -Wall -g

#librairies
LIB = 

#nom de l executable
PROG = leaSh

#definition de la cible a reconstruire
all : directories $(DEST)$(PROG)

$(DEST)$(PROG) : $(DEST)main.o
	$(CC) $(DEST)main.o  -o $(DEST)$(PROG)

#compilation main
$(DEST)main.o : $(SRC)main.c 
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)main.c -o $(DEST)main.o



#########################################################################

#exemple edition de liens et production de l executable
#$(PROG) : fich.o fich2.o
#	$(CC) fich.o fich2.o  -o $(PROG)


#exemple compilation du fichier ListATree
#$(DEST)fich.o : $(SRC)fich.c 
#	$(CC) $(CFLAGS) $(LIB) -c $(SRC)fich.c -o $(DEST)fich.o

#########################################################################



#creation du dossier bin
directories:
	@mkdir -p $(DEST) 2>/dev/null

#destruction de tous les fichiers intermediaires crees
clean : 
	@rm -f $(DEST)*.o

#suppression de tout ce qui peut etre regenere
mrproper : clean
	@rm -f $(DEST)$(PROG)
