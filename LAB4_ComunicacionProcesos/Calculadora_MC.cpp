#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/wait.h> 

int main() {
    pid_t pid;
    char operacion[100];

    // Crear la memoria compartida
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *shm_op = (char*) shmat(shmid, NULL, 0);
    char *shm_res = shm_op + 512; // Usar la mitad de la memoria para el resultado

    pid = fork(); // Crear el proceso hijo

    if (pid < 0) {
        printf("Error al crear el proceso.\n");
        return 1;
    } else if (pid == 0) {
        // Proceso hijo: Realiza el cálculo
        sleep(3); // Esperar un poco para asegurarnos de que el padre escribe primero
        printf(" > Proceso hijo recibe: %s\n", shm_op);

        // Realizar el cálculo
        char operador;
        int num1, num2, res;
        sscanf(shm_op, "%d %c %d", &num1, &operador, &num2);

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
                if (num2 != 0)
                    res = num1 / num2;
                else {
                    strcpy(shm_res, "Error: División por cero.");
                    shmdt(shm_op); // Desadjuntar memoria
                    exit(1); // Terminar el proceso hijo con error
                }
                break;
            default:
                strcpy(shm_res, "Error: Operación no válida.");
                shmdt(shm_op);
                exit(1); // Terminar el proceso hijo con error
        }

        sprintf(shm_res, "Resultado: %d", res);
        printf(" > Proceso hijo envía: %s\n", shm_res);
        shmdt(shm_op); // Desadjuntar memoria compartida
        exit(0); // Terminar el proceso hijo correctamente

    } else {
        // Proceso padre
        printf(" >> Calculadora con memoria compartida <<\n\n");
        printf(" Operación >> ");
        fgets(operacion, sizeof(operacion), stdin); // Leer la operación de la entrada estándar
        printf("\n > Proceso padre envía: %s", operacion);
        strcpy(shm_op, operacion); // Escribir la operación en la memoria compartida

        // Esperar a que el hijo termine
        wait(NULL); // Esperar a que el hijo termine

        // Leer el resultado desde la memoria compartida
        printf(" > Proceso padre recibe: %s\n", shm_res);

        // Desadjuntar y eliminar la memoria compartida
        shmdt(shm_op);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
