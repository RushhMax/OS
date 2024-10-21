#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> 

int main() {
    int fd[2]; // Array para el pipe
    pid_t pid;
    char operacion[100];
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
    } else if (pid == 0) { // Proceso hijo
        close(fd[1]); // Cerrar el extremo de escritura del pipe
        read(fd[0], operacion, sizeof(operacion)); // Leer la operación enviada por el padre
        printf(" > Proceso hijo recibe: %s\n", operacion);

        // Realizar el cálculo
        char operador;
        int num1, num2, res = 0;
        sscanf(operacion, "%d %c %d", &num1, &operador, &num2);

        switch (operador) {
            case '+':
                res = num1 + num2;
                break;
            case '-':
                res = num1 - num2;
                break;
            case '*':
                res = num1 * num2;
                break;
            case '/':
                if (num2 != 0) {
                    res = num1 / num2;
                } else {
                    strcpy(buffer, "Error: División por cero.");
                    printf(" > Proceso hijo envía: %s\n", buffer);
                    write(fd[1], buffer, strlen(buffer) + 1);
                    close(fd[0]); // Cerrar el extremo de lectura
                    exit(0);
                }
                break;
            default:
                strcpy(buffer, "Error: Operación no válida.");
                printf(" > Proceso hijo envía: %s\n", buffer);
                write(fd[1], buffer, strlen(buffer) + 1);
                close(fd[0]); // Cerrar el extremo de lectura
                exit(0);
        }

        // Preparar el resultado para enviarlo al padre
        sprintf(buffer, "%d\n", res);
        printf(" > Proceso hijo envía: %s", buffer);
        write(fd[1], buffer, strlen(buffer) + 1); // Enviar el resultado al padre
        close(fd[0]); // Cerrar el extremo de lectura
        exit(0); // Salir del proceso hijo
    } else { // Proceso padre
        close(fd[0]); // Cerrar el extremo de lectura del pipe 
        printf(" >> Calculadora con tuberías <<\n\n");
        printf("Operacion >> ");
        fgets(operacion, sizeof(operacion), stdin); // Leer la operación desde la entrada estándar
        printf("\n > Proceso padre envía: %s", operacion);
        write(fd[1], operacion, strlen(operacion) + 1); // Enviar la operación al hijo
        
        wait(NULL); // Esperar a que el hijo termine

        // Leer el resultado enviado por el hijo
        close(fd[1]); // Cerrar el extremo de escritura después de escribir
        read(fd[0], buffer, sizeof(buffer)); // Leer el resultado del pipe
        printf(" > Proceso padre recibe: %s", buffer);
        exit(0); // Salir del proceso padre
    }

    return 0;
}
