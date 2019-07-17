#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* task1(){
	while(1){
		printf("Task 1\n");

	}
}

void* task2(){
	while(1){
		printf("Task 2\n");

	}
}

int main(void) {
	pthread_t hilo1, hilo2;
    pthread_create(&hilo1, NULL, task1,  NULL);
    pthread_create(&hilo2, NULL, task2,  NULL);
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pause(5);
	return EXIT_SUCCESS;
}
