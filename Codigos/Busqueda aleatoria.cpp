/*
Universidad Autonoma de Aguascalientes
Centro de Ciencias Basicas
Ingenieria en Computacion Inteligente
Departamento de Ciencias de la Computacion
Programacion Cientifica

	- Perez Moreno Sergio Gabriel
	- Rovelo Gonzalez Miguel Adrian
Agosto del 2023
*/

#include <iostream>
#include <cmath>
#include <cstdlib>

#define max_iteraciones 100

using namespace std;

double fx(int x,int y)
{
    double f=-((y+47)*sin(sqrt(abs(y+(x/2)+47))))-(x*(sin(sqrt(abs(x-(y+47))))));
    return(f);
}

int main()
{
    int inf,sup,it,x1,x2;
    double result;
    cout<<"Indica el limite inferior: ";
    cin>>inf;
    cout<<"\n";
    
    cout<<"Indica el limite superior: ";
    cin>>sup;
    cout<<"\n";
    
    x1= inf + rand()% (sup+1-inf);
    x2= inf + rand()% (sup+1-inf);
    
    int i=0;
    
    result=fx(x1,x2);
    
    while(i < max_iteraciones)
	{
        x1= inf + rand()% (sup+1-inf);
        x2= inf + rand()% (sup+1-inf);
        
        if(fx(x1,x2) < result)
		{
            result=fx(x1,x2);
        }
        i++;
    }
    cout<<"El minimo encontrado es:"<<result;
}   

