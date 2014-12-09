#ifndef CUSTOMLINKEDLIST_H
#define CUSTOMLINKEDLIST_H


#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

typedef struct element{
	void* object;
	struct element* precedent;
	struct element* suivant;
} Elem;

typedef struct custom{
	Elem* tete;
	Elem* queue;
	int size;
} List;

typedef void (* FonctionFree)(void* e);

List* liste_init();
void liste_dest(List* l, FonctionFree fct_free);


void liste_add_last(List* l, void* obj);

Elem* liste_tete(List* l);
Elem* liste_get(List* l, int nb);
int liste_a_un_suivant(List* l, Elem* el);
Elem* liste_suivant(List* l, Elem* el);
void liste_supprime(List* l, Elem* el);

void liste_free(List* l, FonctionFree fct_free);

Elem* elem_init(void* obj);
void elem_dest(Elem* el);

#endif
