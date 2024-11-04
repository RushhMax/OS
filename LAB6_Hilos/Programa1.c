// SISTEMAS OPERATIVOS 
// LABORATORIO 6 
// realizado por: Rushell Vanessa Zavalaga Orozco

// EJERCICIO 1

// Crear un programa que sea capaz de contar cuántas vocales hay en un fichero. El programa padre debe lanzar cinco procesos hijo, donde cada uno de ellos se ocupará de contar una vocal concreta (que puede ser minúscula o mayúscula). Cada subproceso que cuenta vocales deberá dejar el resultado en un fichero. El programa padre se ocupará de recuperar los resultados de los ficheros, sumar todos los subtotales y mostrar el resultado final en pantalla.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *contarVocales(void *vocal) {
    char *v = (char *) vocal;
    int contador = 0;
    char c;
    FILE *fichero = fopen("vocales.txt", "r");
    if (fichero == NULL) {
        perror("Error! No se pudo abrir el archivo de entrada! ");
        pthread_exit(NULL);
    }

    while ((c = fgetc(fichero)) != EOF) {
        if (c == *v || c == *v - 32) { 
            contador++;
        }
    }
    fclose(fichero);

    char nombreFichero[20];
    sprintf(nombreFichero, "vocal%c.txt", *v);
    fichero = fopen(nombreFichero, "w");
    if (fichero == NULL) {
        perror("Error al abrir el archivo de salida");
        pthread_exit(NULL);
    }
    fprintf(fichero, "%d", contador);
    fclose(fichero);

    pthread_exit(NULL);
}

int main() {
    pthread_t hiloA, hiloE, hiloI, hiloO, hiloU;

    pthread_create(&hiloA, NULL, contarVocales, "a");
    pthread_create(&hiloE, NULL, contarVocales, "e");
    pthread_create(&hiloI, NULL, contarVocales, "i");
    pthread_create(&hiloO, NULL, contarVocales, "o");
    pthread_create(&hiloU, NULL, contarVocales, "u");

    pthread_join(hiloA, NULL);
    pthread_join(hiloE, NULL);
    pthread_join(hiloI, NULL);
    pthread_join(hiloO, NULL);
    pthread_join(hiloU, NULL);

    int contadorA, contadorE, contadorI, contadorO, contadorU;
    FILE *fA = fopen("vocala.txt", "r");
    FILE *fE = fopen("vocale.txt", "r");
    FILE *fI = fopen("vocali.txt", "r");
    FILE *fO = fopen("vocalo.txt", "r");
    FILE *fU = fopen("vocalu.txt", "r");

    fscanf(fA, "%d", &contadorA);
    fscanf(fE, "%d", &contadorE);
    fscanf(fI, "%d", &contadorI);
    fscanf(fO, "%d", &contadorO);
    fscanf(fU, "%d", &contadorU);

    fclose(fA);
    fclose(fE);
    fclose(fI);
    fclose(fO);
    fclose(fU);

    int total = contadorA + contadorE + contadorI + contadorO + contadorU;
    printf("Vocales a = %d, e = %d, i = %d, o = %d, u = %d \n ",
            contadorA, contadorE, contadorI, contadorO, contadorU);
    printf("Total de vocales: %d\n", total);

    return 0;
}
