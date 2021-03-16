/*
 * Nodes.cpp
 *
 *  Created on: Mar 13, 2021
 *      Author: oguzkerem
 */

#include "Nodes.h"

using namespace std;


void Nodes::setNodeArray(int input[])
{
	for(int i = 0; i<SIZE_OF_PUZZLE; i++){
		this->nodeArray[i] = input[i];
	}
}

void Nodes::setGoalState(int input[])
{
	for(int i = 0; i<SIZE_OF_PUZZLE; i++){
		this->goalState[i] = input[i];
	}
}

//name tells everything
void Nodes::printPuzzle()
{
	cout << "\n";
	for(int i = 0; i < SIZE_OF_PUZZLE; i++){
		cout << nodeArray[i] << " ";
		if(i%NUM_OF_COLUMN == NUM_OF_COLUMN - 1)
			cout << "\n";
	}
	cout << "\n";
}

void Nodes::printPuzzle(int source[])
{
	cout << "\n";
	for(int i = 0; i < SIZE_OF_PUZZLE; i++){
		cout << source[i] << " ";
		if(i%NUM_OF_COLUMN == NUM_OF_COLUMN - 1)
			cout << "\n";
	}
	cout << "\n";
}


// test for reaching the goal node
bool Nodes::isThisGoalNode()
{
	int goalPoint[SIZE_OF_PUZZLE] = {1,2,3,
								      4,5,6,
									  7,8,0};

	bool goalNodeFlag = true;
//	cout << "node array for goal test: \n";
//	printPuzzle(nodeArray);
	for(int i = 0; i<SIZE_OF_PUZZLE; i++){
		if(nodeArray[i] != goalPoint[i]){
			goalNodeFlag = false;
		}
	}
//	cout << "goal test result " << goalNodeFlag << endl;
	return goalNodeFlag;
}

bool Nodes::isTheSamePuzzle(int source[])
{
	bool same = true;
	for(int i = 0; i< SIZE_OF_PUZZLE; i++){
		if(nodeArray[i] != source[i]){
			same = false;
		}
	}
	return same;

}
//copies original puzzle array not to modify original
void Nodes::copyPuzzle(int source[], int destination[])
{
	for(int i = 0; i< SIZE_OF_PUZZLE; i++){
		destination[i] = source[i];
	}
}
//legal moves
void Nodes::moveZeroUp(int input[], int index)
{
	// rule for moving up
	if(index > NUM_OF_COLUMN -1){
		int tmpArray[SIZE_OF_PUZZLE] = {0};

		copyPuzzle(input, tmpArray);

		int tmpIndexVal = tmpArray[index];
		tmpArray[index] = tmpArray[index - NUM_OF_COLUMN];
		tmpArray[index - NUM_OF_COLUMN] = tmpIndexVal;
		//create new child and ad it to children list
		Nodes* child = new Nodes(tmpArray, this, this->depthOfNode+1);
		child->depthOfNode = this->depthOfNode + 1;
		children.push_back(child);
	}
}
void Nodes::moveZeroDown(int input[], int index)
{
	// rule for moving down
	if(index < SIZE_OF_PUZZLE - NUM_OF_COLUMN ){
		int tmpArray[SIZE_OF_PUZZLE] = {0};

		copyPuzzle(input, tmpArray);

		int tmpIndexVal = tmpArray[index];
		tmpArray[index] = tmpArray[index + NUM_OF_COLUMN];
		tmpArray[index + NUM_OF_COLUMN] = tmpIndexVal;

		Nodes* child = new Nodes(tmpArray, this, this->depthOfNode+1);
		children.push_back(child);
	}
}
void Nodes::moveZeroLeft(int input[], int index)
{
	// rule for moving left
	if(index % NUM_OF_COLUMN > 0){
		int tmpArray[SIZE_OF_PUZZLE] = {0};

		copyPuzzle(input, tmpArray);

		int tmpIndexVal = tmpArray[index];
		tmpArray[index] = tmpArray[index - 1];
		tmpArray[index - 1] = tmpIndexVal;

		Nodes* child = new Nodes(tmpArray, this, this->depthOfNode+1);
		child->depthOfNode = this->depthOfNode + 1;
		children.push_back(child);
	}
}
void Nodes::moveZeroRight(int input[], int index)
{
	// rule for moving right
	if(index % NUM_OF_COLUMN < NUM_OF_COLUMN -1){
		int tmpArray[SIZE_OF_PUZZLE] = {0};

		copyPuzzle(input, tmpArray);

		int tmpIndexVal = tmpArray[index];
		tmpArray[index] = tmpArray[index + 1];
		tmpArray[index + 1] = tmpIndexVal;

		Nodes* child = new Nodes(tmpArray, this, this->depthOfNode+1);
		child->depthOfNode = this->depthOfNode + 1;
		children.push_back(child);
	}
}

void Nodes::createNextNode()
{
	//find index of zero
	int indexOfZero = 0;
	for(int i = 0; i < SIZE_OF_PUZZLE; i++){
		if(nodeArray[i] == 0){
			indexOfZero = i;
			break;
		}
	}
	//make all legal move
	moveZeroUp( nodeArray, indexOfZero);
	moveZeroDown( nodeArray, indexOfZero);
	moveZeroLeft( nodeArray, indexOfZero);
	moveZeroRight( nodeArray, indexOfZero);
}






