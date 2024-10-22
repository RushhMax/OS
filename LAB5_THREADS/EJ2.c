#include <windows.h>
#include <stdio.h>

DWORD WINAPI HiloFuncion(LPVOID Param){
    printf("Hola desde el hilo!\n");
    return 0; 
}

int main(){
    HANDLE Hilo;       /* Manejador del hilo */
    DWORD ThreadId;    /* Identificador del hilo */

    /* Crear un hilo que ejecutará la función HiloFuncion */
    Hilo = CreateThread(
        NULL,          /* Atributos de seguridad predeterminados */
        0,             /* Tamaño predeterminado de la pila */
        HiloFuncion,   /* Función que ejecutará el hilo */
        NULL,          /* Parámetro de la función del hilo */
        0,             /* Bandera de creación predeterminada */
        &ThreadId);    /* Devuelve el identificador del hilo */
    
    if (Hilo == NULL){
        printf("Error al crear el hilo.\n");
        return 1;
    }

    WaitForSingleObject(Hilo, INFINITE);

    CloseHandle(Hilo);

    printf("El hilo ha terminado.\n");
    return 0;
}
