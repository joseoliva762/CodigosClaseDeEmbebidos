#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char dato;
	struct nodo* ptrNext;
}nodo;

nodo* header = NULL;
nodo* footer = NULL;

nodo* new(){
	nodo* nuevo = malloc(sizeof(nodo));
	nuevo->ptrNext = NULL;
	return(nuevo);
}

void insert(nodo* Nodo){
    if(header == NULL){
    	header = Nodo;
    	footer = Nodo;
    }else{
    	footer->ptrNext = Nodo;
    	footer = Nodo;
    }
}

void run(){
	nodo* i = NULL;
	i = header;
	while(i != NULL){
		printf("%d->", i->dato);
		i = i->ptrNext;
	}
}

int main(void) {
	unsigned char i =0;
	for(i=0; i<50; i++){
		nodo* nuevo = new();
		nuevo->dato = i;
		insert(nuevo);
	}
	run();

	return EXIT_SUCCESS;
}
