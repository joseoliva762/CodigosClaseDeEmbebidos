#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char dato; 
    struct nodo* ptrNext;
} nodo;

nodo* header =NULL;
nodo* footer = NULL;

nodo* new();
void insert(nodo* Nodo);
void run();

int main(void){
    /*
    nodo* primero = new();
    primero->dato = 5; //flechitas es direccion. el punto llamar la variable.
    nodo* segundo  = new();
    segundo->dato = 10;
    insert(primero);
    insert(segundo);
    */
   unsigned char i = 0;
   for (i=0; i<=50; i++){
       nodo* nuevo = new();
       nuevo->dato = i;
       insert(nuevo);
   }
    run();
    printf(" FIN.");
    return EXIT_SUCCESS;
}

//funciones

nodo* new(){
    nodo* nuevo = malloc(sizeof(nodo));
    nuevo->ptrNext = NULL;
    return(nuevo);
    //esta funcion crea un bloque con espacio de memoria y un puntero que aputa a NULL.
}

//para insertar
void insert(struct nodo* Nodo){
    if (header == NULL){
        header = (struct nodo*) Nodo;
        footer  = (struct nodo*) Nodo;
    }else{
        footer->ptrNext= (struct nodo*) Nodo;
        footer = Nodo;
    }
}

void run(){
    nodo* i = NULL;
    i = header;
    while(i != NULL){
        printf("%d ->  ",  i->dato);
        i = i->ptrNext;
    }
}