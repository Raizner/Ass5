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

	int minumimNumber = -1;


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