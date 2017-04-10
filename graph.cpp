#include "graph.h"

Graph::Graph() {}

Graph::Graph(std::vector< std::vector<int> > al, bool aLorM){
    if(aLorM)
        adjList = al;
    else {
        std::vector<int> temp;
        for(int i=0; i<al.size(); i++) {
            temp.clear();
            for(int j=0; j<al.size(); j++)
                if(al[i][j])
                    temp.push_back(j);
            adjList.push_back(temp);
        }
    }
}

// Graph::Graph(std::string adjMatrixFileName) {

// }

Graph::~Graph() {
	for(int i=0; i<adjList.size(); i++)
		adjList[i].clear();
	adjList.clear();
}
