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
