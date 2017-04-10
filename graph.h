#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <vector>

class Graph {
public:
    std::vector< std::vector<int> > adjList;

    Graph();
	Graph(std::vector< std::vector<int> > adjListMatrix, bool adjList); // adjList->true if adjList is given, or false if adjMatrix is given
	// Graph(std::string adjMatrixFileName);
	~Graph();
};

#endif //_GRAPH_H_
