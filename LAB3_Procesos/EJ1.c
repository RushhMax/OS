#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t fork_return;
    int status;

    fork_return = fork();  // Creación del proceso hijo

    if (fork_return == 0) {  /* Proceso hijo */
        printf("\n I'm the child!");
        exit(0);  // Termina el proceso hijo
    }
    else if (fork_return > 0) {  /* Proceso padre */
        wait(&status);  // Espera a que el hijo termine
        printf("\n I'm the parent!");
        if (WIFEXITED(status)) {
            printf("\n Child returned: %d\n", WEXITSTATUS(status));
        }
    }
    else {
        printf("Fork failed!");
    }

    return 0;  // Indica que el programa terminó correctamente
}
