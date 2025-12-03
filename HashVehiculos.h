#pragma once 
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Vehiculo{
string id;
string tipo;
string placa;
int origen;
int destino;
string hora;

Vehiculo() : origen(-1), destino(-1){}

};

class HashVehiculos{
public:
explicit HashVehiculos(size_t mIni =101);

bool insertar (const Vehiculo &v);
const Vehiculo* buscar(const string &id) const;
bool eliminar(const string &id);

bool guardarCSV(const string &ruta) const;
bool cargarCSV(const string &ruta);

void info(ostream &o = cout) const;

 size_t size() const { return nElems; }
size_t buckets() const { return m; }

private:
 vector<vector<Vehiculo>> tabla;
size_t m;
size_t nElems;

 size_t hashStr(const string &s) const;
   void rehash(size_t newM);
};

