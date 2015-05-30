// Genetic5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma warning(disable:4786)		// disable debug warning

#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include "Graph.h"

#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")

using namespace std;				// polluting global namespace, but hey...

struct ga_struct 
{
	Graph* graph; 
	string str;						// the string
	unsigned int fitness;			// its fitness
};

typedef vector<ga_struct> ga_vector;// for brevity

void init_population(ga_vector &population,
					 ga_vector &buffer ) 
{
	int tsize = GA_TARGET.size();

	for (int i=0; i<GA_POPSIZE; i++) {
		ga_struct citizen;
		
		citizen.fitness = 0;
		citizen.str.erase();

		//for (int j=0; j<tsize; j++)
			//citizen.str += (rand() % 90) + 32;
		shared_ptr<array<size_t,N>> colors = Graph::createRandomColors(N);
		citizen.graph = new Graph(givenGraph,*colors,N, edgeslist);
		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

void calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;

	for (int i=0; i<GA_POPSIZE; i++) {
		fitness = 0;
		/*for (int j=0; j<tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j]));
		}*/
		fitness=population[i].graph->CalcFitness();
		population[i].fitness = fitness;
	}
}

bool fitness_sort(ga_struct x, ga_struct y) 
{ return (x.fitness < y.fitness); }

inline void sort_by_fitness(ga_vector &population)
{ sort(population.begin(), population.end(), fitness_sort); }

void elitism(ga_vector &population, 
				ga_vector &buffer, int esize )
{
	for (int i=0; i<esize; i++) {
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
	}
}

void mutate(ga_struct &member)
{
	/*int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32; 

	member.str[ipos] = ((member.str[ipos] + delta) % 122);*/
	/*CSM mutation */
	int minimumColor = member.graph->getColorNumberOfMinumumApperancesOfColorInVertices();
	int colorPlace = rand()%member.graph->getNumberOfVertices();
	int color = member.graph->getVertexColorAtIndex(colorPlace);
	while(color==minimumColor){//We found vertex with same color , as minimum ,must be changed.
		colorPlace = rand()%member.graph->getNumberOfVertices();
		color = member.graph->getVertexColorAtIndex(colorPlace);
	}
	member.graph->changeAllVerteciesWithGivenColor(minimumColor,color);
	return;
}

void UniformMating(ga_struct parent1,ga_struct parent2 , ga_struct offspring){
	for(int i=0;i<parent1.graph->getNumberOfVertices();i++){
		int coin = rand()%2;
		if(coin==1){
			offspring.graph->setVertexColorAtIndex(i,parent1.graph->getVertexColorAtIndex(i));
		}else{
			offspring.graph->setVertexColorAtIndex(i,parent2.graph->getVertexColorAtIndex(i));
		}
	}
	return;
}


int getBestCitizenIndexTournamentOfFour(ga_struct citizen1,ga_struct citizen2,ga_struct citizen3 , ga_struct  citizen4,int i1,int i2,int i3,int i4){
	int max = 0,best1=0,best2=0;
	best1=citizen1.fitness>=citizen2.fitness?i1:i2;
	best2=citizen3.fitness>=citizen4.fitness?i3:i4;
	return best1>=best2?best1:best2;
}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2,i3,i4,parent1Index,parent2Index;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {

		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		i3 = rand() % (GA_POPSIZE / 2);
		i4 = rand() % (GA_POPSIZE / 2);
		parent1Index = getBestCitizenIndexTournamentOfFour(population[i1],population[i2],population[i3],population[i4],i1,i2,i3,i4);
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		i3 = rand() % (GA_POPSIZE / 2);
		i4 = rand() % (GA_POPSIZE / 2);
		parent2Index = getBestCitizenIndexTournamentOfFour(population[i1],population[i2],population[i3],population[i4],i1,i2,i3,i4);
		/* Uniform Mating :
		buffer[i].str = population[i1].str.substr(0, spos) + 
			            population[i2].str.substr(spos, tsize - spos);
*/
		UniformMating(population[parent1Index],population[parent2Index],buffer[i]);
		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}




inline void print_best(ga_vector &gav)
{ cout << "Best: " << gav[0].str << " (" << gav[0].fitness << ")" << endl; }

inline void swap(ga_vector *&population,
				 ga_vector *&buffer)
{ ga_vector *temp = population; population = buffer; buffer = temp; }

int main()
{
	edgeslist = *Graph::createEdgesList();
	maxDensityEdge = Graph::FindMaxDensityEdge();
	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i=0; i<GA_MAXITER; i++) {
		calc_fitness(*population);		// calculate fitness
		sort_by_fitness(*population);	// sort them
		print_best(*population);		// print the best one

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers
	}

	return 0;
}
