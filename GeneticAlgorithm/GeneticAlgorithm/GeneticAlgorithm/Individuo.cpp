//  Individuo.cpp
//  Geneticos
//
//  Created by Vini Cassol
//

#include "Individuo.hpp"

Individuo::Individuo()
{
    
}

void Individuo::SetupIndividuo(int qtdGenes)
{
    fitness = 0;
    //genes = new int[qtdGenes];
	this->genes = std::vector<int>(qtdGenes);
    this->qtdGenes = qtdGenes;
    
    // set genes randomicamente com valor 0 ou 1
    for(int i=0; i< qtdGenes; i++)
    {
        genes[i] = rand() % 2;
    }
}

void Individuo::CalcularFitness()
{
    fitness = 0;
    for (int i = 0; i < qtdGenes; i++) {
        if (genes[i] == 1)
        {
            ++fitness;
        }
    }
}

int Individuo::GetFitness()
{
    return fitness;
}
    
int Individuo::GetQtdGenes()
{
    return qtdGenes;
}
