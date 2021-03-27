//  Populacao.hpp
//  Geneticos
//
//  Created by Vini Cassol.

#include "Individuo.hpp"
#include <vector>

class Populacao
{
public:
    Populacao(int numIndividuos, int numGenes);
    Individuo GetFittest();
    Individuo GetSegundoFittest();
    int GetPosMenosFittest();
    void CalcularFitnessIndividuos();
    int GetFittestValue();

    //Individuo *individuos;
	std::vector<Individuo> individuos;
private:
    int popTamanho;
    int fittest;
};
