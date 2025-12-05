#include "Grafo.h"

// es el constructor del grafo
Grafo::Grafo() {
    // Los vectores empiezan vacíos automáticamente
    // No necesitamos hacer nada aquí
}

//FUNCIONES AUXILIARES (PRIVADAS)

// Busca la posición de una intersección por su id hasta encontrarla, recorriendo asi todo el vector
int Grafo::buscarPosicion(int id) {
    // Recorremos todas las intersecciones
    for (int i = 0; i < intersecciones.size(); i++) {
        // Si encontramos el id, regresamos su posición
        if (intersecciones[i].id == id) {
            return i;
        }
    }
    // Si no lo encontramos, regresamos -1
    return -1;
}

// Verifica si una intersección existe y está activa
bool Grafo::existeNodo(int id) {
    // Buscamos la posición
    int pos = buscarPosicion(id);
    
    // Si no existe se usa la posición -1, regresamos false
    if (pos == -1) {
        return false;
    }
    
    // Si existe pero está eliminado, regresamos false
    if (!intersecciones[pos].existe) {
        return false;
    }
    
    // Si existe y está activo, regresamos true
    return true;
}

// FUNCIONES PARA NODOS

//  agrega una nueva intersección al mapa
void Grafo::agregarNodo(int id, string nombre) {
    // Primero verificamos si ya existe
    int pos = buscarPosicion(id);
    
    // CASO 1: Ya existe y está activo
    if (pos != -1 && intersecciones[pos].existe) {
        cout << "Hubo un error ya que la interseccion con el id: " << id << " ya existe" << endl;
        return;
    }
    
    // CASO 2: Existió pero fue eliminado - lo reactivamos
    if (pos != -1 && !intersecciones[pos].existe) {
        intersecciones[pos].existe = true;
        intersecciones[pos].nombre = nombre;
        conexiones[pos].clear(); // Borramos sus calles viejas
        cout << "La interseccion con el id: " << id << " ha sido reactivada" << endl;
        return;
    }
    
    // CASO 3: Es totalmente nuevo
    Nodo nuevo;
    nuevo.id = id;
    nuevo.nombre = nombre;
    nuevo.existe = true;
    
    // Lo agregamos al vector
    intersecciones.push_back(nuevo);
    
    // Creamos su lista de calles vacía
    vector<Calle> listaVacia;
    conexiones.push_back(listaVacia);
    
    cout << "la interseccion conn el id: " << id << " fue agregada" << endl;
}

/**
 * Elimina una intersección del mapa
 */
void Grafo::eliminarNodo(int id) {
    // Buscamos la posición
    int pos = buscarPosicion(id);
    
    // Verificamos que exista
    if (pos == -1 || !intersecciones[pos].existe) {
        cout <<"La interseccion con el id de: " << id << " no existe" << endl;
        return;
    }
    
    // Marcamos el nodo como eliminado
    intersecciones[pos].existe = false;
    
    // Borramos todas sus calles salientes
    conexiones[pos].clear();
    
    // Borramos todas las calles que lleguen a este nodo
    for (int i = 0; i < conexiones.size(); i++) {
        // Revisamos cada calle de cada intersección
        for (int j = 0; j < conexiones[i].size(); j++) {
            // Si esta calle va hacia el nodo eliminado, la quitamos
            if (conexiones[i][j].destino == id) {
                conexiones[i].erase(conexiones[i].begin() + j);
                j--; // Retrocedemos porque borramos un elemento
            }
        }
    }
    
    cout <<"la interceccion con el id de " << id << " fue eliminada" << endl;
}

//FUNCIONES PARA CALLES

// Agrega una nueva calle entre dos intersecciones
void Grafo::agregarCalle(int origen, int destino, float distancia, bool dobleVia) {
    // Verificamos que ambas intersecciones existan
    if (!existeNodo(origen) || !existeNodo(destino)) {
        cout << "una o las dos intersecciones no existen\n";
        return;
    }
    
    // Buscamos las posiciones
    int posOrigen = buscarPosicion(origen);
    int posDestino = buscarPosicion(destino);
    
    // PASO 1: Agregar calle de origen -> destino
    
    // Primero verificamos que no exista ya
    bool yaExiste = false;
    for (int i = 0; i < conexiones[posOrigen].size(); i++) {
        if (conexiones[posOrigen][i].destino == destino) {
            yaExiste = true;
            break;
        }
    }
    
    // Si no existe, la agregamos
    if (!yaExiste) {
        Calle nueva;
        nueva.destino = destino;
        nueva.distancia = distancia;
        conexiones[posOrigen].push_back(nueva);
    }
    
    // PASO 2: Si es doble vía, agregar calle de destino -> origen
    if (dobleVia) {
        // Verificamos que no exista ya
        yaExiste = false;
        for (int i = 0; i < conexiones[posDestino].size(); i++) {
            if (conexiones[posDestino][i].destino == origen) {
                yaExiste = true;
                break;
            }
        }
        
        // Si no existe, la agregamos
        if (!yaExiste) {
            Calle nueva;
            nueva.destino = origen;
            nueva.distancia = distancia;
            conexiones[posDestino].push_back(nueva);
        }
    }
    
    cout << "se agrega la calle: " << origen << " -> " << destino;
    cout << " (" << distancia << " km";
    if (dobleVia) cout << ", doble via";
    cout << ")" << endl;
}

/**
 * Elimina una calle entre dos intersecciones
 */
