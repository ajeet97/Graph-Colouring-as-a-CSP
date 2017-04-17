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

Graph::Graph(std::string adjMatrixFileName) {
    std::ifstream in;
    in.open(adjMatrixFileName.c_str());
    if(!in.is_open()) {
        std::cout<<"Could not open "<<adjMatrixFileName<<" file"<<std::endl;
        exit(EXIT_FAILURE);
    }
    int nNodes;
    in>>nNodes;

    adjList.resize(nNodes);
    for(int i=0; i<nNodes; i++) {
        for(int j=0; j<nNodes; j++) {
            int x;
            in>>x;
            if(x == 1)
                adjList[i].push_back(j);
        }
    }
    in.close();
}

Graph::~Graph() {
	for(int i=0; i<adjList.size(); i++)
		adjList[i].clear();
	adjList.clear();
}

void Graph::generateRandom(int nNodes) {
    std::vector< std::vector<bool> > adjMat(nNodes, std::vector<bool>(nNodes, false));
    srand(time(NULL));
    for(int i=0; i<nNodes; i++) {
        for(int j=i+1; j<nNodes; j++) {
            int rnd = rand() % 2;
            if(rnd == 1) {
                adjMat[i][j] = adjMat[j][i] = true;
            }
        }
    }

    std::ofstream out;
    out.open("adjMatrix.txt");
    if(!out.is_open()) {
        std::cout<<"Could not open adjMatrix.txt"<<std::endl;
        exit(EXIT_FAILURE);
    }
    out<<nNodes<<std::endl;
    for(int i=0; i<nNodes; i++) {
        for(int j=0; j<nNodes; j++) {
            if(adjMat[i][j])
                out<<"1 ";
            else
                out<<"0 ";
        }
        out<<"\n";
    }
    out.close();
}
