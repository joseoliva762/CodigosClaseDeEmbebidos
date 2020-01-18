//estructuras
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// un nuevo tipo de variables como un int, char...
typedef struct{
    char nombre[50];
    char edad;
    float estatura;
}empleado;

void llenar(empleado* estr) ;

int main(void){     
    empleado docentes;
    empleado* ptrDocentes = NULL;
    ptrDocentes = &docentes;
    //llenar los datos en una funcion.
    llenar (ptrDocentes);
    printf("\tNombre: %s. \r\n", docentes.nombre);
    printf("\tEdad: %d. \r\n", docentes.edad);
    printf("\tEstatura: %.2f.\r\n", docentes.estatura);
    return 0;
}

//cuando esta en una funcion tengo que referme con una flechay no con un el punto
//se puede con punteros. Punteros a estructuras (IMPORTANTE)
void llenar(empleado* estr) {
    estr->estatura = 1.80;
    estr->edad = 21;
    memcpy(estr->nombre,"jose oliva",sizeof("jose_oliva"));
}
