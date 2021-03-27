/**
* Esse codigo foi majoritariamente baseado
* em https://github.com/kooli89/Travelling-Salesman-Problem
* Varias mudancas foram feitas para adequar ao problema pedido
* entretanto a logica do algoritmo segue o mesmo raciocinio apresentado
* em aula.
*/

#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#define inf 1000000


namespace GeneticAlgorithm {

	typedef std::vector<std::tuple<std::string, std::string, int> > ListOfCities;
	typedef std::vector<std::pair<int, int>> VerticePoints;

	const int V = 1005;
	const int E = 10005;
	int n, m, nbPopulation = 26;
	std::map<std::vector<int>, int> memo_value; // Memoization map to store the value of each path

	//Graph to store the path
	std::vector<std::vector<int>> graph(V, std::vector<int>(V));
	std::vector<int> bstSol;
	std::vector<std::vector<int> > par, child;
	int idx; //index of every city, useful for the encoding function
	std::map<std::string, int> encod;
	std::map<int, std::string> decod;


	void init() {
		idx = 0;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				graph[i][j] = inf; // initially we haven't any edge
			}
		}
	}

	void addBiEdge(int u, int v, int c) {
		graph[u][v] = c;
		graph[v][u] = c;
	}

	/*	void encoding() {
			std::ifstream inputFile("in");
			inputFile >> n >> m;
			init();
			for (int i = 0; i < m; ++i) {
				std::string city1, city2;
				int distance;
				inputFile >> city1 >> city2 >> distance;
				if (!encod.count(city1)) // if a city doesn't have an index yet
				{
					encod[city1] = idx;
					decod[idx++] = city1;
				}
				if (!encod.count(city2)) {
					encod[city2] = idx;
					decod[idx++] = city2;
				}
				// we use the index of each node to add a bidrectional edge to our graph representation
				addBiEdge(encod[city1], encod[city2], distance);
			}
		}*/

	void encoding(ListOfCities& cityList) {
		//std::ifstream inputFile("in");
		//	inputFile >> n >> m;
		//	n = n_;
		m = cityList.size();
		init();
		for (int i = 0; i < m; ++i) {
			std::string city1, city2;
			int distance;
			std::tie(city1, city2, distance) = cityList[i];

			//		inputFile >> city1 >> city2 >> distance;
					// if a city doesn't have an index yet
			if (!encod.count(city1)) {
				encod[city1] = idx;
				decod[idx++] = city1;
			}
			if (!encod.count(city2)) {
				encod[city2] = idx;
				decod[idx++] = city2;
			}
			// we use the index of each node to add a bidrectional edge to our graph representation
			addBiEdge(encod[city1], encod[city2], distance);
		}
	}

	std::vector<int> gen() {
		std::vector<int> myvector;
		for (int i = 0; i < n; ++i)
			myvector.push_back(i);
		random_shuffle(myvector.begin(), myvector.end()); // Random shuffling to generate population
		return myvector;
	}

	std::vector<int> crossover(std::vector<int> &par1, std::vector<int> &par2) {
		// visited array to mark nodes that have been already placed
		//bool vis[n]; 
		std::vector<bool> vis(n, false);
		// index of each node in the array par1 (parent1)
		//int indexOf[n];
		std::vector<int> indexOf(n);

		for (int i = 0; i < n; ++i) {
			indexOf[par1[i]] = i;
		}
		//memset(vis, false, sizeof vis);
		int ind = rand() % n; // random index from which we will begin the crossover
		vis[ind] = true;
		ind = indexOf[par2[ind]];
		while (!vis[ind]) // if vis[ind]==true then we have reached a cycle
		{
			vis[ind] = true;
			ind = indexOf[par2[ind]];
		}
		std::vector<int> ret;
		ret.resize(n);
		for (int i = 0; i < n; ++i) {
			if (vis[i])
				ret[i] = par1[i];
			else
				ret[i] = par2[i]; // not visited nodes in par1 will be replaced by nodes with the same index in par2
		}
		return ret;
	}

	int evaluation(std::vector<int> v) {
		int ret = 0;
		for (int i = 0; i < n - 1; i++) {
			ret += graph[v[i]][v[i + 1]]; // counting the sum of weights of edges represented by the nodes in the array v
		}
		ret += graph[v[n - 1]][v[0]];
		return ret;
	}

	std::vector<int> mutation(std::vector<int> &vv) {
		std::vector<int> v = vv;
		int ind1 = rand() % n; // first index
		int ind2(ind1); // second index
		while (ind2 == ind1) {
			ind2 = rand() % n;
		}
		// ind1 is always less than ind2
		if (ind1 > ind2)
			std::swap(ind1, ind2);
		reverse(v.begin() + ind1, v.begin() + ind2 + 1);
		return v;
	}

	int optimalSolution(int n) // brute force solution computes all the possible paths and return the best
	{      // Only works for small graphs ( n < 11 )
		std::vector<int> g;
		g.clear();
		g.resize(n);
		for (int i = 0; i < n; ++i)
			g[i] = i;
		int ret(4 * inf);
		while (next_permutation(g.begin(), g.end())) {
			ret = std::min(ret, evaluation(g));
		}
		return ret;
	}

	void population_generate() {
		nbPopulation += 4 - nbPopulation % 4; // nbPopulation will be always a multiple of 4
		par.clear();
		par.resize(nbPopulation);
		for (int i = 0; i < nbPopulation; ++i) {
			par[i] = gen();
		}
	}

	// Comparator that will be useful to sort the population according to their value
	bool cmp(const std::vector<int> &v1, const std::vector<int> &v2) {
		return (memo_value[v1] < memo_value[v2]);
	}

	void population_evaluate() {
		for (int i = 0; i < nbPopulation; ++i) {
			memo_value[par[i]] = evaluation(par[i]);
		}
		sort(par.begin(), par.end(), cmp);
		bstSol = par[0];
		for (int i = 0; i < nbPopulation / 2; ++i) {
			par.pop_back(); // eliminate last ranked populations after the sort operation
		}
	}

	void population_crossover() {
		int nn = (int)par.size();
		// for each consecutive couple in the population we do their crossover
		for (int i = 0; i < nn - 1; i += 2) {
			par.push_back(crossover(par[i], par[i + 1]));
		}
	}

	void population_mutation() {
		// We do the mutation only for the best solutions to have a effcient genotype
		for (int i = 0; i < nbPopulation / 4; ++i) {
			par.push_back(mutation(par[i]));
		}
	}

	std::string decoding(const std::vector<int>& v) {
		std::string output = "";
		for (int i = 0; i < (int)v.size(); ++i) {
			output += decod[v[i]] + " ";
			if (i < v.size())
				output += "--> ";
		}
		output += decod[v[0]] + "\n";
		return output;
	}

	std::vector<int> decodingIntVector() {
		std::vector<int> output(bstSol.size());
		for (int i = 0; i < (int)bstSol.size(); ++i) {
			output[i] = std::stoi(decod[bstSol[i]]);
		}
		output.push_back(std::stoi(decod[bstSol[0]]));
		return output;
	}

	void clearVars() {
		memo_value.clear();
		bstSol.clear();
		par.clear();
		child.clear();
		encod.clear();
		decod.clear();
		n = 0;
		m = 0;
		nbPopulation = 26;
		idx = 0;
	}

	void runGenetic(ListOfCities& generatedGraph, System::Windows::Forms::RichTextBox^ richBox, int verticeNumber) {
		//safe clean
		clearVars();
		//	encoding();
		n = verticeNumber;
		richBox->AppendText(gcnew System::String("Enconding the graph\n"));

		encoding(generatedGraph);
		richBox->AppendText(gcnew System::String("Population generating"));

		population_generate();
		richBox->AppendText(gcnew System::String("Population evaluate\n"));

		population_evaluate();
		richBox->AppendText(gcnew System::String("Starting the evolution\n"));

		int cur_best_solution = evaluation(par[0]); // current best solution of the population taken
		int last_best_solution(-1);
		int nb_iteration = 1;
		int infinite_iterations = (int) 1e6; // limit of number of iterations to avoid infinit loop
		while ((last_best_solution != cur_best_solution) && (infinite_iterations--)) {
			last_best_solution = cur_best_solution;
			population_crossover();
			population_mutation();
			population_evaluate();
			cur_best_solution = evaluation(par[0]); // par[0] is the best solution in the population because
			nb_iteration++; //  of the sort operation in population_evaluate function

			//imprime interacao
			std::string outputIt = "Iteration = " + std::to_string(nb_iteration) + "\n" + decoding(bstSol);
			richBox->AppendText(gcnew System::String(outputIt.c_str()));
		}

		std::string outputStr = "Number of iterations = " + std::to_string(nb_iteration) + "\n";
		outputStr += "Genetic algorithm solution weight: " + std::to_string(evaluation(bstSol)) + "\n";

		outputStr += "Genetic algorithm's outputed path:\n" + decoding(bstSol) + "\n";
		outputStr += std::to_string(n) + " vertices\n";
		if (n <= 8) {
			outputStr += "Optimal solution weight: " + std::to_string(optimalSolution(n)) + "\n";
		}
		else {
			outputStr += "It is not possible to get the optimal result for more than 8 vertices\n";
		}
		richBox->AppendText(gcnew System::String(outputStr.c_str()));

	}
}

