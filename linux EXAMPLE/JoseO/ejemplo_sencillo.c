#include <stdio.h>
#include <stdlib.h>


char cuenta(){
    static char cont; //staticram para que no almacene en el heap si no en la sram
     cont++; 
    return(cont);
}

void mem(){
    int vector[1000]={};
}

int main(void){
    cuenta();
    mem(); // esto para modificar el head
    cuenta();
    mem();
    cuenta();
    mem();
    printf("cuenta = %d ", cuenta());

    return EXIT_SUCCESS;
}