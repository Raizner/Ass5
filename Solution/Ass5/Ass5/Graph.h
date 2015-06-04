#pragma once
#include <array>
#include <list>
#include <memory>
#include <tuple>
#include <iostream>
#include <ostream>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace std;
extern int maxDensityEdge;
const size_t N = 10;
extern list<pair<int, int>> edgeslist;
extern array<int, N> densityVerticesNumber  ;
extern array<array<bool, N>, N> givenGraph;
extern int T;

namespace {

	
	const size_t depth = 2; 
	//array<array<bool, N>, N> givenGraph = {{
	//	{0,1,0,0,0},
	//	{1,0,1,0,1},
	//	{0,1,0,1,0},
	//	{0,0,1,0,1},
	//	{0,1,0,1,0},
	//}};

}

enum searchTypeEnum {
	HillClimbing = 0,
	TabuSearch,
	SimulatedAnnealing

};

typedef enum searchTypeEnum searchType;


class Graph
{
public :
	array<array<bool, N>, N> &p_matrix;
	list<pair<int, int>> &p_edges;
	array<size_t, N> p_colors;
	int kColor;
	void hillClimbing();
	void tabuSearch(list<array<size_t,N>> & tabuList );
	void simulatedAnneling();
	int calcFitness(array<size_t, N> *input = nullptr);
	int findNumberConflictVertecies();
	shared_ptr<list<pair<size_t, size_t>>> findAllConflictVertecies( array<size_t, N> *input_colors = nullptr);
	void printColoringVertices(ostream &out) const;
	const int maxDensity;
	void reduceNumberOfColors();
	void fixK();
	set<int> getAllNeighboorsColors(size_t);
	int getOtherColorOfNeighoorsNotIncludedForIndex(set<int>, size_t indexColor);
	int countNumberOfColorsInGraph();

	int temprature;

	Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors,int numberOfColors, list<pair<int, int>> &edges);
	~Graph(void);
	void RunLocalSearch(bool toShuffle = true, searchType type = HillClimbing);
	int CalcFitness();
	int getColorNumberOfMinumumApperancesOfColorInVertices();
	int getColorNumberOfMinumumApperancesOfColorInVertices(int whatColorToSkip);
	int getNumberOfVertices();
	int getVertexColorAtIndex(int index);
	void setVertexColorAtIndex(int index,int color,shared_ptr< array<size_t, N>> colors=nullptr);
	void changeAllVerteciesWithGivenColor(int colorToChange,int newColor);
	bool doWeWantToStop();
	void reduceKColorVariable();
	int getKColor();
	list<array<size_t, N>>::iterator* findAnelingMember( list<array<size_t, N>> & neighboors);




	friend ostream &operator<<(ostream &out, const Graph &obj);

	static shared_ptr<array<size_t, N>> createRandomColors(int maxColors){

		shared_ptr<array<size_t, N>> temp(new array<size_t, N>());

		for (int i = 0; i < N; i++)
		{
			(*temp)[i] = rand() % maxColors;
		}
		return temp;
	}

	static int FindMaxDensityEdge(){
		int maximum = 0;
		for (int i = 0; i < N; ++i)
		{
			int counterOfEdges=0;
			for (int j = 0; j < N; ++j)
			{
				if(givenGraph[i][j]==1){
					++counterOfEdges;
				}
			}

			densityVerticesNumber[i] = counterOfEdges;

			maximum = (counterOfEdges > maximum ) ? counterOfEdges : maximum;
		}
		return maximum;
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

