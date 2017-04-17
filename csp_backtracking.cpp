#include <iostream>
#include <cstdlib>
#include <chrono>

#include "csp.cpp"

using namespace std;

class CSPSimpleBacktracking : public CSP {
public:
    CSPSimpleBacktracking(int colors, Graph graph);

    int selectUnassignmentVariable();
    vector<int> orderDomainValues(int var);

protected:
    bool inference(int var, int color);
private:
    bool selectFirstVariable;
};

CSPSimpleBacktracking::CSPSimpleBacktracking(int c, Graph g) : CSP(c, g), selectFirstVariable(true) {}

int CSPSimpleBacktracking::selectUnassignmentVariable() {
    if(selectFirstVariable) {
        selectFirstVariable = false;
        return degreeHeuristic(assignment, graph);
    }
    return minimumRemainingValue(assignment, domain);
}

vector<int> CSPSimpleBacktracking::orderDomainValues(int var) {
    if(var < 0 || var >= graph.adjList.size()) {
        cout<<"Exit from CSPSimpleBacktracking::orderDomainValues"<<endl;
        exit(EXIT_FAILURE);
    }
    return leastConstrainingValue(var, domain, graph);
}

bool CSPSimpleBacktracking::inference(int var, int color) {
    // If any variable has only one value in domain then assign it
    for(int i=0; i<domain.size(); i++) {
        if(domain[i].size() == 1)
            addAssignment(i, domain[i][0], false);
        else if(domain[i].size() == 0)
            return false;
    }
    return true;
}

void pa(Assignment a) {
    for(int i=0; i<a.size(); i++)
        cout<<a[i]<<", ";
    cout<<endl;
}

int main() {
//    vector< vector<int> > adjList(7);
//    adjList[0].push_back(1);
//    adjList[0].push_back(2);
//    adjList[1].push_back(0);
//    adjList[1].push_back(2);
//    adjList[1].push_back(3);
//    adjList[2].push_back(0);
//    adjList[2].push_back(1);
//    adjList[2].push_back(3);
//    adjList[2].push_back(4);
//    adjList[2].push_back(5);
//    adjList[3].push_back(1);
//    adjList[3].push_back(2);
//    adjList[3].push_back(4);
//    adjList[4].push_back(2);
//    adjList[4].push_back(3);
//    adjList[4].push_back(5);
//    adjList[5].push_back(2);
//    adjList[5].push_back(4);

    Graph graph("adjMatrix.txt");

    CSPSimpleBacktracking csp(3, graph);

    auto start = std::chrono::high_resolution_clock::now();
    Assignment assignment = backtrack(csp);
    auto finish = std::chrono::high_resolution_clock::now();
    float exeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count(); // in ns

    csp.assignment = assignment;

    if(csp.isAssignmentSolution()) {
        cout<<"Node\t->\tColor"<<endl;
        cout<<"---------------------"<<endl;
        for(int i=0; i<assignment.size(); i++) {
            cout<<(i+1)<<"\t->\t"<<assignment[i]<<endl;
        }
    } else {
        cout<<"Solution does not exist."<<endl;
    }

    cout<<"\nExecution time: "<<exeTime / 1000000<<" ms.\n";

	return 0;
}
