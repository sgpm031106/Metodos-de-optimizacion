/* 		PRACTICA 5

Universidad Autonoma de Aguascalientes
Centro de Ciencias Basicas
Departamento Ciencias de la Computacion
Optimizacion Inteligente
	
	- Pérez Moreno Sergio Gabriel
	- Rovelo Gonzalez Miguel Adrian

Luis Fernando Guiterrez Marfileño
	
Problema del job shop resuelto mediante la implementación de un sistema de colonia de hormigas

Diciembre del 2023
*/

#include<iostream>
#include<math.h>
#include<time.h>
#include<stdlib.h>

using namespace std;

const int a = 1; // Influencia en rastro de feromonas
const int b = 2; // Influencia en la informacion metaheuristica
const int p = 0.1; // Parametro de evaporacion
const int n0 = 1; // Feromona inicial
const int k = 50; // Numero de hormigas
const int feromonasDeposito = 35; // Deposito de feromonas


// Funcion para mover la hormiga
void mover(int tiempos[9][9], int maquinas[9], double feromonas[9][9], int recorrido[3], int tiempoMaquina[3], int posiciones[3])
{
	// Se desplaza a un nodo aleatorio del primer trabajo (0 al 3)
	int nodoActual = rand() % 3;
	
	int k = 0;
	while (maquinas[nodoActual] != 0)
	{
		// Se guardan los datos del primer nodo
		recorrido[k] = maquinas[nodoActual];
		posiciones[k] = nodoActual;
		
		int caminos[3];
		int j = 0;
		for (int i = 0; i < 9; i++)
		{
			// Guarda los nodos a los posibles caminos
			if (tiempos[nodoActual][i] != 0)
			{
				caminos[j] = i;
				j++;
			} 
		}
		
		// Se obtiene la probabilidad para cada camino
		double tn[3] = {0.0, 0.0, 0.0};
		double sumatoria = 0.0;
		for (int i = 0; i < j; i++)
		{
		   double n = (1.0 / (tiempos[nodoActual][caminos[i]])) * (1.0 / (tiempos[nodoActual][caminos[i]]));
		   tn[i] = feromonas[nodoActual][caminos[i]] * n;
		   sumatoria += tn[i];
		}
		
		double probabilidad[3];
		for (int i = 0; i < j; i++)
		{
			if (i > 0)
			{
				probabilidad[i] = (tn[i] / sumatoria) + probabilidad[i - 1];
			}
			else
			{
				probabilidad[i] = (tn[i] / sumatoria);
			}
		}
		
		// Se mueve al siguiente
		double prob = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
		for (int i = 0; i < j; i++)
		{
			if (prob < probabilidad[i])
			{
				// Se guarda el tiempo
				tiempoMaquina[k] = tiempos[nodoActual][caminos[i]];
				
				nodoActual = caminos[i];
				i = j;
			}
		}
		
		k++;
	}
}

// Función para comprobar el tiempo total
void calcularMakespan(int& makespan, int tiempoMaquina[3])
{
	makespan = 0;
	for (int i = 0; i < 3; i++)
	{
		makespan += tiempoMaquina[i];
	}
}

// Funcion para inicializar feromonas
void inicializarFeromonas(double feromonas[9][9], int tiempos[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			// Si existe la arista
			if (tiempos[i][j] != 0)
			{
				feromonas[i][j] = n0;
			}
			else 
			{
				feromonas[i][j] = 0.0;
			}
		}
	}
}

// Funcion para actualizar feromonas
void actualizarFeromonas(double feromonas[9][9], int makespan, int posiciones[3])
{
	int k = 0; // Para saber si ha pasado por esa arista
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			// Para comprobar si recorrio la arista y aumentar su valor
			int transitada = 0;
			if (posiciones[k] == i && posiciones[k + 1] == j)
			{
				transitada = (feromonasDeposito / makespan);
				k++;
			}
			// Actualiza las feromonas
			if (feromonas[i][j] > 0)
			{
				feromonas[i][j] = (1 - p) * feromonas[i][j] + transitada;
			}
		}
	}
}

