/*
 * Nodes.h
 *
 *  Created on: Mar 13, 2021
 *      Author: oguzkerem
 */

#ifndef NODES_H_
#define NODES_H_

#include <iostream>
#include <list>
#include <iterator>

using namespace std;

#define NUM_OF_COLUMN 3
#define NUM_OF_ROW 3
#define SIZE_OF_PUZZLE NUM_OF_COLUMN*NUM_OF_ROW

class Nodes
{
	public:
		list<Nodes*> children;
		Nodes * parent = NULL;

		int goalState[SIZE_OF_PUZZLE] = {0};
		int nodeArray[SIZE_OF_PUZZLE] = {0};
		int indexOfZero = 0;
		int depthOfNode = 0;

		Nodes(int* initialCondition);
		Nodes(int* initialCondition, Nodes *_parent, int _depth) {
			setNodeArray(initialCondition);
			parent = _parent;
			depthOfNode = _depth;
		  }
		Nodes(int* initialCondition, int* finalCondition, Nodes *_parent, int _depth) {
			setNodeArray(initialCondition);
			setGoalState(finalCondition);
			parent = _parent;
			depthOfNode = _depth;
		  }
		void printPuzzle();
		void printPuzzle(int source[]);
		bool isThisGoalNode();
		bool isTheSamePuzzle(int source[]);


		void copyPuzzle(int source[], int destination[]);
		void moveZeroUp(int input[], int index);
		void moveZeroDown(int input[], int index);
		void moveZeroLeft(int input[], int index);
		void moveZeroRight(int input[], int index);
		void createNextNode();
		void setNodeArray(int input[]);
		void setGoalState(int input[]);

};
#endif /* NODES_H_ */
