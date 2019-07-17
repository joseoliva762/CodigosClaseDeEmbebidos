#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char data;
	struct nodo* nextPtr;
} nodo;

nodo* header = NULL;
nodo* footer = NULL;

nodo* new(){
	nodo* nuevo = malloc(sizeof(nodo));
	nuevo->nextPtr = NULL;
	return(nuevo);
}

void insert(nodo* Nodo){
	if(header == NULL){
		header = Nodo;
		footer = Nodo;
	}
	else{
		footer->nextPtr = Nodo;
		footer=Nodo;
	}
}

void run(){
	nodo* i = header;
	while(i!=NULL){
		printf("%d -> ", i->data);
		i = i->nextPtr;
	}
}

void mem_free(){
	nodo* temp = NULL;
	nodo* i = header;
	while(i != NULL){
		temp = i;
		i = i->nextPtr;
		free(temp);
	}
	header = NULL;
	footer = NULL;
}

int main(){
	unsigned char i = 0;
	for(i=0; i<10; i++){
		nodo* Nodo = new();
		Nodo->data = i;
		insert(Nodo);
	}
	run();
	mem_free();
	run();
	return(0);
}