int main()
{
	cout << "\t\t --- Colonia de Hormigas ---\n\n";
	cout << " El ACO es una tecnica probabilistica para solucionar problemas computacionales\n"
	 << " que pueden reducirse a buscar los mejores caminos o rutas en grafos.\n"
	 << " El problema JSSP (Job Shop Scheduling Problem) se compone de lo siguiente: dado un conjunto\n"
	 << " M que contiene |M| maquinas, y un conjunto J que contiene |J| trabajos, donde cada uno de\n"
     << " los trabajos tiene una secuencia ordenada de operaciones a ser ejecutadas en las maquina.\n\n";
	 
	system("pause");
	system("cls");
	
	srand(time(NULL));
	
	// Creación de matriz de tiempos (cada nodo representa un proceso,
	// las aristas los tiempos)
	int tiempos[9][9] = { 	{0, 0, 0, 3, 3, 3, 0, 0, 0}, // Trabajo 1
							{0, 0, 0, 3, 3, 3, 0, 0, 0},
							{0, 0, 0, 3, 3, 3, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 2, 2, 0}, // Trabajo 2
							{0, 0, 0, 0, 0, 0, 3, 3, 0},
							{0, 0, 0, 0, 0, 0, 4, 4, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 3}, // Trabajo 3
							{0, 0, 0, 0, 0, 0, 0, 0, 2},
							{0, 0, 0, 0, 0, 0, 0, 0, 0} }; // Nodo final
	// Indica que maquina es cada nodo (0 es nodo final)
	int maquinas[9] = {1, 2, 3, 3, 3, 1, 2, 2, 0}; 
	// Rastro de feromonas por arista
	double feromonas[9][9];
	
	int opc = 0;
	do
	{
		system("cls");
		cout << "\t\t --- Colonia de Hormigas ---\n\n";
		
		// Se incializan las feromonas
		inicializarFeromonas(feromonas, tiempos);
	
		// Guarda el recorrido de la hormiga, es decir, las maquinas que realizan cada trabajo
		int recorrido[3] = {0, 0, 0}; 
		// Guardan el tiempo de cada trabajo
		int tiempoMaquina[3] = {0, 0, 0};
		// Guarda el makespan total
		int makespan = 100;
		
		// Comienza el algoritmo de colonia de hormigas 
		int hormiga = 0;
		while(hormiga < k)
		{
			int nuevoRecorrido[3] = {0, 0, 0};
			int nuevosTiempos[3] = {0, 0, 0};
			int nuevoMakespan = 0;
			int posiciones[3];
			
			mover(tiempos, maquinas, feromonas, nuevoRecorrido, nuevosTiempos, posiciones);	
			calcularMakespan(nuevoMakespan, nuevosTiempos);
			actualizarFeromonas(feromonas, nuevoMakespan, posiciones);
			
			// Si el nuevo recorrido es mejor que el anterior, se reemplaza
			if (nuevoMakespan < makespan)
			{
				for (int i = 0; i < 3; i++)
				{
					recorrido[i] = nuevoRecorrido[i];
					tiempoMaquina[i] = nuevosTiempos[i];
				}
				makespan = nuevoMakespan;
			}
			
			hormiga++;
		}
		
		cout << " Tiempos (resultados) finales::\n\n";
		for (int i = 0; i < 3; i++)
		{
			cout << " Trabajo " << i + 1 << ": Maquina " << recorrido[i] << " - Tiempo: " << tiempoMaquina[i] << "\n";
		}
		cout << "\n Makespan: " << makespan << "\n\n";
		
		system("pause");
		system("cls");
		cout << " Deseas ejecutarlo de nuevo?\n";
		cout << " 1. Si\n";
		cout << " 2. No\n";
		cout << " Opcion: ";
		cin >> opc;
	}while(opc != 2);
	
	system("cls");
	cout<<"Saliendo del programa. "<<endl<<endl;
	system("pause");
	
	return 0;
}
