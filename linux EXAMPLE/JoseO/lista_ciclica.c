#include <stdio.h>
#include <stdlib.h>

char numLista = 15, inicioLista = 0,reinc = 0, agg = 0,  pss = 0;
typedef struct{
    char dato, posicion;
    struct nodo* ptrNext;
    struct nodo* ptrPrev;
}nodo;

nodo* header = NULL;
nodo* footer = NULL;
nodo* selection = NULL;
nodo* marca = NULL;
nodo* new();
void insert(nodo* Nodo);
void run();
void explo(char mover);
void aggDato();

int main(void){
    while(1){
        fflush;
        char accion = 0, post = 1, nota = 0, access = 0, dat = 0, mn = 0;
        system("clear");
        do{
            printf("\t --> Recuerde que el inicio debe ser menor que el final. <--\r\n\r\n");
            printf("\r\nIngrese el numero de inicio de la lista: ");
            scanf("%d", &inicioLista);
            if(nota >=1) inicioLista -= 1;
            printf("\r\n");
            printf("Ingrese el final de la lista: ");
            scanf("%d", &numLista);
            if(inicioLista < numLista) access =1;
            system("clear");
        }while(access == 0);
        
        for ( int i= inicioLista; i<=numLista; i++){
            nodo* nuevo = new();
            nuevo->dato = i;
            insert(nuevo);
            nuevo->posicion = post;
            post++; 
        }
        
        while(1){
            system("clear");
            if(accion == 0 || accion == 'f' && accion == 'h') printf("\t --> Lista Ciclica. <--\r\n");
            if(accion == 'a'){
                printf("\t <-- Exploracion en el Nodo Anterior\r\n");
            }
            if(accion == 's'){
                printf("\t --> Exploracion en el Nodo Siguiente\r\n");
            }
            if(accion == 'm') printf("\t --> la lista tiene %d Nodos\r\n", footer->posicion); 
            printf("\n");
            run();
            printf(" null.\n");
            if(accion == 0) printf("\r\n\t --> Elija una opcion <--");
            switch (accion)
            {
                case 'f':
                    printf("\t--> Aun no Disponible.\r\n\r\n");
                    accion=0;
                    reinc++;
                break;
                case 'h':
                    printf("\t<-- Aun no Disponible\r\n\r\n");
                    reinc++;
                    accion=0;
                    break;
                case 'a':
                case 's':
                    explo(accion);
                break;
                case 'v':
                    printf("\t DATA Agg.\r\n\r\n");
                    nodo* h1 = header;
                    nodo* nuevonodo2 = new();
                    agg = 1;
                    aggDato();
                    agg = 0;
                    nodo* object2 = marca;
                    //object2->posicion = marca->posicion;
                    h1 = object2->ptrNext;
                    object2->ptrNext =nuevonodo2;
                    h1->ptrPrev =nuevonodo2;
                    nuevonodo2->ptrNext= h1;
                    nuevonodo2->ptrPrev = object2;
                    while(object2 != header->ptrPrev){
                        object2 = object2->ptrNext;
                        object2->posicion += 1;
                    }
                    nuevonodo2->posicion = (h1->posicion)-1;
                    printf("\r\n\t --> Ingrese el nuevo dato: ");
                    scanf("%d", &nuevonodo2->dato);
                    printf("\r\n\t --> Change Complete. <-- \r\n");
                    marca = NULL;
                break;
                case 'c':
                    printf("\t DATA Config\r\n\r\n");
                    nodo* nuevonodo = new();
                    agg = 1;
                    aggDato();
                    agg = 0;
                    nodo* object = marca;
                    nuevonodo = object;
                    printf("\r\n\t --> Ingrese el nuevo dato: ");
                    scanf("%d", &dat);
                    nuevonodo->dato = dat;
                    printf("\r\n\t --> Change Complete. <-- \r\n");
                    marca = NULL;
                break;
                case 'b':
                    printf("\t DATA  Delete.\r\n\r\n");
                    nodo* h2 = NULL;
                    nodo* h3 = NULL;
                    agg = 1;
                    aggDato();
                    agg = 0;
                    nodo* object3 = marca;
                    h2 = object3->ptrNext;
                    h3 = object3->ptrPrev;
                    while(object3 != header->ptrPrev){
                        object3 = object3->ptrNext;
                        object3->posicion -= 1;
                    }
                    h2->ptrPrev = h3;
                    h3->ptrNext =h2;
                    marca = NULL;
                break;
                case 'n':
                    /*nueva Lista                */
                    printf("\t Aun no Disponible\r\n\r\n");
                break;
                default:
                    
                break;
            }
          do {
                printf("\r\n\n\t a) Para Anterior.                                          f) Para volver a la lista anterior. (Aun no disponible).\r\n\t s) Para Siguiente.                                         h) Para ir a la lista siguiente. (Aun no disponible).\r\n\t c) Para cambiar dato de la lista.                          v) Para agregar dato a la lista. \r\n\t b) Para borrar un dato de la lista.                        Necesitas una nueva lista? Presiona n.(Aun no disponible).\r\n\t e) Para Exit.                                              m) Para mostrar la lista.\r\n"); 
                printf(" \tAccion: ");
                scanf(" %c",&accion);
            } while(accion != 's' && accion != 'a' && accion != 'e' && accion != 'h' && accion != 'f' && accion != 'n' && accion != 'c' && accion != 'v' && accion != 'b' && accion != 'm');
            if(accion == 'e') break;      
        }
        if(accion == 'e') break; //salida del sistema.      
    }
    system("clear");
    printf("\r\n\tExit Complete.\r\n\r\n");
    return EXIT_SUCCESS;
}
//Crear un nuevo nodo.
nodo* new(){
    nodo* nuevo = malloc(sizeof(nodo));
    nuevo->ptrNext =NULL;
    nuevo->ptrPrev = NULL;
    return (nuevo);
}
//Insertar al nodo.
void insert(nodo* Nodo){
    if(header == NULL){
        header = (struct nodo*) Nodo;
        footer = (struct nodo*) Nodo;
        selection = (struct nodo*) Nodo;
    }else{
        footer->ptrNext = (struct nodo* )  Nodo;
        Nodo->ptrPrev = footer;
        //Nodo->ptrNext = header->ptrNext;
        Nodo->ptrNext = header;
        footer = Nodo;
        header->ptrPrev = footer;
    }
}
void run(){
    nodo* remanente=NULL;
    remanente = header;
    do{
        printf("%d -> ", remanente->dato);
        remanente = remanente->ptrNext;
    }while(remanente != header);
}
//exploracion de la lista
void explo(char mover){
    //printf("\r\n\t--> Exploracion en: [ %d ] %d", selection->dato, mover);
    if(mover == 's'){
        selection = selection->ptrNext;
        nodo* datante = NULL;
        nodo* datpost = NULL;
        datante = selection->ptrPrev;
        datpost = selection->ptrNext;
        if(agg == 1) marca = selection;
        if(reinc >= 1) printf("\r\n\t --> Exploracion en la Lista: anterior --> %d [ %d ] %d <-- %ciguiente.", datante->dato, selection->dato, datpost->dato, mover);
        if(reinc == 0) printf("\r\n\t --> Exploracion en el nodo %d: anterior --> %d [ %d ] %d <-- %ciguiente.", selection->posicion, datante->dato, selection->dato, datpost->dato, mover);
    }
    if(mover == 'a'){
        selection = selection->ptrPrev;
        nodo* datante = NULL;
        nodo* datpost = NULL;
        datante = selection->ptrPrev;
        datpost = selection->ptrNext;
        if(agg == 1) marca = selection;
        if(reinc >= 1) printf("\r\n\t <-- Exploracion en la lista: %cnterior --> %d [ %d ] %d <-- siguisente. ", mover, datante->dato, selection->dato, datpost->dato);
        if(reinc == 0) printf("\r\n\t <-- Exploracion en el nodo %d: %cnterior --> %d [ %d ] %d <-- siguiente. ",selection->posicion,  mover, datante->dato, selection->dato, datpost->dato);
    }
}
void aggDato(){
    char nd = 0, mover = 0,  rebote = 0;
    system("clear");
    while(nd != 'n'){
        system("clear");
        rebote = 0;
        mover = 0;
        do{
            if(rebote == 0) printf("\r\n\t A donde quiere mover [a/s]: " );
            //if(rebote >= 1) printf("\t");
            scanf("%c", &mover);
            rebote++;
        }while(mover != 'a' && mover != 's');
        rebote = 0;
        explo(mover);
        do{
            if(rebote == 0) printf("\n\r\n\t  Quiere seguir buscando? [y/n]: ");
            //if(rebote >= 1) printf("\t");
            scanf("%c", &nd);
            rebote++;
        } while(nd != 'y' && nd != 'n');
    }
    system("clear");
    printf("\r\n\t --> Nodo seleccionado. <-- \r\n");
}
