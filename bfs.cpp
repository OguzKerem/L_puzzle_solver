/*
 * bfs.cpp
 *
 *  Created on: Mar 14, 2021
 *      Author: oguzkerem
 */

#include "bfs.h"
#include<unistd.h>
#include <algorithm>

bfs::bfs() {
	// TODO Auto-generated constructor stub

}

bfs::~bfs() {
	// TODO Auto-generated destructor stub
}


list<Nodes*> bfs::executeBfs(Nodes *initial_condition, bool* solution_exists)
{
	list<Nodes*> solution;
	list<Nodes*> openList;
	list<Nodes*>::iterator itForOpenList;
	list<Nodes>::iterator itForScanningChildren;
	list<Nodes*> closedList;
	list<Nodes>::iterator itForClosedList;
	bool solutionExistFlag = false;

	//put the start point to openList
	openList.push_back(initial_condition);
	//start feeding and consuming openList
	while(openList.size() > 0 && solutionExistFlag == false){

		//FIFO implementation starts
		Nodes *currentNode = openList.front();
		closedList.push_back(currentNode);
		openList.pop_front();
		//FIFO implementation finishes

		//apply all available moves to node to create new children
		currentNode->createNextNode();

		//scan newly created children for goal state
		for(auto itForScanningChildren : currentNode->children ){
			Nodes *currentChild = itForScanningChildren;

			if(currentChild->isThisGoalNode() == true){
				solutionExistFlag = true;
				findSolutionPath(solution, currentChild);
			}else{
				if(currentChild->depthOfNode > 4){
					*solution_exists = solutionExistFlag;
					return solution;
				}

				if(!isItInTheList(openList, currentChild)){
					if(!isItInTheList(closedList, currentChild)){
						openList.push_back(currentChild);
					}else{
						//something ??
					}
				}else{
//					//something ??
				}
			}
		}
	}
	*solution_exists = solutionExistFlag;
	return solution;
}


void bfs::findSolutionPath(list<Nodes*> path, Nodes* goalNode)
{

	Nodes * currentNode = goalNode;
	path.push_back(currentNode);

	while(currentNode->parent != NULL){
		currentNode = currentNode->parent;
		path.push_back(currentNode);
	}

	  reverse(path.begin(), path.end());
	  cout << "PATH TO SOLUTION:" << endl;
	  for (auto i : path) {
	    i->printPuzzle();
	    cout << endl;
	  }
}

//check if particular list contains particular item
bool bfs::isItInTheList(list<Nodes*> checkToList, Nodes* checkNode)
{
	  bool contains = false;
	  while (!checkToList.empty()) {
	    if (checkToList.front()->nodeArray == checkNode->nodeArray)
	    	contains = true;
	    checkToList.pop_front();
	  }
	  return contains;
}


