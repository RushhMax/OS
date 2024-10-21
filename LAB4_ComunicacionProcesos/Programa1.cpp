#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2]; // Array para el pipe
    pid_t pid;
    char buffer[100];

    // Crear el pipe
    if (pipe(fd) == -1) {
        printf("Error al crear el pipe.\n");
        return 1;
    }

    pid = fork(); // Crear el proceso hijo

    if (pid < 0) {
        printf("Error al crear el proceso.\n");
        return 1;
    } else if (pid == 0) {
        // Proceso hijo
        close(fd[1]); // Cerrar el extremo de escritura
        read(fd[0], buffer, sizeof(buffer)); // Leer desde el pipe
        printf("Proceso hijo recibió: %s\n", buffer);
        close(fd[0]); // Cerrar el extremo de lectura
    } else {
        // Proceso padre
        close(fd[0]); // Cerrar el extremo de lectura
        char mensaje[] = "Hola desde el proceso padre!";
        write(fd[1], mensaje, strlen(mensaje) + 1); // Escribir en el pipe
        close(fd[1]); // Cerrar el extremo de escritura
    }

    return 0;
}
