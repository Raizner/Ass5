#include "stdafx.h"
#include "Graph.h"


Graph::Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors,int numberOfColors, list<pair<int, int>> &edges) :
	p_matrix(matrix), p_colors(colors), kColor(numberOfColors), p_edges(edges)
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


void Graph::changeAllVerteciesWithGivenColor(int colorToChange,int newColor){
	for (int i = 0; i < p_colors.size(); i++)
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