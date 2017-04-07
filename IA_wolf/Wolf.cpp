#include "stdafx.h"
#include "Wolf.h"


bool pelea(lobo a, lobo b) { return a.aptitud < b.aptitud; }
bool peleafuerte(lobo a, lobo b) {
	return a.aptitud > b.aptitud;
}



Wolf::Wolf(float(*funcion)(std::vector<float>), unsigned int numero_poblacion, unsigned int dimension, float minimo, float maximo, bool minimizar) :
	dim(dimension), funcion(funcion), min(minimo), max(maximo)
{
	std::random_device generador;
	std::uniform_real_distribution<float> distribucion(min, max);
	for (size_t i = 0; i < numero_poblacion; i++)
	{
		poblacion.push_back(lobo(dim, generador, distribucion));
	}
	if (minimizar)
	{
		comparacion = pelea;
	}
	else
	{
		comparacion = peleafuerte;
	}
}

Wolf::Wolf( float (*nfuncion)(std::vector<float>) ,unsigned int num_poblacion,unsigned int ndim, float min, float max) 
{
	Wolf(nfuncion, num_poblacion, ndim, min, max, true);
}

lobo Wolf::optmizar(unsigned int iteraciones)
{
	float tick = 2.0 / iteraciones;
	float a = 2.0;

	std::vector<float> A1, A2, A3, C1, C2, C3;
	std::random_device generador;
	std::uniform_real_distribution<float> distribucion(0, 1);

	for (size_t i = 0; i < dim; i++)
	{
		A1.push_back(distribucion(generador) * 4 - 2);
		A2.push_back(distribucion(generador) * 4 - 2);
		A3.push_back(distribucion(generador) * 4 - 2);
		C1.push_back(distribucion(generador) * 2);
		C2.push_back(distribucion(generador) * 2);
		C3.push_back(distribucion(generador) * 2);
	}

	for (size_t i = 0; i < poblacion.size(); i++)
	{
		poblacion[i].aptitud = funcion(poblacion[i].data);
	}

	std::sort(poblacion.begin(), poblacion.end(),comparacion);

	alpha = poblacion[0];
	betha = poblacion[1];
	delta = poblacion[2];
	unsigned int t = 0;
	while (t < iteraciones)
	{
		if (t < 50)
		{
			historico[t] = alpha.aptitud;
		}
#pragma omp parallel for
		for (int i = 0; i < poblacion.size(); i++)
		{
			for (int j = 0; j < dim; j++)
			{
				float Da = std::abs(C1[j] * alpha.data[j] - poblacion[i].data[j]);
				float Db = std::abs(C2[j] * betha.data[j] - poblacion[i].data[j]);
				float Dd = std::abs(C3[j] * delta.data[j] - poblacion[i].data[j]);

				float x1 = alpha.data[j] - A1[j] * Da;
				float x2 = betha.data[j] - A2[j] * Db;
				float x3 = delta.data[j] - A3[j] * Dd;	

				poblacion[i].data[j] = std::min(max,std::max(min,(x1 + x2 + x3) / 3));
				
			}
		}
		a -= tick;
#pragma omp parallel for
		for (int i = 0; i < dim; i++)
		{
			A1[i] = distribucion(generador) * a * 2 - a;
			A2[i] = distribucion(generador) * a * 2 - a;
			A3[i] = distribucion(generador) * a * 2 - a;
			C1[i] = distribucion(generador) * 2;
			C2[i] = distribucion(generador) * 2;
			C3[i] = distribucion(generador) * 2;
		}
#pragma omp parallel for
		for (int i = 0; i < poblacion.size(); i++)
		{
			poblacion[i].aptitud = funcion(poblacion[i].data);
		}

		std::sort(poblacion.begin(), poblacion.end(), comparacion);
		std::vector<lobo> desafio;
		desafio.reserve(6);
		desafio.push_back(alpha);
		desafio.push_back(betha);
		desafio.push_back(delta);
		desafio.push_back(poblacion[0]);
		desafio.push_back(poblacion[1]);
		desafio.push_back(poblacion[2]);
		std::sort(desafio.begin(), desafio.end(), comparacion);
		alpha = desafio[0];
		betha = desafio[1];
		delta = desafio[2];
		
		t++;
	}
		return alpha;
}


Wolf::~Wolf()
{
}
