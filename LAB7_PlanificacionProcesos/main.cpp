#include <iostream>
#include <map>
#include <queue>
using namespace std;

struct Process{
    string id;
    int tiempoLlegada;
    int tiempoServicio;
    int tiempoRestante;

    Process(string id, int llegada, int servicio) : id(id), tiempoLlegada(llegada), tiempoServicio(servicio), tiempoRestante(servicio) {}
};


void RoundRobin(map<int, Process*> processes, int quantum) {
    queue<Process*> cola;

    int tiempo = 0;
    if (processes.find(tiempo) != processes.end()) cola.push(processes[tiempo]);

    Process* p_actual = nullptr;
    int tiempoEjecucion = 0;

    while (!cola.empty()) {
        if (!p_actual) p_actual = cola.front();
        
        tiempoEjecucion++;
        p_actual->tiempoRestante--;

        if (p_actual->tiempoRestante == 0) {
            cola.pop();
            if (!cola.empty()) p_actual = cola.front();
            tiempoEjecucion = 0;
        }
        if (tiempoEjecucion == quantum) {
            cola.pop();
            if (p_actual->tiempoRestante > 0) cola.push(p_actual);
            if (!cola.empty()) p_actual = cola.front();
            tiempoEjecucion = 0;
        }

        cout << "Tiempo: " << tiempo
             << ", Proceso: " << p_actual->id
             << ", Tiempo de Servicio: " << p_actual->tiempoServicio
             << ", Tiempo Restante: " << p_actual->tiempoRestante << endl;

        tiempo++;
        // Agregar nuevos procesos al tiempo actual
        if (processes.find(tiempo) != processes.end()) {
            cola.push(processes[tiempo]);
        }

        // Verificar si todos los procesos han terminado
        bool procesosTerminados = true;
        for (auto& [t, p] : processes) {
            if (p->tiempoRestante > 0) {
                procesosTerminados = false;
                break;
            }
        }
        if (procesosTerminados) break;
    }

    cout << "Round Robin" << endl;
}



void SJF(){
    cout << "SJF" << endl;
}



int main() {
    // Crear el mapa de procesos
    map<int, Process*> processes;

    // Insertar procesos en el mapa (clave: tiempo de llegada, valor: puntero al proceso)
    processes[0] = new Process("A", 0, 8); // Proceso A, llega en tiempo 0, servicio 8 unidades
    processes[1] = new Process("B", 1, 4); // Proceso B, llega en tiempo 1, servicio 4 unidades
    processes[2] = new Process("C", 2, 9); // Proceso C, llega en tiempo 2, servicio 9 unidades
    processes[3] = new Process("D", 3, 5); // Proceso D, llega en tiempo 3, servicio 5 unidades

    // Imprimir los procesos (para verificar)
    cout << "Procesos inicializados:\n";
    for (auto& [tiempo, proceso] : processes) {
        cout << "Tiempo: " << tiempo
             << ", Proceso: " << proceso->id
             << ", Tiempo de Servicio: " << proceso->tiempoServicio
             << ", Tiempo Restante: " << proceso->tiempoRestante << endl;
    }

    // Aquí puedes llamar a tu función RoundRobin
    int quantum = 3;
    RoundRobin(processes, quantum-1);

    // Liberar memoria
    for (auto& [tiempo, proceso] : processes) {
        delete proceso;
    }

    return 0;
}
