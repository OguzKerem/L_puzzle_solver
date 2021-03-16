/*
 * bfs.h
 *
 *  Created on: Mar 14, 2021
 *      Author: oguzkerem
 */

#ifndef BFS_H_
#define BFS_H_

#include <iostream>
#include <list>
#include "Nodes.h"

using namespace std;

class bfs {
public:
	bfs();
	virtual ~bfs();

	list<Nodes*> executeBfs(Nodes *initial_condition, bool* solution_exists);
	//void executeBfs(Nodes initial_condition);
	bool isItInTheList(list<Nodes*> checkToList, Nodes* checkNode);
	void printfParents(Nodes* Node);
	void printSolutionPath(Nodes *goalNode);
	void findSolutionPath(list<Nodes*> path, Nodes* goalNode);
};

#endif /* BFS_H_ */
