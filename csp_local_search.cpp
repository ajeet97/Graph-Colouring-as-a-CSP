#include <iostream>
#include <cstdlib>
#include <ctime>

#include "csp.cpp"

using namespace std;

void pa(Assignment a) {
    for(int i=0; i<a.size(); i++)
        cout<<a[i]<<", ";
    cout<<endl;
}

class CSPLocalSearch : public CSP {
public:
    vector<int> conflicted;

    CSPLocalSearch(int colors, Graph graph);

    void assignRandomValues();
    int selectUnassignmentVariable();
    int conflictedVariable();
    vector<int> orderDomainValues(int var);
    int minConflictsValue(int var);
    void updateConflicts();

protected:
    bool inference(int var, int color);

private:
    void assignRandomValueUtility(int v, int value);
};

bool CSPLocalSearch::inference(int var, int color){}

CSPLocalSearch::CSPLocalSearch(int c, Graph g) : CSP(c, g) {
    conflicted.resize(g.adjList.size(), 0);
}

void CSPLocalSearch::assignRandomValues() {
    srand(time(NULL));

//    int tmp[] = {3, 1, 3, 2, 3, 1, 1};
//    int tmp[] = {1, 2, 3, 1, 1, 1, 1};
//    for(int i=0; i<7; i++)
//        assignment[i] = tmp[i];
//
//    updateConflicts();
//    return;

    bool allAssigned;
    do {
        allAssigned = true;
        for(int i=0; i<assignment.size(); i++) {
            if(assignment[i] == 0) {
                assignRandomValueUtility(i, rand() % colors + 1);
                allAssigned = false;
                break;
            }
        }
    }while(!allAssigned);

    updateConflicts();
}

void CSPLocalSearch::assignRandomValueUtility(int v, int value) {
    if(assignment[v] != 0) return;

    assignment[v] = value;

//    value = value + 1;
//    if(value > colors)
//        value = 1;
    value = rand() % colors + 1;
    if(value == assignment[v])
        value = (value == colors) ? 1 : value + 1;

    for(int i=0; i<graph.adjList[v].size(); i++)
        assignRandomValueUtility(graph.adjList[v][i], value);
}

void CSPLocalSearch::updateConflicts() {
    conflicted.resize(graph.adjList.size(), 0);

    for(int i=0; i<graph.adjList.size(); i++) {
        for(int j=0; j<graph.adjList[i].size(); j++) {
            if(assignment[i] == assignment[graph.adjList[i][j]]) {
                conflicted[i]++;
                conflicted[graph.adjList[i][j]]++;
            }
        }
    }
}

int CSPLocalSearch::selectUnassignmentVariable() {
    for(int i=0; i<assignment.size(); i++)
        if(assignment[i] == 0)
            return i;
    return -1;
}

int CSPLocalSearch::conflictedVariable() {
    vector<int> cnf;
//    int maxC = 0;
//    for(int i=0; i<conflicted.size(); i++)
//        if(maxC < conflicted[i])
//            maxC = conflicted[i];
//    for(int i=0; i<conflicted.size(); i++)
//        if(conflicted[i] == maxC)
//            cnf.push_back(i);
    for(int i=0; i<conflicted.size(); i++)
        if(conflicted[i] > 0)
            cnf.push_back(i);
    return cnf[rand() % cnf.size()];
}

vector<int> CSPLocalSearch::orderDomainValues(int var) {
    if(var < 0 || var >= graph.adjList.size()) {
        cout<<"Exit from CSPLocalSearch::orderDomainValues"<<endl;
        exit(EXIT_FAILURE);
    }
    return domain[var];
}

int CSPLocalSearch::minConflictsValue(int var) {
    vector<int> mcv;
    vector<int> nc(colors, graph.adjList.size()+1);
    for(int value: domain[var])
        nc[value-1] = 0;
    for(int value: domain[var])
        for(int i=0; i<graph.adjList[var].size(); i++)
            if(assignment[graph.adjList[var][i]] == value)
                nc[value-1]++;

    int minC = graph.adjList.size();

//    cout<<"conflicts:\n";
    for(int i=0; i<nc.size(); i++) {
        if(minC > nc[i])
            minC = nc[i];
//        cout<<i<<": "<<nc[i]<<endl;
    }

//    cout<<"minC: "<<minC<<endl;

//    cout<<"mcv:\n";
    int k=0;
    for(int i=0; i<nc.size(); i++) {
        if(nc[i] == minC) {
            mcv.push_back(i+1);
//            cout<<mcv[k++]<<", ";
        }
    }
//    cout<<endl;

    return mcv[rand() % mcv.size()];
}

Assignment minConflicts(CSPLocalSearch *csp, int max_steps) {
    Assignment failure(csp->assignment.size(), -1);

    for(int i=0; i<max_steps; i++) {
        if(csp->isAssignmentSolution()) {
            cout<<"Found solution at step "<<(i+1)<<".\n";
            return csp->assignment;
        }
        int var = csp->conflictedVariable();
//        cout<<"i: "<<i<<endl;
//        cout<<"var: "<<var<<endl;
        int value = csp->minConflictsValue(var);
//        cout<<"value: "<<value<<endl<<endl;
        csp->addAssignment(var, value, false);
//        cout<<"Assignment: ";
//        pa(csp->assignment);
        csp->updateConflicts();
    }
    return failure;
}

Assignment localSearch(CSPLocalSearch &csp, int max_steps) {
    csp.assignRandomValues();

    cout<<"Initial random assignment:\n";
    pa(csp.assignment);

    return minConflicts(&csp, max_steps);
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

    CSPLocalSearch csp(3, graph);

//    cout<<"adjList:"<<endl;
//    for(int i=0; i<csp.graph.adjList.size(); i++) {
//        cout<<i<<": ";
//        for(int j=0; j<csp.graph.adjList[i].size(); j++) {
//            cout<<csp.graph.adjList[i][j]<<", ";
//        }
//        cout<<endl;
//    }

    int max_steps = 50;
    Assignment assignment = localSearch(csp, max_steps);

    if(csp.isAssignmentSolution()) {
        cout<<"Node\t->\tColor"<<endl;
        cout<<"---------------------"<<endl;
        for(int i=0; i<assignment.size(); i++) {
            cout<<(i+1)<<"\t->\t"<<assignment[i]<<endl;
        }
    } else {
        cout<<"Could not find solution in "<<max_steps<<" steps."<<endl;
    }
	return 0;
}

