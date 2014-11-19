#######################
##     VARIABLES     ##
#######################

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
TEST = leaShTest

#liste des programmes AJOUT ici
LISTE = $(DEST)utils.o $(DEST)cmd.o $(DEST)customlinkedlist.o $(DEST)exec.o


#######################
##      REGLES       ##
#######################


#definition de la cible a reconstruire
all : $(DEST)$(PROG)

#definition de la cible test a reconstruire
test : $(DEST)$(TEST)

#programme principal
$(DEST)$(PROG) : $(LISTE) $(DEST)main.o
	@rm -f $(DEST)$(TEST)
	@rm -f $(DEST)test.o 2>/dev/null
	$(CC) $(DEST)main.o $(LISTE) -o $(DEST)$(PROG)

#programme tests
$(DEST)$(TEST) : $(LISTE) $(DEST)test.o	
	@rm -f $(DEST)$(PROG)
	@rm -f $(DEST)main.o 2>/dev/null
	$(CC) $(DEST)test.o $(LISTE) -o $(DEST)$(TEST)

#compilation des fichiers (hors main et test)
allfiles : $(LISTE)


#######################
##    EXECUTABLES    ##
#######################

#compilation main
$(DEST)main.o : $(SRC)main.c 
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)main.c -o $(DEST)main.o

#compilation test
$(DEST)test.o : $(SRC)test.c
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)test.c -o $(DEST)test.o

#######################
##  NON EXECUTABLES  ##
#######################
## AJOUTER REGLES LA ##
#######################


#compilation utils
$(DEST)utils.o : $(SRC)utils.c 
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)utils.c -o $(DEST)utils.o

#compilation cmd
$(DEST)cmd.o : $(SRC)cmd.c
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)cmd.c -o $(DEST)cmd.o

#compilation exec
$(DEST)exec.o : $(SRC)exec.c
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)exec.c -o $(DEST)exec.o

#compilation customlinkedlist
$(DEST)customlinkedlist.o : $(SRC)customlinkedlist.c
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)customlinkedlist.c -o $(DEST)customlinkedlist.o

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
	@rm -f $(DEST)$(TEST)
