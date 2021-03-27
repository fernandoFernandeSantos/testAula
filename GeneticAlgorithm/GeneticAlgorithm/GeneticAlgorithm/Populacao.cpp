//  Populacao.cpp
//  Geneticos
//
//  Created by Vini Cassol.

#include "Populacao.hpp"
#include <limits.h>

Populacao::Populacao(int numIndividuos, int numGenes)
{
    popTamanho = numIndividuos;
    //individuos = new Individuo[numIndividuos];
	this->individuos = std::vector<Individuo>(numIndividuos);

    for(int i=0; i < numIndividuos; i++)
    {
        individuos[i].SetupIndividuo(numGenes);
    }
}

Individuo Populacao::GetFittest()
{
   int maxFit = 0;
   int maxFitIndex = 0;
   for (int i = 0; i < popTamanho; i++) {
       if (maxFit <= individuos[i].GetFitness()) {
           maxFit = individuos[i].GetFitness();
           maxFitIndex = i;
       }
   }
   fittest = individuos[maxFitIndex].GetFitness();
    
   return individuos[maxFitIndex];
}

Individuo Populacao::GetSegundoFittest()
{
    int maxFit1 = 0;
    int maxFit2 = 0;
    for (int i = 0; i < popTamanho; i++)
    {
       if (individuos[i].GetFitness() > individuos[maxFit1].GetFitness())
       {
           maxFit2 = maxFit1;
           maxFit1 = i;
       } else if (individuos[i].GetFitness() > individuos[maxFit2].GetFitness()) {
           maxFit2 = i;
       }
    }
    return individuos[maxFit2];
}

int Populacao:: GetPosMenosFittest()
{
    int minFitVal = INT_MAX;
    int minFitPos = 0;
    
    for (int i = 0; i < popTamanho; i++)
    {
        if (minFitVal >= individuos[i].GetFitness()) {
            minFitVal = individuos[i].GetFitness();
            minFitPos = i;
        }
    }
    return minFitPos;
}

void Populacao::CalcularFitnessIndividuos()
{
    for (int i = 0; i < popTamanho; i++)
    {
        individuos[i].CalcularFitness();
    }
    GetFittest(); // Para calcular o valor de fit de cada individuo
}

int Populacao::GetFittestValue()
{
    return  fittest;
}
