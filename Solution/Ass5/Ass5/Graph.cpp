#include "stdafx.h"
#include "Graph.h"


Graph::Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors,int numberOfColors, list<pair<int, int>> &edges) :
	p_matrix(matrix), p_colors(colors), kColor(numberOfColors), p_edges(edges),maxDensity(maxDensityEdge)
{

}



Graph::~Graph(void)
{
}


int Graph::getColorNumberOfMinumumApperancesOfColorInVertices(){

	int tempMimimum = -1;
	int minumumColorNumber = -1;
	int *colorArray = new int[kColor];


	for (int i = 0; i < N; ++i)
	{
		++(colorArray[p_colors[i]]);
	}

	for (int i = 0; i < N; i++)
	{
		if (tempMimimum < colorArray[i])
		{
			tempMimimum = colorArray[i];
			minumumColorNumber = i;
		}
	}
	delete colorArray;
	return minumumColorNumber;
}


int Graph::getNumberOfVertices(){
	return p_colors.size();
}

int Graph::getVertexColorAtIndex(int index){
	return p_colors.at(index);
}

void Graph::setVertexColorAtIndex(int index,int color){
	p_colors[index]=color;

}

void Graph::changeAllVerteciesWithGivenColor(int colorToChange,int newColor){
	for (size_t i = 0; i < p_colors.size(); i++)
	{
		if(p_colors.at(i)==colorToChange){
			p_colors.at(i)=newColor;
		}
	}
}

int Graph::calcFitness(array<size_t, N> *input){

	int fitness = 0;
	array<size_t, N> temp_color;
	if (input == nullptr)
	{
		temp_color = p_colors;
	} else
	{
		temp_color = * input;
	}
	list<pair<int, int>>::iterator it = p_edges.begin();
	for (; it != p_edges.end(); ++it)
	{
		if (temp_color[it->first] == temp_color[it->second])
		{
			fitness += 7;
		}
	}
	return fitness;
}

int Graph::CalcFitness(){

	return calcFitness(nullptr);
}


void Graph::printColoringVertices(ostream &out) const{

	cout << "Graph Colors: " ;
	for (size_t i = 0; i < p_colors.size(); i++)
	{
		cout << i << ": " << p_colors[i] << ", ";
	}
	cout << ". K = " << kColor;

}


ostream &operator<<(ostream &out, const Graph &obj) {
	obj.printColoringVertices(out);
	return out;	
}


bool Graph::doWeWantToStop(){
	if (kColor <= maxDensity)
	{
		return true;
	}
	return false;
}


void Graph::RunLocalSearch(bool toShuffle, searchType type){
	int numberOfConflicts = 0;
	numberOfConflicts=this->findNumberConflictVertecies();
	if (toShuffle)
	{
		type = static_cast<searchType>(rand() % 3);
	}
	switch (type)
	{

	case HillClimbing:
		hillClimbing();
		break;
	case TabuSearch:
		tabuSearch();
		break;
	case SimulatedAnnealing:
		simulatedAnneling();
		break;
	default:
		break;
	}
}


void Graph::hillClimbing(){



}

void Graph::tabuSearch(){




}

void Graph::simulatedAnneling(){



}


shared_ptr<list<pair<size_t, size_t>>> Graph::findAllConflictVertecies(){
	shared_ptr<list<pair<size_t, size_t>>> confictsVertices(new list<pair<size_t, size_t>>());

	int conflictsNumbers[N];

	list<pair<int, int>>::iterator it = p_edges.begin();
	for (; it != p_edges.end(); ++it)
	{
		if (p_colors[it->first] == p_colors[it->second])
		{
			++((conflictsNumbers)[ densityVerticesNumber[it->first] < densityVerticesNumber[it->second] ? it->first : it->second ]);
		}
	}

	for (int i = 0; i < N; i++)
	{
		if (conflictsNumbers[i] > 0)
		{
			shared_ptr<pair<size_t, size_t>> newPair( new pair<size_t,size_t>(i, conflictsNumbers[i]));

			(*confictsVertices).push_back(*newPair);
		}
	}

	return confictsVertices;

}


int Graph::findNumberConflictVertecies(){

	list<pair<size_t, size_t>> temp = * findAllConflictVertecies();

	return temp.size();
}