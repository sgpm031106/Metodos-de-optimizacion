/*
Universidad Autonoma de Aguascalientes
Centro de Ciencas Basicas
Ingenieria en Computacion Inteligente
Departamento de Ciencias de la Computacion
Optimizacion Inteligente

	- Pérez Moreno Sergio Gabriel
	- Rovelo Gonzalez Miguel Adrian
	
Agosto del 2023
*/

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double f(double x)
{
	return 2*pow(x,2)+ 16/x;
}
double ff(double x)
{
	return 4*x - 16/pow(x,2);
}

double fff(double x)
{
	return 4 + 32/pow(x,3);
}

int main()
{
	double x, error;
	int max_iteraciones = 100;
	
	cout<<"Ingrese el valor inicial de x: ";
	cin>>x;
	cout<<"\n";
	
	cout<<"Digite la tolerancia de error: ";
	cin>>error;
	cout<<"\n";
	
	cout << "Funcion original: f(x) = 2x^2 + 16/x" << endl;
    cout << "1a derivada: f'(x) = 4x - 16/x^2" << endl;
    cout << "2a derivada: f''(x) = 4 + 32/x^3" << endl << endl;

    cout<<setw(12)<<"No. Iteración"<<setw(12)<<"xk"<<setw(12)<<"f(x)"<<setw(12)<<"f'(x)"<<setw(12)<<"f''(x)"<<setw(12)<<"x(k+1)"<<endl;
    cout<<"\n";
    
    double x_sig = x - ff(x)/fff(x);
    
    int i = 0;
    
    while(i < max_iteraciones)
    {
    	double fx = f(x);
    	double fx_1 = ff(x);
    	double fx_2 = fff(x);
    	
    	cout<<setw(12)<<i<<setw(12)<<x<<setw(12)<<fx<<setw(12)<<fx_1<<setw(12)<<fx_2;
    	
    	double x_sig = x - fx_1/fx_2;
    	
    	cout<<setw(12)<<x_sig<<endl;
    	
    	if(abs(x_sig - x) < error)
    	{
    		cout<<"\n";
    		cout<<"El valor minimo encontrado de la funcion es: "<<f(x_sig)<<endl; 
    		return 0;
		}
		x = x_sig;
		i++;
	}
	cout<<"Se superaron el numero de iteraciones. "<<endl;
	return 0;
}
