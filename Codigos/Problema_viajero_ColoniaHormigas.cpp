/*

Universidad Autonoma de Aguascalientes
Centro de Ciencias Basicas
Ingenieria en Computacion Inteligente
Departamento de Ciencias de la Computacion
Optimizacion Inteligente

	- Perez Moreno Sergio Gabriel
	
Luis Fernando Gutierrez Marfileño

Diciembre del 2023

Tercer examen parcial

*/
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

struct City {
    std::string name;
    int x;
    int y;
	int index;
    City(std::string n, int xx, int yy) : name(n), x(xx), y(yy) {}
};

vector<City> generate_unique_cities(int num_cities, const std::vector<City>& available_cities) {
    vector<City> cities;

	//Empezar con la ciudad uno
    cities.push_back(available_cities[0]);

	//Añadir las ciudades que faltan
    for (int i = 1; i < num_cities; ++i) {
        while (true) {
            City city = available_cities[rand() % available_cities.size()];
            bool city_exists = false;
            for (size_t j = 0; j < cities.size(); ++j) {
                if (cities[j].name == city.name) {
                    city_exists = true;
                    break;
                }
            }
            if (!city_exists) {
                cities.push_back(city);
                break;
            }
        }
    }

	//Añadir la primer ciudad al ultimo que es donde tiene que terminar el recorrido
    cities.push_back(available_cities[0]);

    return cities;
}

void print_city_map(const std::vector<City>& cities) {
    const int map_size = 13;

    for (int y = map_size; y >= 0; --y) {
        for (int x = 0; x <= map_size; ++x) {
            bool city_found = false;

            for (size_t i = 0; i < cities.size(); ++i) {
                if (cities[i].x == x && cities[i].y == y) {
                    city_found = true;
                    break;
                }
            }

            if (city_found) {
                std::cout << "x ";
            } else {
                std::cout << ". ";
            }
        }
        cout << std::endl;
    }
}

//Calcular la distancia de los recorridos con los cuadrados de las cordenadas y sacandoles su raiz
double calculate_distance(const City& city1, const City& city2) {
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

//Imprimir la tabla de coordenadas del recorrido total del viajero
void print_coordinates_table(const std::vector<City>& route) {
    cout << "\nTabla de Coordenadas:\n";
    cout << "+--------------+--------------+\n";
    cout << "|   Ciudad     | Coordenadas  |\n";
    cout << "+--------------+--------------+\n";
    for (size_t i = 0; i < route.size() - 1; ++i) {
        cout << "|    " << route[i].name << "    | (" << route[i].x << ", " << route[i].y << ")  |\n";
    }
    cout << "|    " << route[0].name << "    | (" << route[0].x << ", " << route[0].y << ")  |\n";
    cout << "+--------------+--------------+\n";
}

double calculate_fitness(const vector<City>& route) {
    double total_distance = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        total_distance += calculate_distance(route[i], route[i+1]);
    }
    total_distance += calculate_distance(route.back(), route[0]);
    return total_distance;
}

vector<vector<double> > initialize_pheromone_matrix(int num_cities) {
    const double initial_pheromone = 1.0;
    return vector<vector<double> >(num_cities, std::vector<double>(num_cities, initial_pheromone));
}

vector<City> generate_ant_tour(const vector<vector<double> >& pheromones, const vector<City>& cities) {
    const int num_cities = pheromones.size();
    vector<City> tour;
    vector<bool> visited(num_cities, false);

    int start_city = rand() % num_cities;
    tour.push_back(cities[start_city]);
    visited[start_city] = true;

    for (int i = 0; i < num_cities - 1; ++i) {
        int current_city_index = tour.back().index;
        double total_pheromone = 0.0;

        for (int j = 0; j < num_cities; ++j) {
            if (!visited[j]) {
                total_pheromone += pheromones[current_city_index][j];
            }
        }

        double rand_value = static_cast<double>(rand()) / RAND_MAX;
        double threshold = rand_value * total_pheromone;

        for (int j = 0; j < num_cities; ++j) {
            if (!visited[j]) {
                threshold -= pheromones[current_city_index][j];
                if (threshold <= 0.0) {
                    tour.push_back(cities[j]);
                    visited[j] = true;
                    break;
                }
            }
        }
    }

    return tour;
}


