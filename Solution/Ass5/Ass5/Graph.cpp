#include "stdafx.h"
#include "Graph.h"


Graph::Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors,int numberOfColors) :
	p_matrix(matrix), p_colors(colors), kColor(numberOfColors)
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
