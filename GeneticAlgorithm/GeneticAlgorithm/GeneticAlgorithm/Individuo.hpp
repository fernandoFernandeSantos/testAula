//  Individuo.hpp
//  Geneticos
//
//  Created by Vini Cassol on 14/03/21.
//

#include <iostream>
//using namespace std;
#include <vector>

class Individuo
{
public:
    Individuo();
    void SetupIndividuo(int qtdGenes);
    void CalcularFitness();
    int GetFitness();
    int GetQtdGenes();
    //int *genes;
	std::vector<int> genes;

private:
    int fitness;
    int qtdGenes;
};
