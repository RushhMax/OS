#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // Crear clave para la memoria compartida
    key_t key = ftok("shmfile", 65);

    // Crear segmento de memoria compartida
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);

    // Adjuntar la memoria compartida al proceso
    char *str = (char*) shmat(shmid, NULL, 0);

    if (fork() == 0) {
        // Proceso hijo escribe en la memoria compartida
        sprintf(str, "Hola desde el proceso hijo.");
        shmdt(str); // Desadjuntar memoria compartida
    } else {
        // Proceso padre lee de la memoria compartida
        sleep(1); // Esperar para que el hijo escriba
        printf("Proceso padre lee: %s\n", str);
        shmdt(str); // Desadjuntar memoria compartida
        shmctl(shmid, IPC_RMID, NULL); // Eliminar el segmento de memoria compartida
    }

    return 0;
}
