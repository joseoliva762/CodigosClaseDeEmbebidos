#include <stdio.h>
#include <stdlib.h>

#define Derecha 77
#define Izquierda 75

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
   unsigned char i = 0,  mover = 0;
    system("clear");

   for (i=0; i<=4; i++){
       nodo* nuevo = new();
       nuevo->dato = i;
       insert(nuevo);
   }
   //int val = '^[[D';
   //printf("%d\r\n",val);
   printf("\t--> Estructura Autoreferenciada. <--\r\n\r\n");
   while(1){
        if(mover == Derecha){
            printf("\t--> Se movio a la Derecha.\r\n\r\n");
            derecha();
        }
        if(mover == Izquierda){
            printf("\t--> Se movio a la Izquierda.\r\n\r\n");
            izquierda();
            //footer->ptrNext 
        }
        run();
        printf(" null.");

        mover = 0;
        do {
                printf(" \r\n\tMove: ");
                mover = getch();  
          if(mover == 'e' || mover == '^[[D' || mover == 77) break; //salida de emergencia.
        } while();

      
        if (mover == 'e')break;   //salida de emergencia.
        system("clear");
   }
   printf("\r\n\tExit Complete. ");
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
   printf("%d ->  ",  i->dato);
   /* 
    while(i != NULL){
        printf("%d ->  ",  i->dato);
        i = i->ptrNext;
    }
*/
}
//derecha.
void derecha(){
    nodo* i =NULL;
    i = header;
    nodo* nuevoNodo = new();
    nuevoNodo->dato = i->dato;
    insert(nuevoNodo);
    header = i->ptrNext; 
}

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
}