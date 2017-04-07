// IA_wolf.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "Wolf.h"
#define PI 3.14159
float f1(std::vector<float> poblacion)
{
	float resultado = 0;
	for each (float var in poblacion)
	{
		resultado += var*var;
	}
	return resultado;
}

float ackley(std::vector<float> poblacion)
{
	float a=20, b=0.2,c=2*PI,d=poblacion.size(),cua=0,cose=0;
	for each (auto var in poblacion)
	{
		cua += var*var;
		cose += cos(c*var);
	}

	return -a*exp(-b*sqrt(cua / d)) - exp(cose / d) + a + exp(1);
}

float schwefel(std::vector<float> poblacion)
{
	float sum = 0;
	for each (auto var in poblacion)
	{
		sum += 418.9829f - var*sin(sqrt(abs(var)));
	}
	return sum;
}
	
float funcion3(std::vector<float> poblacion)
{
	float sum = 0;
	for each (auto var in poblacion)
	{
		sum += var*var;
	}

	sum = 0.5 - (pow(sin(sqrt(sum)), 2) - 0.5) / pow(1.0 + 0.001*sum, 2);
	return sum;
}

int main()
{
	Wolf *bosque[100];
	//true minimizar, false maximizar
	//Wolf prueba(funcion3, 100, 2, -100.0f, 100.0f,false);

	//prueba.optmizar(50);

	FILE *data;
	fopen_s(&data, "data.txt", "w");

	

#pragma omp parallel for
	for (int i = 0; i < 100; i++)
	{
		bosque[i]=new Wolf(schwefel, 150, 10, -500.0f, 500.0f,true);
		bosque[i]->optmizar(400);
	}
	float temp;
	for (size_t j = 0; j < 100; j++)
	{
		for (size_t i = 0; i < 50; i++)
		{
			temp = bosque[j]->historico[i];
			fprintf_s(data, "%f\t", temp);
		}
		fprintf_s(data, "\n");
		
	}
	fclose(data);
	
    return 0;
}

