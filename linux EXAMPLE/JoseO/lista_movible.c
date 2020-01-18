#include <stdio.h>
#include <stdlib.h>

unsigned char numListas = 5;
typedef struct{
    char dato; 
    struct nodo* ptrNext;
} nodo;

nodo* header =NULL;
nodo* footer = NULL;

nodo* new();
void insert(nodo* Nodo);
void run();
void izquierda();
void derecha();

int main(void){
   unsigned char i = 0,  mover = 0,  flecha = '_';
    system("clear");

   for (i=0; i<=numListas; i++){
       nodo* nuevo = new();
       nuevo->dato = i;
       insert(nuevo);
   }
   printf("\t--> Estructura Autoreferenciada. <--\r\n\n");
   while(1){
        if(mover == 's'){
            printf("\t--> Se movio a la Derecha.\r\n\r\n");
            derecha();
        }
        if(mover == 'a'){
            printf("\t<-- Se movio a la Izquierda.\r\n\r\n");
            izquierda();
        }
       if(header->dato < 10)  for(char f =0; f<3; f++) printf("%c", flecha);
       else for(char f =0; f<4; f++) printf("%c", flecha);
       printf("\n");
       run();
       printf(" null.\r\n");
       mover = 0;
       do {
            printf("\r\n\t a) Para Anterior.\r\n\t s) Para Siguiente.\r\n\t e) Para Exit. \r\n"); 
            printf(" \tMove: ");
            scanf(" %c",&mover);
            if(mover == 'e') break; //salida de emergencia.
        } while(mover != 's' && mover != 'a');

        if (mover == 'e')break;   //salida de emergencia.
        system("clear");
   }
   system("clear");
   printf("\r\n\t--> Exit Complete. \r\n\r\n");
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
void insert(nodo* Nodo){
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
  // printf("%d ->  ",  i->dato);
    while(i != NULL){
        printf(" %d ->  ",  i->dato);
        i = i->ptrNext;
    }
}
//derecha.
void derecha(){
    nodo* remanente =NULL;
    remanente = header;
    nodo* nuevoNodo = new();
    nuevoNodo->dato = remanente->dato;
    insert(nuevoNodo);
    header = remanente->ptrNext; 
}
//izquierda
void izquierda(){
   nodo* remanente =NULL;
   nodo* anteFooter =NULL; 
   int dato = 0;
   remanente = footer;
    footer->ptrNext = NULL;
   nodo* nuevoNodo = new();
   nuevoNodo->dato = remanente->dato;
   nuevoNodo->ptrNext = header;
   anteFooter = nuevoNodo;
   while(1){
        anteFooter = anteFooter->ptrNext;
        if (anteFooter->ptrNext == remanente) {
            anteFooter->ptrNext =NULL;
            footer = anteFooter;
            break;
        }
   }
   header = nuevoNodo;
}

/*
void izquierda(){
   nodo* i =NULL;
   //nodo* x =NULL;
   int dato = 0;
   i = footer;
    footer->ptrNext = NULL;
   nodo* nuevoNodo = new();
   nuevoNodo->dato = i->dato;
   nuevoNodo->ptrNext = header;
   footer = nuevoNodo;
   while(1){
        footer = footer->ptrNext;
        if (footer->ptrNext == i) break;
   }
   header = nuevoNodo;
}*/