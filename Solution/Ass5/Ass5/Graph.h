#pragma once
#include <array>
#include <list>
#include <memory>

using namespace std;

namespace {

	const int N = 3;

	array<array<bool, N>, N> givenGraph = {{
	{0,1,1},
	{1,0,0},
	{1,0,0}
}};
}





class Graph
{
private:
	array<array<bool, N>, N> &p_matrix;
	array<size_t, N> p_colors;

	void hillClimbing();
	void tabuSearch();
	void simulatedAnneling();
	int calcFitness(array<size_t, N>);
	int findConflictVertex();
	array<size_t, N> findAllConflictVertecies();

public:
	Graph(array<array<bool, N>, N> &, array<size_t, N>);
	~Graph(void);
	void RunLocalSearch();
	int CalcFitness();

	static shared_ptr<array<size_t, N>> createRandomColors(int maxColors){

		shared_ptr<array<size_t, N>> temp(new array<size_t, N>());

		for (int i = 0; i < N; i++)
		{
			(*temp)[i] = rand() % maxColors;
		}
		return temp;
	}

};

