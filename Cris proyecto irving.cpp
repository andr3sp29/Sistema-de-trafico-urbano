#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

class Grafo {
public:
    Grafo() = default;

    // Altas / bajas de nodos y aristas
    void altaNodo(int id, const string& nombre);
    void bajaNodo(int id);
    void altaArista(int origen, int destino, float peso, bool dirigido);
    void bajaArista(int origen, int destino, bool dirigido);

    // Impresiones
    void imprimirListaAdj() const;
    void imprimirMatriz() const;

private:
    struct Nodo {
        int id;
        string nombre;
        bool activo;
    };

    struct Arista {
        int destino;   // id del nodo destino
        float peso;    // tiempo/distancia/costo
    };

    vector<Nodo> nodos;              // nodos registrados
    vector<vector<Arista>> adj;      // lista de adyacencia por índice de nodo

    int indiceNodo(int id) const;    // regresa índice en 'nodos' a partir del id
    const Nodo* buscarNodoPorId(int id) const;
};

int Grafo::indiceNodo(int id) const {
    for (size_t i = 0; i < nodos.size(); ++i) {
        if (nodos[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

const Grafo::Nodo* Grafo::buscarNodoPorId(int id) const {
    int idx = indiceNodo(id);
    if (idx == -1 || !nodos[idx].activo) return nullptr;
    return &nodos[idx];
}

void Grafo::altaNodo(int id, const string& nombre) {
    int idx = indiceNodo(id);

    if (idx != -1) {
        if (nodos[idx].activo) {
            cout << "[Advertencia] El nodo con id " << id << " ya existe.\n";
        } else {
            // Reutilizamos un nodo dado de baja
            nodos[idx].activo = true;
            nodos[idx].nombre = nombre;
            adj[idx].clear();
            cout << "[OK] Nodo " << id << " reactivado.\n";
        }
        return;
    }

    Nodo n;
    n.id = id;
    n.nombre = nombre;
    n.activo = true;

    nodos.push_back(n);
    adj.emplace_back(); // nueva lista de adyacencia vacía

    cout << "[OK] Nodo " << id << " agregado.\n";
}

void Grafo::bajaNodo(int id) {
    int idx = indiceNodo(id);
    if (idx == -1 || !nodos[idx].activo) {
        cout << "[Error] Nodo " << id << " no existe o ya esta dado de baja.\n";
        return;
    }

    // Marcamos el nodo como inactivo y borramos sus aristas salientes
    nodos[idx].activo = false;
    adj[idx].clear();

    // Eliminamos aristas entrantes hacia este nodo
    for (size_t i = 0; i < adj.size(); ++i) {
        adj[i].erase(
            remove_if(adj[i].begin(), adj[i].end(),
                      [id](const Arista& a) { return a.destino == id; }),
            adj[i].end()
        );
    }

    cout << "[OK] Nodo " << id << " dado de baja.\n";
}

void Grafo::altaArista(int origen, int destino, float peso, bool dirigido) {
    const Nodo* nOri = buscarNodoPorId(origen);
    const Nodo* nDes = buscarNodoPorId(destino);

    if (!nOri || !nDes) {
        cout << "[Error] Origen o destino no existen o estan dados de baja.\n";
        return;
    }

    int idxOri = indiceNodo(origen);
    int idxDes = indiceNodo(destino);

    // Evitamos duplicados simples (misma arista exacta)
    auto existe = [](const vector<Arista>& lista, int destino) {
        for (const auto& a : lista) {
            if (a.destino == destino) return true;
        }
        return false;
    };

    if (!existe(adj[idxOri], destino)) {
        adj[idxOri].push_back({destino, peso});
    }

    if (!dirigido) {
        if (!existe(adj[idxDes], origen)) {
            adj[idxDes].push_back({origen, peso});
        }
    }

    cout << "[OK] Arista " << origen << " -> " << destino
         << " (peso=" << peso << (dirigido ? ", dirigida" : ", no dirigida") << ") agregada.\n";
}

void Grafo::bajaArista(int origen, int destino, bool dirigido) {
    const Nodo* nOri = buscarNodoPorId(origen);
    const Nodo* nDes = buscarNodoPorId(destino);

    if (!nOri || !nDes) {
        cout << "[Error] Origen o destino no existen o estan dados de baja.\n";
        return;
    }

    int idxOri = indiceNodo(origen);
    int idxDes = indiceNodo(destino);

    auto borrar = [](vector<Arista>& lista, int destino) {
        size_t antes = lista.size();
        lista.erase(
            remove_if(lista.begin(), lista.end(),
                      [destino](const Arista& a) { return a.destino == destino; }),
            lista.end()
        );
        return antes != lista.size();
    };

    bool borrada1 = borrar(adj[idxOri], destino);
    bool borrada2 = false;

    if (!dirigido) {
        borrada2 = borrar(adj[idxDes], origen);
    }

    if (borrada1 || borrada2) {
        cout << "[OK] Arista(s) eliminada(s).\n";
    } else {
        cout << "[Advertencia] No se encontro la arista especificada.\n";
    }
}

void Grafo::imprimirListaAdj() const {
    cout << "\n=== Lista de adyacencia ===\n";
    for (size_t i = 0; i < nodos.size(); ++i) {
        if (!nodos[i].activo) continue;

        cout << nodos[i].id << " (" << nodos[i].nombre << ") -> ";

        if (adj[i].empty()) {
            cout << "(sin vecinos)";
        } else {
            bool primero = true;
            for (const auto& a : adj[i]) {
                if (!primero) cout << ", ";
                const Nodo* destino = buscarNodoPorId(a.destino);
                if (destino) {
                    cout << destino->id << " (" << destino->nombre
                         << ", w=" << a.peso << ")";
                } else {
                    cout << a.destino << " (inactivo, w=" << a.peso << ")";
                }
                primero = false;
            }
        }
        cout << '\n';
    }
}

void Grafo::imprimirMatriz() const {
    cout << "\n=== Matriz de adyacencia ===\n";
    // Recolectamos sólo nodos activos
    vector<int> idsActivos;
    for (const auto& n : nodos) {
        if (n.activo) idsActivos.push_back(n.id);
    }

    if (idsActivos.empty()) {
        cout << "[Info] No hay nodos para mostrar.\n";
        return;
    }

    const float INF = numeric_limits<float>::infinity();
    size_t n = idsActivos.size();

    // Construir matriz llena de INF y diagonal en 0
    vector<vector<float>> matriz(n, vector<float>(n, INF));
    for (size_t i = 0; i < n; ++i) {
        matriz[i][i] = 0.0f;
    }

    // Llenar a partir de la lista de adyacencia
    for (size_t i = 0; i < n; ++i) {
        int idOrig = idsActivos[i];
        int idxOrig = indiceNodo(idOrig);

        for (const auto& a : adj[idxOrig]) {
            // buscamos la posición del destino
            for (size_t j = 0; j < n; ++j) {
                if (idsActivos[j] == a.destino) {
                    matriz[i][j] = a.peso;
                    break;
                }
            }
        }
    }

    cout << "INF = sin conexion\n\n";
    cout << fixed << setprecision(1);

    // Encabezado
    cout << setw(6) << " ";
    for (size_t j = 0; j < n; ++j) {
        cout << setw(6) << idsActivos[j];
    }
    cout << '\n';

    // Filas
    for (size_t i = 0; i < n; ++i) {
        cout << setw(6) << idsActivos[i];
        for (size_t j = 0; j < n; ++j) {
            if (matriz[i][j] == INF) {
                cout << setw(6) << "INF";
            } else {
                cout << setw(6) << matriz[i][j];
            }
        }
        cout << '\n';
    }
}

// --- Pequeño menu de prueba SOLO para el Módulo I ---
int main() {
    Grafo g;
    int opcion;

    do {
        cout << "\n===== MODULO I: Grafo (Estructuras base) =====\n";
        cout << "1) Alta nodo\n";
        cout << "2) Baja nodo\n";
        cout << "3) Alta arista\n";
        cout << "4) Baja arista\n";
        cout << "5) Mostrar lista de adyacencia\n";
        cout << "6) Mostrar matriz de adyacencia\n";
        cout << "0) Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int id;
                string nombre;
                cout << "Id del nodo: ";
                cin >> id;
                cout << "Nombre del nodo: ";
                getline(cin >> ws, nombre);
                g.altaNodo(id, nombre);
                break;
            }
            case 2: {
                int id;
                cout << "Id del nodo a eliminar: ";
                cin >> id;
                g.bajaNodo(id);
                break;
            }
            case 3: {
                int u, v;
                float w;
                int dir;
                cout << "Nodo origen: ";
                cin >> u;
                cout << "Nodo destino: ";
                cin >> v;
                cout << "Peso (tiempo/distancia/costo): ";
                cin >> w;
                cout << "Dirigida? (1 = si, 0 = no): ";
                cin >> dir;
                g.altaArista(u, v, w, dir == 1);
                break;
            }
            case 4: {
                int u, v;
                int dir;
                cout << "Nodo origen: ";
                cin >> u;
                cout << "Nodo destino: ";
                cin >> v;
                cout << "Dirigida? (1 = si, 0 = no): ";
                cin >> dir;
                g.bajaArista(u, v, dir == 1);
                break;
            }
            case 5:
                g.imprimirListaAdj();
                break;
            case 6:
                g.imprimirMatriz();
                break;
            case 0:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }

    } while (opcion != 0);

    return 0;
}
