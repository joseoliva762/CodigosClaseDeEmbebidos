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

    //llenar los datos en una funcion.
    docentes.estatura = 1.80;
    docentes.edad = 21;
    //libreria para escribir en un vector:
    memcpy(docentes.nombre,"jose oliva",sizeof("jose_oliva"));
    //empleado* ptrempleado = NULL;
    printf("\tNombre: %s. \r\n", docentes.nombre);
    printf("\tEdad: %d. \r\n", docentes.edad);
    printf("\tEstatura: %.2f.\r\n", docentes.estatura);
    return 0;
}