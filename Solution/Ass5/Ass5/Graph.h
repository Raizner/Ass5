#pragma once
#include <array>
#include <list>
#include <memory>
#include <tuple>

using namespace std;

namespace {

	const int N = 3;

	array<array<bool, N>, N> givenGraph = {{
	{0,1,1},
	{1,0,0},
	{1,0,0}
}};


	list<pair<int, int>> edgeslist;
}





class Graph
{
private:
	array<array<bool, N>, N> &p_matrix;
	list<pair<int, int>> &p_edges;
	array<size_t, N> p_colors;
	int kColor;
	void hillClimbing();
	void tabuSearch();
	void simulatedAnneling();
	int calcFitness(array<size_t, N> *input = nullptr);
	int findConflictVertex();
	array<size_t, N> findAllConflictVertecies();
	

public:
	Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors,int numberOfColors, list<pair<int, int>> &edges);
	~Graph(void);
	void RunLocalSearch();
	int CalcFitness();
	int getColorNumberOfMinumumApperancesOfColorInVertices();
	int getNumberOfVertices();
	int getVertexColorAtIndex(int index);
	void setVertexColorAtIndex(int index,int color);
	void changeAllVerteciesWithGivenColor(int colorToChange,int newColor);
	static shared_ptr<array<size_t, N>> createRandomColors(int maxColors){

		shared_ptr<array<size_t, N>> temp(new array<size_t, N>());

		for (int i = 0; i < N; i++)
		{
			(*temp)[i] = rand() % maxColors;
		}
		return temp;
	}
	
	static shared_ptr<list<pair<int, int>>> createEdgesList(){
	
		shared_ptr<list<pair<int, int>>> temp(new list<pair<int, int>>());

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N && j < i; j++)
			{
				if (givenGraph[i][j])
				{
					shared_ptr<pair<int, int>> newPair( new pair<int,int>(i, j));
					temp->push_back(*newPair);
				}
			}

		}

		return temp;
	}

};

