#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Grafo {
public:

    Grafo();

    void agregarNodo(int id, string nombre);
    

    void eliminarNodo(int id);


    void agregarCalle(int origen, int destino, float distancia, bool dobleVia);
    

    void eliminarCalle(int origen, int destino, bool dobleVia);

    void mostrarLista();

    void mostrarMatriz();

    int contarNodos();

private:
    struct Nodo {
        int id;           // Número único (ejemplo: 0, 1, 2)
        string nombre;    // Nombre (ejemplo: "Centro")
        bool existe;      // true = activo, false = eliminado
    };


    struct Calle {
        int destino;      // A dónde llega la calle
        float distancia;  // Cuántos kilómetros
    };
    
    vector<Nodo> intersecciones;      // Lista de todas las intersecciones
    vector<vector<Calle>> conexiones; // Lista de calles de cada intersección

    int buscarPosicion(int id);
    

    bool existeNodo(int id);
};

#endif // GRAFO_H