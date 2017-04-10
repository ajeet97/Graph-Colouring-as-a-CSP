#include <iostream>
#include <cstdlib>

#include "csp.cpp"

using namespace std;

class CSPForwardChecking : public CSP {
public:
    CSPForwardChecking(int colors, Graph graph);

    int selectUnassignmentVariable();
    vector<int> orderDomainValues(int var);

protected:
    bool inference(int var, int color);
};

CSPForwardChecking::CSPForwardChecking(int c, Graph g) : CSP(c, g) {}

int CSPForwardChecking::selectUnassignmentVariable() {
    for(int i=0; i<assignment.size(); i++)
        if(assignment[i] == 0)
            return i;
    return -1;
}

vector<int> CSPForwardChecking::orderDomainValues(int var) {
    if(var < 0 || var >= graph.adjList.size()) {
        cout<<"Exit from CSPForwardChecking::orderDomainValues"<<endl;
        exit(EXIT_FAILURE);
    }
    return domain[var];
}

bool CSPForwardChecking::inference(int var, int color) {

    /* Forward Checking */
    // Iterate through all neighbors of var
    for(int i=0; i<graph.adjList[var].size(); i++) {
        // Remove color=color from the domain of all neighbors
        for(vector<int>::iterator it = domain[graph.adjList[var][i]].begin(); it != domain[graph.adjList[var][i]].end(); it++){
            if(*it == color) {
                domain[graph.adjList[var][i]].erase(it);
                break;
            }
        }
    }
    /* End */

    // If any variable has only one value in domain then assign it
    for(int i=0; i<domain.size(); i++) {
        if(domain[i].size() == 1)
            addAssignment(i, domain[i][0], false);
        else if(domain[i].size() == 0)
            return false;
    }
    return true;
}

int main() {
    vector< vector<int> > adjList(7);
    adjList[0].push_back(1);
    adjList[0].push_back(2);
    adjList[1].push_back(0);
    adjList[1].push_back(2);
    adjList[1].push_back(3);
    adjList[2].push_back(0);
    adjList[2].push_back(1);
    adjList[2].push_back(3);
    adjList[2].push_back(4);
    adjList[2].push_back(5);
    adjList[3].push_back(1);
    adjList[3].push_back(2);
    adjList[3].push_back(4);
    adjList[4].push_back(2);
    adjList[4].push_back(3);
    adjList[4].push_back(5);
    adjList[5].push_back(2);
    adjList[5].push_back(4);

    Graph graph(adjList, true);

    CSPForwardChecking csp(3, graph);

    Assignment assignment = backtrack(csp);
    cout<<"Node\t->\tColor"<<endl;
    cout<<"---------------------"<<endl;
    for(int i=0; i<assignment.size(); i++) {
        cout<<(i+1)<<"\t->\t"<<assignment[i]<<endl;
    }

	return 0;
}

