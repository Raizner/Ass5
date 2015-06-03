#include "stdafx.h"
#include "Graph.h"


list<pair<int, int>> edgeslist;
array<int, N> densityVerticesNumber = {} ;
int T = 10;

Graph::Graph(array<array<bool, N>, N> &matrix, array<size_t, N> colors,int numberOfColors, list<pair<int, int>> &edges) :
	p_matrix(matrix), p_colors(colors), kColor(numberOfColors), p_edges(edges),maxDensity(maxDensityEdge)
{
	fixK();
	temprature  = T;
}



Graph::~Graph(void)
{
}
int Graph::getColorNumberOfMinumumApperancesOfColorInVertices(int whatColorToSkip){
	int tempMimimum = -1;
	int minumumColorNumber = -1;
	int *colorArray = new int[kColor];

	for (int i = 0; i < kColor; i++)
	{
		colorArray[i] = 0;
	}
	for (int i = 0; i < N; ++i)
	{
		++(colorArray[p_colors[i]]);
	}

	for (int i = 0; i < kColor; i++)
	{
		if (tempMimimum < colorArray[i] && whatColorToSkip != i)
		{
			tempMimimum = colorArray[i];
			minumumColorNumber = i;
		}
	}
	delete colorArray;
	return minumumColorNumber;

}