void Grafo::eliminarCalle(int origen, int destino, bool dobleVia) {
    // Verificamos que ambas intersecciones existan
    if (!existeNodo(origen) || !existeNodo(destino)) {
        cout << "una o las dos intersecciones no existen" << endl;
        return;
    }
    
    // Buscamos las posiciones
    int posOrigen = buscarPosicion(origen);
    int posDestino = buscarPosicion(destino);
    
    bool eliminada = false;
    
// elimina la calle en el sentido origen -> destino
    for (int i = 0; i < conexiones[posOrigen].size(); i++) {
        if (conexiones[posOrigen][i].destino == destino) {
            conexiones[posOrigen].erase(conexiones[posOrigen].begin() + i);
            eliminada = true;
            break;
        }
    }
    
// se valida si es doble via para eliminar la calle en sentido contrario
    if(dobleVia) {
        for (int i = 0; i < conexiones[posDestino].size(); i++) {
            if (conexiones[posDestino][i].destino == origen) {
                conexiones[posDestino].erase(conexiones[posDestino].begin() + i);
                eliminada = true;
                break;
            }
        }
    }
    
    if(eliminada){
        cout << " la calle " << origen << " -> " << destino << " fue eliminada" << endl;
    } else {
        cout << "la calle ingresada no existe" << endl;
    }
}


// se muestra el grafo como una lista de adyacencia
void Grafo::mostrarLista() {
    cout << "LISTA DE ADYACENCIA" << endl;
    
    // Recorremos todas las intersecciones
    for (int i = 0; i < intersecciones.size(); i++) {
        // Solo mostramos las activas
        if (!intersecciones[i].existe) continue;
        
        // Mostramos el id y nombre de la intersección
        cout << intersecciones[i].id << " (" << intersecciones[i].nombre << ") -> ";
        
        // Si no tiene calles, lo indicamos
        if (conexiones[i].empty()) {
            cout << "(sin calles)";
        } else {
            // Mostramos todas sus calles
            for (int j = 0; j < conexiones[i].size(); j++) {
                int idDestino = conexiones[i][j].destino;
                float dist = conexiones[i][j].distancia;
                
                // Buscamos el nombre del destino
                int posDestino = buscarPosicion(idDestino);
                string nombreDestino = "";
                if (posDestino != -1) {
                    nombreDestino = intersecciones[posDestino].nombre;
                }
                
                // Imprimimos la calle
                cout << idDestino << " (" << nombreDestino << ", " << dist << " km)";
                
                // Si hay más calles, ponemos coma
                if (j < conexiones[i].size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
}

/**
 * Muestra el grafo como una matriz (tabla)
 */
void Grafo::mostrarMatriz() {
    cout << "MATRIZ DE ADYACENCIA" << endl;
    
    // PASO 1: Recolectar solo los nodos activos
    vector<int> idsActivos;
    for (int i = 0; i < intersecciones.size(); i++) {
        if (intersecciones[i].existe) {
            idsActivos.push_back(intersecciones[i].id);
        }
    }
    
    // Si no hay nodos, no hay nada que mostrar
    if (idsActivos.empty()) {
        cout << "No hay intersecciones para mostrar" << endl;
        return;
    }
    
    int n = idsActivos.size();
    
    // PASO 2: Crear la matriz llena de -1 (significa "no hay calle")
    vector<vector<float>> matriz;
    for (int i = 0; i < n; i++) {
        vector<float> fila;
        for (int j = 0; j < n; j++) {
            fila.push_back(-1); // -1 = no hay conexión
        }
        matriz.push_back(fila);
    }
    
    // PASO 3: Llenar la diagonal con 0 (distancia de un nodo a sí mismo es 0)
    for (int i = 0; i < n; i++) {
        matriz[i][i] = 0;
    }
    
    // PASO 4: Llenar la matriz con las distancias reales
    for (int i = 0; i < n; i++) {
        int idOrigen = idsActivos[i];
        int posOrigen = buscarPosicion(idOrigen);
        
        // Revisamos todas las calles de esta intersección
        for (int k = 0; k < conexiones[posOrigen].size(); k++) {
            int idDestino = conexiones[posOrigen][k].destino;
            float distancia = conexiones[posOrigen][k].distancia;
            
            // Buscamos en qué columna está el destino
            for (int j = 0; j < n; j++) {
                if (idsActivos[j] == idDestino) {
                    matriz[i][j] = distancia;
                    break;
                }
            }
        }
    }
    
    // PASO 5: Imprimir la matriz bonita
    cout << "-1 = no hay calle" << endl;
    
    // Imprimir encabezado de columnas
    cout << "      "; // Espacio para el encabezado de filas
    for (int j = 0; j < n; j++) {
        cout << "   " << idsActivos[j] << "  ";
    }
    cout << "\n";
    
    // Imprimir cada fila
    for (int i = 0; i < n; i++) {
        // Encabezado de fila
        cout << "   " << idsActivos[i] << "  ";
        
        // Valores de la fila
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] == -1) {
                cout << "  -1  "; // No hay calle
            } else {
                cout << "  " << matriz[i][j] << "  ";
            }
        }
        cout << endl;
    }
}

//  FUNCIONES AUXILIARES 

//Cuenta cuántas intersecciones activas hay
int Grafo::contarNodos() {
    int contador = 0;
    
    // Recorremos todos los nodos
    for (int i = 0; i < intersecciones.size(); i++) {
        // Si está activo, lo contamos
        if (intersecciones[i].existe) {
            contador++;
        }
    }
    
    return contador;
}