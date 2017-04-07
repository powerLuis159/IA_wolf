#pragma once
struct lobo
{
	std::vector<float> data;
	int dim;
	float aptitud;
	lobo() { dim = 1; aptitud - 1; }
	lobo(int d, std::random_device &generador, std::uniform_real_distribution<float> &distribucion) :dim(d)
	{
		
		for (size_t j = 0; j < dim; j++)
		{
			data.push_back(distribucion(generador));
		}
		aptitud = -1;
	}
};


class Wolf
{
private:
	float (*funcion)(std::vector<float>);
	bool(*comparacion)(lobo, lobo);
	unsigned int dim;
	lobo alpha, betha, delta;
	std::vector<lobo> poblacion;
	float min, max;

public:

	float historico[50];

	Wolf(float(*funcion)(std::vector<float>), unsigned int numero_poblacion, unsigned int dimension, float minimo, float maximo,bool minimizar);
	Wolf(float(*funcion)(std::vector<float>),unsigned int numero_poblacion,unsigned int dimension, float minimo,float maximo);
	lobo optmizar(unsigned int iteraciones);
	~Wolf();
};

