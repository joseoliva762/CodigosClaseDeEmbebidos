#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* task1(){
    while(1)    printf("TASK UNO\n");
}

void* task2(){
    while(1) printf("TASK DOS\n");
}

int main(void){
    pthread_t hilo1, hilo2; //pthread_t es un tipo de estructura
    pthread_create(&hilo1, NULL, task1, NULL ); //(estructura, atributos, funcion, paramentros);
    pthread_create(&hilo2, NULL, task2, NULL );
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return EXIT_SUCCESS;
}