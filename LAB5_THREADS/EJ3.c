#include <windows.h>
#include <stdio.h>

#define NUM_THREADS 5

/* Variable global para almacenar la suma total */
LONG Sum = 0;

/* Estructura para pasar parámetros a los hilos */
typedef struct {
    DWORD start;
    DWORD end;
} ThreadData;

/* Esta función será ejecutada por cada hilo */
DWORD WINAPI HiloSuma(LPVOID Param){
    ThreadData* data = (ThreadData*)Param;
    LONG localSum = 0;

    /* Sumar los números en el rango asignado */
    for (DWORD i = data->start; i <= data->end; i++) {
        localSum += i;
    }

    /* Sumar al total de forma segura */
    InterlockedAdd(&Sum, localSum);

    printf("Hilo sumando desde %lu hasta %lu, suma parcial = %ld\n", data->start, data->end, localSum);
    return 0;
}

int main(){
    HANDLE hilos[NUM_THREADS];
    DWORD ThreadId;
    ThreadData datos[NUM_THREADS];

    /* Dividir la suma en 5 hilos, cada uno con un rango diferente */
    DWORD rango = 100 / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        datos[i].start = i * rango + 1;
        datos[i].end = (i + 1) * rango;

        /* Crear un hilo que ejecutará la función HiloSuma */
        hilos[i] = CreateThread(
            NULL,             /* Atributos de seguridad predeterminados */
            0,                /* Tamaño predeterminado de la pila */
            HiloSuma,         /* Función que ejecutará el hilo */
            &datos[i],        /* Parámetros de la función del hilo */
            0,                /* Bandera de creación predeterminada */
            &ThreadId);       /* Devuelve el identificador del hilo */
        
        if (hilos[i] == NULL) {
            printf("Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    /* Esperar a que todos los hilos terminen */
    WaitForMultipleObjects(NUM_THREADS, hilos, TRUE, INFINITE);

    /* Cerrar todos los manejadores de hilos */
    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(hilos[i]);
    }

    /* Imprimir el resultado total */
    printf("Suma total = %ld\n", Sum);

    return 0;
}
