/*
Universidad Autonoma de Aguascalientes
Centro de Ciencias Exactas
Ingenieria en Computacion Inteligente
Departamento de Ciencias de la Camputacion
Optimizacion Inteligente

	Pérez Moreno Sergio Gabriel

Profesor: Marfileño

Octubre del 2023
*/


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

//Constantes que determinan el comportamiento del codigo
const int N = 8; //numero de reinas
const int NUM_INDIVIDUOS = 50; //individuos que se generaran al azar
const int NUM_PADRES = 10; //el numero de mejores individuos que se tomaran
const int NUM_HIJOS = 10; //el numero de hijos resultantes de 2 padres
const double PROBABILIDAD_MUTACION = 0.5; //probabilidad de mutacion
const int MAX_GENERACIONES = 1000; //maximo de generaciones en busqueda de la respuesta

struct Individuo {
    int tablero[N][N];
    int numChoques;
};

//generacion de los individuos aleatorios 
void generarTableroInicial(int tablero[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tablero[i][j] = 0;
        }
    }

    std::vector<int> columnas_disponibles(N, 0);

    for (int i = 0; i < N; i++) {
        int columna = rand() % N;
        while (columnas_disponibles[columna] == 1) {
            columna = rand() % N;
        }
        tablero[i][columna] = 1;
        columnas_disponibles[columna] = 1;
    }
    
    /*
    for (int i=0; i<N; i++)
    {
    	for(int j=0; j<N; j++)
    	{
    		cout<<tablero[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
	*/
}

//contador de choques de reinas en el tablero
int contarChoques(int tablero[N][N]) {
    int numChoques = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tablero[i][j] == 1) {
                for (int k = 1; k < N; k++) {
                	
                	/*
                    if (i + k < N && j + k < N && tablero[i + k][j + k] == 1) {
                        numChoques++; // Diagonal inferior derecha
                    }
                    if (i + k < N && j - k >= 0 && tablero[i + k][j - k] == 1) {
                        numChoques++; // Diagonal inferior izquierda
                    }
                    if (i - k >= 0 && j + k < N && tablero[i - k][j + k] == 1) {
                        numChoques++; // Diagonal superior derecha
                    }
                    if (i - k >= 0 && j - k >= 0 && tablero[i - k][j - k] == 1) {
                        numChoques++; // Diagonal superior izquierda
                    }
                    */
                    
                    if (j + k < N && tablero[i][j + k] == 1) {
                        numChoques++; // Filas a la derecha
                    }
                    if (j - k >= 0 && tablero[i][j - k] == 1) {
                        numChoques++; // Filas a la izquierda
                    }
                    if (i + k < N && tablero[i + k][j] == 1) {
                        numChoques++; // Hacia abajo en la misma fila
                    }
                    if (i - k >= 0 && tablero[i - k][j] == 1) {
                        numChoques++; // Hacia arriba en la misma columna
                    }
                }
            }
        }
    }

    return numChoques / 2; // Se cuentan dos veces
}


//se comparan los individuos para buscar los mejores con menor numero de choques
bool compararPorChoques(const Individuo& a, const Individuo& b) {
    return a.numChoques < b.numChoques;
}

void cruzar(const int madre[N][N], const int padre[N][N], int hijo[N][N]) {
    int puntoCorte = rand() % N;

    for (int k = 0; k < N; k++) {
        if (k <= puntoCorte) {
            std::copy(padre[k], padre[k] + N, hijo[k]);
        } else {
            std::copy(madre[k], madre[k] + N, hijo[k]);
        }
    }
}
//muta a los hijos 
void mutar(int hijo[N][N]) {
    int filaMutacion = rand() % N;
    int colMutacion = rand() % N;
    hijo[filaMutacion][colMutacion] = 1;
}

bool no=false;

void final()
{
	std::srand(std::time(0));

    std::vector<Individuo> poblacion(NUM_INDIVIDUOS);
//se ordenan las matrizes o individuos de acuerdo a su numero de choques
    for (int i = 0; i < NUM_INDIVIDUOS; i++) {
        generarTableroInicial(poblacion[i].tablero);
        poblacion[i].numChoques = contarChoques(poblacion[i].tablero);
    }
    bool solucionCorrecta = true;
 	int generaciones = 0;
    while (generaciones < MAX_GENERACIONES) {
        std::sort(poblacion.begin(), poblacion.end(), compararPorChoques);

//si el primero individuo o el menor numero de choques es igual a 0 se verifica que tenga las 8 reinas, si si se imprime y se detiene el codigo, si no continua a reproducir y mutar
        if (poblacion[0].numChoques == 0) {
            
          
            for (int i = 0; i < N; i++) {
                int contadorReinas = 0;
                for (int j = 0; j < N; j++) {
                    if (poblacion[0].tablero[i][j] == 1)
                        contadorReinas++;
                }
                if (contadorReinas != 1) {
                    solucionCorrecta = false;
                    break;
                }
            }

            if (solucionCorrecta) {
                cout << "Solucion encontrada en " << generaciones << " generaciones:" << endl;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++)
                        cout << poblacion[0].tablero[i][j] << " ";
                    cout << endl;
                }
                no=true;
                break;
            }
        }

        std::vector<Individuo> nuevaPoblacion;

        for (int i = 0; i < NUM_PADRES; i += 2) {
            for (int j = 0; j < NUM_HIJOS; j++) {
                Individuo hijo1, hijo2;
                cruzar(poblacion[i].tablero, poblacion[i+1].tablero, hijo1.tablero);
                cruzar(poblacion[i+1].tablero, poblacion[i].tablero, hijo2.tablero);
                mutar(hijo1.tablero);
                mutar(hijo2.tablero);
                hijo1.numChoques = contarChoques(hijo1.tablero);
                hijo2.numChoques = contarChoques(hijo2.tablero);
                nuevaPoblacion.push_back(hijo1);
                nuevaPoblacion.push_back(hijo2);
            }
        }

        poblacion.insert(poblacion.end(), nuevaPoblacion.begin(), nuevaPoblacion.end());
        std::sort(poblacion.begin(), poblacion.end(), compararPorChoques);
        poblacion.resize(NUM_INDIVIDUOS);

    generaciones++;
    }
      
         if (!no) {
         		cout<<"A diferencia de una reina, una torre solo se mueve de izquierda a derecha y de arriba a abajo. "<<endl;
         		cout<<"Por lo tanto se quitan los contadores de choques en diagonales inferiores y superiores tanto derechas como izquierdas. "<<endl;
                cout << "Solucion mas cercana encontrada en " << generaciones << " generaciones:" << endl;
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++)
                        cout << poblacion[0].tablero[i][j] << " ";
                    cout << endl;
                }
              
            }
}

int main()
{
	int op;
	do
	{
		cout<<"1. Resolver problema de las torres. "<<endl;
		cout<<"2. Salir del programa. "<<endl;
		cout<<"Que desea hacer? ";
		cin>>op;
		switch(op)
		{
			case 1:
				final();
				break;
				
			case 2:
				cout<<"Saliendo del programa. "<<endl<<endl;
				break;
				
			default:
				cout<<"Ingrese otra respuesta porfavor. "<<endl<<endl;
				break;
		}
		system("pause");
		system("cls");
	}while(op != 2);
    return 0;
}

