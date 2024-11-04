
// EJERCICIO 2

// Dado un archivo de texto que contiene una lista de números enteros. El primer valor de la lista es el número de entradas (números) del resto del archivo. Escriba un programa que lea el archivo en un array (ubicado en la memoria principal) y obtenga el valor máximo usando varios hilos. Para obtener el valor máximo, el array debe dividirse en tantos bloques como número de hilos. De esta manera, cada hilo calcula el valor máximo de su porción de array asignada. Luego, el programa principal calcula el máximo global a partir de los valores parciales proporcionados por los hilos.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

typedef struct thread_data {
   int *numeros;
   int rangoInicial;
   int rangoFinal;
   int max;

} thread_data;

void *valorMaximo(void *arg) {
    thread_data *t_data = (thread_data *) arg;
    int *numeros = t_data->numeros;
    int rangoInicial = t_data->rangoInicial;
    int rangoFinal = t_data->rangoFinal;

    printf("Calculando max entre %d y %d\n", rangoInicial, rangoFinal);

    int max = numeros[rangoInicial];
    for (int i = rangoInicial; i < rangoFinal; i++) {
        if (numeros[i] > max) {
            max = numeros[i];
        }
    }

    t_data->max = max;
    pthread_exit(NULL);
}

int main() {
    
    FILE *fichero = fopen("numeros.txt", "r");
    if (fichero == NULL) {
        perror("Error! No se pudo abrir el archivo de entrada! ");
        return -1;
    }

    int n;
    fscanf(fichero, "%d", &n);
    int *numeros = (int *) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(fichero, "%d", &numeros[i]);
    }

    fclose(fichero);


    int numHilos = 10;
    printf("Ingrese cantidad de hilos no mayor a %d\n", n);
    scanf("%d", &numHilos);
    pthread_t hilos[numHilos];

    int maxGLOBAL = INT_MIN;
    
    for (int i = 0; i < numHilos; i++) {
        thread_data t_data;
        t_data.numeros = numeros;
        t_data.rangoInicial = i * (n / numHilos);
        t_data.rangoFinal = (i + 1) * (n / numHilos);
        pthread_create(&hilos[i], NULL, valorMaximo, (void *)&t_data);
        pthread_join(hilos[i], NULL);
        if(t_data.max > maxGLOBAL){
            maxGLOBAL = t_data.max;
        }
    }

    printf("Máximo Global: %d\n", maxGLOBAL);

    return 0;
}
