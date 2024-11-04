#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double num1;
    double num2;
} Numbers;

void *suma(void *args) {
    Numbers *op = (Numbers *) args;
    printf("Suma: %.2f + %.2f = %.2f\n", op->num1, op->num2, op->num1 + op->num2);
    return NULL;
}

void *resta(void *args) {
    Numbers *op = (Numbers *) args;
    printf("Resta: %.2f - %.2f = %.2f\n", op->num1, op->num2, op->num1 - op->num2);
    return NULL;
}

void *multiplicacion(void *args) {
    Numbers *op = (Numbers *) args;
    printf("Multiplicación: %.2f * %.2f = %.2f\n", op->num1, op->num2, op->num1 * op->num2);
    return NULL;
}

void *division(void *args) {
    Numbers *op = (Numbers *) args;
    if (op->num2 != 0) {
         printf("División: %.2f / %.2f = %.2f\n", op->num1, op->num2, op->num1 / op->num2);
    } else {
        printf("División: Error, división por cero.\n");
    }
    return NULL;
}

int main() {
    pthread_t hilo_suma, hilo_resta, hilo_multiplicacion, hilo_division;
    Numbers nu;

    printf("Ingrese dos números: ");
    scanf("%lf %lf", &nu.num1, &nu.num2);

    pthread_create(&hilo_suma, NULL, suma, &nu);
    pthread_create(&hilo_resta, NULL, resta, &nu);
    pthread_create(&hilo_multiplicacion, NULL, multiplicacion, &nu);
    pthread_create(&hilo_division, NULL, division, &nu);

    pthread_join(hilo_suma, NULL);
    pthread_join(hilo_resta, NULL);
    pthread_join(hilo_multiplicacion, NULL);
    pthread_join(hilo_division, NULL);

    return 0;
}