//Funcion para actualizar las feromonas
void update_pheromones(vector<vector<double> >& pheromones, const vector<City>& ant_tour, double evaporation_rate) {
    const double pheromone_deposit = 1.0;
    const int num_cities = pheromones.size();

    for (size_t i = 0; i < ant_tour.size() - 1; ++i) {
        int city1 = ant_tour[i].index;
        int city2 = ant_tour[i + 1].index;

        pheromones[city1][city2] = (1.0 - evaporation_rate) * pheromones[city1][city2] + pheromone_deposit;
        pheromones[city2][city1] = (1.0 - evaporation_rate) * pheromones[city2][city1] + pheromone_deposit;
    }
}

vector<City> ant_colony(int num_ants, int num_generations, const vector<City>& cities, double evaporation_rate) {
    const int num_cities = cities.size();
    vector<std::vector<double> > pheromones = initialize_pheromone_matrix(num_cities);
    vector<City> best_route;
    double best_distance = std::numeric_limits<double>::max();

    for (int generation = 0; generation < num_generations; ++generation) {
        vector<City> current_best_route;
        double current_best_distance = std::numeric_limits<double>::max();

        for (int ant = 0; ant < num_ants; ++ant) {
            vector<City> ant_tour = generate_ant_tour(pheromones, cities);
            double ant_distance = calculate_fitness(ant_tour);

            if (ant_distance < current_best_distance) {
                current_best_distance = ant_distance;
                current_best_route = ant_tour;
            }
        }

        if (current_best_distance < best_distance) {
            best_distance = current_best_distance;
            best_route = current_best_route;
        }

        update_pheromones(pheromones, current_best_route, evaporation_rate);
    }

    return best_route;
}

int main() {

    std::vector<City> available_cities;

    available_cities.push_back(City("1", 5, 4));
    available_cities.push_back(City("2", 7, 4));
    available_cities.push_back(City("3", 5, 6));
    available_cities.push_back(City("4", 4, 3));
    available_cities.push_back(City("5", 3, 6));
    available_cities.push_back(City("6", 4, 5));
    available_cities.push_back(City("7", 2, 4));
    available_cities.push_back(City("8", 1, 3));
    available_cities.push_back(City("9", 1, 5));
    available_cities.push_back(City("10", 3, 2));

    for (size_t i = 0; i < available_cities.size(); ++i) {
        available_cities[i].index = i;
    }

    int num_cities = 10;
    vector<City> cities = generate_unique_cities(num_cities, available_cities);
    int num_ants = 5, num_generations;
    double evaporation_rate = 0.5;

    int opc = 0;
    while (opc != 2) {
        cout << "\t\t MENU\n";
        cout << "1. Problema del viajero por colonia de hormigas.\n";
        cout << "2. Salir del programa.\n";
        cout << "Que desea hacer? ";
        cin >> opc;
        if (opc == 1) {
        	num_generations = 1000;
            system("cls");
            
            /*
            cout << "Cuantas hormigas quieres? ";
            cin >> num_ants;
            cout << "Cuanto de tasa de evaporacion? ";
            cin >> evaporation_rate;
            */

            vector<City> best_route = ant_colony(num_ants, num_generations, cities, evaporation_rate);
            double best_distance = calculate_fitness(best_route);
            cout << "\n La distancia total es: " << best_distance << "\n";

            print_coordinates_table(best_route);
            
            system("pause");
            system("cls");

        } else if (opc != 2) {
            cout << "Opcion invalida, ingrese otra respuesta. \n";
            system("pause");
            system("cls");
        } else {
            return 0;
        }
    }

    return 0;
}

