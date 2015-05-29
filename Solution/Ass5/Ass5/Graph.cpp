#include "stdafx.h"
#include "Graph.h"


Graph::Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors) :
	p_matrix(matrix), p_colors(colors)
{
}


Graph::~Graph(void)
{
}
