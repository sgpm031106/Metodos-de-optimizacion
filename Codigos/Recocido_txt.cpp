/*
Universidad Autonoma de Aguascalientes
Centro de Ciencias Exactas
Departamento de Ciencias de la Computacion
Optimizacion Inteligente

	- Perez Moreno Sergio Gabriel
	- Rovelo Gonzalez Miguel Adrian
	
Octubre del 2023
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Estructura para representar una ciudad con coordenadas (x, y)
struct Ciudad {
    int x;
    int y;
};

// Función para calcular la distancia entre dos ciudades (euclidiana en este caso)
double calcularDistancia(Ciudad ciudad1, Ciudad ciudad2) {
    int dx = ciudad1.x - ciudad2.x;
    int dy = ciudad1.y - ciudad2.y;
    return sqrt(dx * dx + dy * dy);
}

// Función para calcular la distancia total de una ruta
double calcularDistanciaTotal(const vector<int>& ruta, const vector<Ciudad>& ciudades) {
    double distanciaTotal = 0.0;
    for (int i = 0; i < ruta.size() - 1; ++i) {
        distanciaTotal += calcularDistancia(ciudades[ruta[i]], ciudades[ruta[i + 1]]);
    }
    distanciaTotal += calcularDistancia(ciudades[ruta.back()], ciudades[ruta[0]]);
    return distanciaTotal;
}

// Función de enfriamiento para controlar la probabilidad de aceptar soluciones peores
double enfriamiento(double T, double factorEnfriamiento) {
    return T * factorEnfriamiento;
}

// Implementación del recocido simulado
pair<vector<int>, double> recocidoSimulado(vector<Ciudad>& ciudades, double TInicial, double factorEnfriamiento, int iteraciones) {
    int numCiudades = ciudades.size();
    vector<int> rutaActual(numCiudades);
    for (int i = 0; i < numCiudades; ++i) {
        rutaActual[i] = i;
    }
    random_shuffle(rutaActual.begin(), rutaActual.end());
    double distanciaActual = calcularDistanciaTotal(rutaActual, ciudades);

    vector<int> mejorRuta = rutaActual;
    double mejorDistancia = distanciaActual;

    srand(time(0));  // Inicializar la semilla aleatoria

    for (int i = 0; i < iteraciones; ++i) {
        double T = enfriamiento(TInicial, factorEnfriamiento);

        int indice1 = rand() % numCiudades;
        int indice2 = rand() % numCiudades;
        while (indice1 == indice2) {
            indice2 = rand() % numCiudades;
        }

        swap(rutaActual[indice1], rutaActual[indice2]);
        double nuevaDistancia = calcularDistanciaTotal(rutaActual, ciudades);
        double diferenciaDistancia = nuevaDistancia - distanciaActual;

        // Decisión: Aceptar la nueva solución si es mejor o con probabilidad e^(-diferenciaDistancia / T)
        if (diferenciaDistancia < 0 || (rand() / static_cast<double>(RAND_MAX)) < exp(-diferenciaDistancia / T)) {
            distanciaActual = nuevaDistancia;
        } else {
            swap(rutaActual[indice1], rutaActual[indice2]);
        }

        // Actualizar la mejor solución encontrada hasta ahora
        if (distanciaActual < mejorDistancia) {
            mejorRuta = rutaActual;
            mejorDistancia = distanciaActual;
        }
    }

    return make_pair(mejorRuta, mejorDistancia);
}

int main() {
	
	cout<<"El siguiente codigo resuelve el problema del viajero mediante el algoritmo de Recocido Simulado. "<<endl;
	cout<<"Se generan aleatoriamente 25 ciudades en lo que seria una matriz de 15x10 como se indica en la imagen del documento de \nla practica. "<<endl;
	cout<<"Se declara que el punto de inicio de la matriz, debera ser tambien el punto de llegada al hacer el recorrido. "<<endl;
	cout<<"Cada iteracion que se realiza es para crear rutas actuales avanzando a la siguiente ciudad\n"
	"paso en el que se realizan intercambios entre ciudades.\n"
	"Hecho esto, mediante probabilidades basadas en el factor de enfriamiento y en las distancias calculadas,\n"
	"se llega a una solucion final y optima hasta completar el total de iteraciones. "<<endl<<endl;
	
	cout<<"Al compilar y ejecutar el codigo, se va a crear un archivo de texto con la solucion al problema. \nSe guardara en la carpeta donde se tenga el codigo. "<<endl;
	cout<<"Al volver a ejecutar, el archivo de texto se actualizara. "<<endl;
	
    int numCiudades = 25;
    vector<Ciudad> ciudades(numCiudades);
    for (int i = 0; i < numCiudades; ++i) {
        ciudades[i].x = rand() % 15; // Coordenadas x aleatorias en el rango [0, 15]
        ciudades[i].y = rand() % 10; // Coordenadas y aleatorias en el rango [0, 10]
    }

    double TInicial = 1000.0;
    double factorEnfriamiento = 0.8;
    int iteraciones = 2000;

    pair<vector<int>, double> resultado = recocidoSimulado(ciudades, TInicial, factorEnfriamiento, iteraciones);

    // Redirigir la salida a un archivo de texto
    ofstream archivoSalida("salida.txt");

    archivoSalida << "Matriz de coordenadas de las ciudades:" << endl;
    for (int i = 0; i < numCiudades; ++i) {
        archivoSalida << "Ciudad " << i + 1 << ": (" << ciudades[i].x << ", " << ciudades[i].y << ")" << endl;
    }

    archivoSalida << "\nLa mejor ruta encontrada es:" << endl;
    for (int i = 0; i < numCiudades; ++i) {
        int ciudadIdx = resultado.first[i];
        archivoSalida << "Ciudad " << ciudadIdx + 1 << ": (" << ciudades[ciudadIdx].x << ", " << ciudades[ciudadIdx].y << ")" << endl;
    }

    archivoSalida << "Distancia total de la mejor ruta: " << resultado.second << endl;

    archivoSalida.close();  // Cerrar el archivo de salida

    return 0;
}