int Graph::getColorNumberOfMinumumApperancesOfColorInVertices(){

	int tempMimimum = -1;
	int minumumColorNumber = 0;
	int *colorArray = new int[kColor];

	for (int i = 0; i < kColor; i++)
	{
		colorArray[i] = 0;
	}
	for (int i = 0; i < N; ++i)
	{
		++(colorArray[p_colors[i]]);
	}
	tempMimimum = colorArray[0];

	for (int i = 0; i < kColor; i++)
	{
		if (tempMimimum <= colorArray[i])
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
	return p_colors[index];
}

void Graph::setVertexColorAtIndex(int index,int color,shared_ptr< array<size_t, N>> colors){
	if(colors != nullptr){
		(*colors)[index]=color;
	}
	else{
		p_colors[index]=color;
	}

}

void Graph::changeAllVerteciesWithGivenColor(int colorToChange,int newColor){
	for (size_t i = 0; i < N; i++)
	{
		if(p_colors[i] == colorToChange){
			p_colors[i] = newColor;
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

	cout << "Graph Colors -  " ;
	for (size_t i = 0; i < p_colors.size(); i++)
	{
		cout << i << ": " << p_colors[i] << "      ";
	}
	cout << ". K = " << kColor;

}


ostream &operator<<(ostream &out, const Graph &obj) {
	obj.printColoringVertices(out);
	return out;	
}


bool Graph::doWeWantToStop(){
	if ((kColor <= 2) && CalcFitness() == 0)
	{
		return true;
	}
	return false;
}


void Graph::RunLocalSearch(bool toShuffle, searchType type){
	temprature   =   T;
	int numberOfConflicts = 0;
	list<array<size_t,N>> *tabuMoves = new list<array<size_t,N>>();
	numberOfConflicts = this->findNumberConflictVertecies();

	if( numberOfConflicts > 0 ){
		if (toShuffle)
		{
			type = static_cast<searchType>(rand() % 3);
		}
		for (int i = 0; i < depth; i++)
		{
			switch (type)
			{

			case HillClimbing:
				hillClimbing();
				break;
			case TabuSearch:
				tabuSearch(*tabuMoves);
				break;
			case SimulatedAnnealing:
				simulatedAnneling();
				break;
			default:
				break;
			}
		}
		return;
	}
	else{
		cout << "Reducing GOOD graph: " << (Graph)*this << endl;
		reduceNumberOfColors();
		return ;
	}
}


void Graph::reduceNumberOfColors(){
	//First will try to reduce the color which appears the least
	if (kColor > 2)
	{
		int minColor = getColorNumberOfMinumumApperancesOfColorInVertices(); // finding the color that appear the minimum times
		int colorToPut = getColorNumberOfMinumumApperancesOfColorInVertices(minColor); // Same just ignoring the minColor color
		changeAllVerteciesWithGivenColor(minColor,colorToPut);
		if(minColor != kColor){
			changeAllVerteciesWithGivenColor(kColor,minColor);
		}
		reduceKColorVariable();
	}

	return;
}

int Graph::countNumberOfColorsInGraph(){

	set<int> listOfColors;
	for (int i = 0; i < N; i++)
	{
		listOfColors.insert(p_colors[i]);
	}
	return listOfColors.size();
}

set<int> Graph::getAllNeighboorsColors(size_t index){

	set<int> listOfColors;
	for (int i = 0; i < N; i++)
	{
		if (givenGraph[index][i])
		{
			listOfColors.insert(p_colors[i]);
		}
	}

	return listOfColors;

}

int Graph::getOtherColorOfNeighoorsNotIncludedForIndex(set<int> neighboorsColors, size_t indexColor){
	neighboorsColors.insert(indexColor);

	if (kColor == neighboorsColors.size())
	{
		return -1;
	}

	for (int i = 0; i < kColor; i++)
	{
		if (neighboorsColors.find(i) == neighboorsColors.end()) // if didn't find it returns iterator to end
		{
			return i;
		}
	}
	return -1;
}

void Graph::hillClimbing(){
	shared_ptr<list<pair<size_t, size_t>>> conflictVertices = findAllConflictVertecies();
	list<array<size_t, N>> neighboors ;
	for (list<pair<size_t, size_t>>::iterator it= (*conflictVertices).begin();  it != (*conflictVertices).end() ; it++)
	{

		pair<size_t, size_t> tempVertex = *it;
		//size_t indexOfVetex = densityVerticesNumber[tempVertex.first] <= densityVerticesNumber[tempVertex.second] ? tempVertex.first : tempVertex.second;
		size_t indexOfVetex = tempVertex.first;
		set<int> neighborsColors = getAllNeighboorsColors(indexOfVetex);
		int colorToPut = getOtherColorOfNeighoorsNotIncludedForIndex(neighborsColors,p_colors[indexOfVetex]);
		if(colorToPut == -1){
			colorToPut = (p_colors[indexOfVetex] - 1) % kColor;
		}
		shared_ptr<array<size_t, N>> newColoring ( new array<size_t, N>());
		for (int i = 0; i < N; i++)
		{
			(*newColoring)[i] = p_colors[i];
		}
		this->setVertexColorAtIndex(indexOfVetex,colorToPut,newColoring);
		neighboors.push_back(*newColoring);

	}
	int minFitness = N * N * 7;

	list<array<size_t, N>>::iterator bestFitnessIterator = neighboors.end() ;
	for (list<array<size_t, N>>::iterator it = neighboors.begin(); it != neighboors.end(); it++)
	{

		int tempFitness = calcFitness(&(*it));
		if(minFitness > tempFitness){
			minFitness = tempFitness;
			bestFitnessIterator=it;
		}


	}

	if (bestFitnessIterator != neighboors.end())
	{
		for (int i = 0; i < N; i++)
		{
			p_colors[i] = (*bestFitnessIterator)[i];
		}
	}


	kColor = countNumberOfColorsInGraph();
	fixK();
	return;
}

void Graph::tabuSearch(list<array<size_t,N>> & tabuList ){
	shared_ptr<list<pair<size_t, size_t>>> conflictVertices = findAllConflictVertecies();
	list<array<size_t, N>> neighboors ;
	for (list<pair<size_t, size_t>>::iterator it= (*conflictVertices).begin();  it != (*conflictVertices).end() ; it++)
	{

		pair<size_t, size_t> tempVertex = *it;
		//size_t indexOfVetex = densityVerticesNumber[tempVertex.first] <= densityVerticesNumber[tempVertex.second] ? tempVertex.first : tempVertex.second;
		size_t indexOfVetex = tempVertex.first;
		set<int> neighborsColors = getAllNeighboorsColors(indexOfVetex);
		int colorToPut = getOtherColorOfNeighoorsNotIncludedForIndex(neighborsColors,p_colors[indexOfVetex]);
		if(colorToPut == -1){
			colorToPut = (p_colors[indexOfVetex] - 1) % kColor;
		}
		shared_ptr<array<size_t, N>> newColoring ( new array<size_t, N>());
		for (int i = 0; i < N; i++)
		{
			(*newColoring)[i] = p_colors[i];
		}
		this->setVertexColorAtIndex(indexOfVetex,colorToPut,newColoring);
		pair<size_t, size_t> temp(p_colors[indexOfVetex], colorToPut);
		neighboors.push_back(*newColoring);

	}
	int minFitness = N * N * 7;

	list<array<size_t, N>>::iterator bestFitnessIterator = neighboors.end() ;
	for (list<array<size_t, N>>::iterator it = neighboors.begin(); it != neighboors.end(); it++)
	{
		int flagSkip=0;
		for (list<array<size_t,N>>::iterator listIterator = tabuList.begin(); listIterator != tabuList.end(); listIterator++)
		{
			if((*listIterator)==(*it))
			{
				flagSkip=1;
			}
		}
		if(!flagSkip){
			int tempFitness = calcFitness(&(*it));
			if(minFitness > tempFitness){
				minFitness = tempFitness;
				bestFitnessIterator=it;
			}
		}
	}
	if (bestFitnessIterator != neighboors.end())
	{
		for (int i = 0; i < N; i++)
		{
			p_colors[i] = (*bestFitnessIterator)[i];
		}
	}
	kColor = countNumberOfColorsInGraph();
	fixK();
	return;
}

list<array<size_t, N>>::iterator* Graph::findAnelingMember( list<array<size_t, N>> & neighboors){
	int myFitness = calcFitness();
	list<array<size_t, N>>::iterator *bestFitnessIterator = new list<array<size_t, N>>::iterator();
	*bestFitnessIterator = neighboors.end();
	double p = 0;
	for (list<array<size_t, N>>::iterator it = neighboors.begin(); it != neighboors.end(); it++)
	{
		int d = myFitness - calcFitness(&(*it));
		double temp_p = exp( -static_cast<double>(d) / static_cast<double>(temprature));
		if (temp_p > p)
		{
			p = temp_p;
			*bestFitnessIterator = it;
		}
	}
	return bestFitnessIterator;
}

void Graph::simulatedAnneling(){
	shared_ptr<list<pair<size_t, size_t>>> conflictVertices = findAllConflictVertecies();
	list<array<size_t, N>> neighboors ;
	for (list<pair<size_t, size_t>>::iterator it= (*conflictVertices).begin();  it != (*conflictVertices).end() ; it++)
	{
		pair<size_t, size_t> tempVertex = *it;
		//size_t indexOfVetex = densityVerticesNumber[tempVertex.first] <= densityVerticesNumber[tempVertex.second] ? tempVertex.first : tempVertex.second;
		size_t indexOfVetex = tempVertex.first;
		set<int> neighborsColors = getAllNeighboorsColors(indexOfVetex);
		int colorToPut = getOtherColorOfNeighoorsNotIncludedForIndex(neighborsColors,p_colors[indexOfVetex]);
		if(colorToPut == -1){
			colorToPut = (p_colors[indexOfVetex] - 1) % kColor;
		}
		shared_ptr<array<size_t, N>> newColoring ( new array<size_t, N>());
		for (int i = 0; i < N; i++)
		{
			(*newColoring)[i] = p_colors[i];
		}
		this->setVertexColorAtIndex(indexOfVetex,colorToPut,newColoring);
		neighboors.push_back(*newColoring);
	}
	int minFitness = N * N * 7;
	int bestFitnessAsNumber = 0;
	list<array<size_t, N>>::iterator bestFitnessIterator = neighboors.end() ;
	for (list<array<size_t, N>>::iterator it = neighboors.begin(); it != neighboors.end(); it++)
	{

		int tempFitness = calcFitness(&(*it));
		if(minFitness > tempFitness){
			minFitness = tempFitness;
			bestFitnessIterator = it;
			bestFitnessAsNumber = tempFitness;
		}
	}
	if (neighboors.size() > 0)
	{
		if(bestFitnessAsNumber >= calcFitness()) {	}
		else{
			list<array<size_t, N>>::iterator *temp = findAnelingMember(neighboors);
			bestFitnessIterator = *temp;
		}

		for (int i = 0; i < N; i++)
		{
			p_colors[i] = (*bestFitnessIterator)[i];
		}

		kColor = countNumberOfColorsInGraph();
		fixK();
	}


	temprature--;
	return;
}


shared_ptr<list<pair<size_t, size_t>>> Graph::findAllConflictVertecies(array<size_t, N> *input_colors){

	shared_ptr<list<pair<size_t, size_t>>> confictsVertices(new list<pair<size_t, size_t>>());

	array<size_t, N> *temp_colors;

	if (input_colors != nullptr)
	{
		temp_colors = input_colors;
	}
	else {
		temp_colors = &p_colors;

	}
	int conflictsNumbers[N] = {0};

	list<pair<int, int>>::iterator it = p_edges.begin();
	for (; it != p_edges.end(); ++it)
	{
		if ((*temp_colors)[it->first] == (*temp_colors)[it->second])
		{
			++((conflictsNumbers)[( densityVerticesNumber[it->first] < densityVerticesNumber[it->second] ) ? it->first : it->second ]);
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

int Graph::getKColor(){
	return kColor;
}

int Graph::findNumberConflictVertecies(){

	list<pair<size_t, size_t>> temp = * findAllConflictVertecies();

	return temp.size();
}


void Graph::reduceKColorVariable(){
	if(kColor>1){
		kColor-=1;
	}
	return;
}


void Graph::fixK(){
	bool *colorUsed = new bool[maxDensity];

	for (int i = 0; i < maxDensity; i++)
	{
		colorUsed[i] = false;
	}

	for (int i = 0; i < N; i++)
	{
		colorUsed[p_colors[i]] = true;
	}

	int realNumberOfColorsUsed = 0;

	for (int i = 0; i < maxDensity; i++)
	{
		colorUsed[i] ? realNumberOfColorsUsed++ : (void)0 ;
	}

	int beginPointer = 0;
	int endPointer = maxDensity - 1;

	while (beginPointer < endPointer)
	{
		if (colorUsed[beginPointer] == false)
		{
			if (colorUsed[endPointer] == true)
			{
				changeAllVerteciesWithGivenColor(endPointer, beginPointer);
				beginPointer++;
			}
			endPointer--;
		}
		else {
			beginPointer++;
		}
	}
	kColor = realNumberOfColorsUsed;

	delete colorUsed;
	return;
}