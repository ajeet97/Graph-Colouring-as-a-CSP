#include "csp.h"

CSP::CSP(int c, Graph g) : colors(c), graph(g) {
    std::vector<int> dmn(c);
    for(int i=0; i<dmn.size(); i++)
        dmn[i] = i+1;

    domain.resize(g.adjList.size(), dmn);
    assignment.resize(g.adjList.size(), 0);
}

CSP::~CSP() {
    for(int i=0; i<domain.size(); i++)
        domain[i].clear();
    domain.clear();
    assignment.clear();
}

bool CSP::isAssignmentComplete() {
    for(int i=0; i<assignment.size(); i++)
        if(assignment[i] == 0)
            return false;
    return true;
}

bool CSP::isAssignmentSolution() {
    for(int i=0; i<graph.adjList.size(); i++)
        for(int j=0; j<graph.adjList[i].size(); j++)
            if(assignment[i] == assignment[graph.adjList[i][j]])
                return false;
    return true;
}

bool CSP::isConsistent(int var, int value) {
    // If any neighbor of var has the color as value the value is not consistent
    for(int nbr : graph.adjList[var])
        if(assignment[nbr] == value)
            return false;
    return true;
}

bool CSP::addAssignment(int var, int color, bool infer) {
    if(var < 0 || var >= graph.adjList.size() || color <= 0 || color > colors) {
        std::cout<<"Exit from CSP::addAssignment"<<std::endl;
//        std::cout<<"var: "<<var<<"\t"<<"color: "<<color<<"\t"<<"infer: "<<infer<<"\n";
        exit(EXIT_FAILURE);
    }

    assignment[var] = color;

    if(infer)
        return inference(var, color);
    return true;
}

template<typename T>
Assignment backtrack(T c) {
    static_assert(std::is_base_of<CSP, T>::value, "T must inherit from CSP!");

    if(c.isAssignmentComplete())
        return c.assignment;

    Assignment failure(c.assignment.size(), -1);

    int var = c.selectUnassignmentVariable();

    for(int value : c.orderDomainValues(var)) {
        if(c.isConsistent(var, value)) {
            T ccopy = c;
            bool assignResult = ccopy.addAssignment(var, value, true);
            if(assignResult) {
                Assignment result = backtrack(ccopy);
                // if result is not failure
                if(result[0] != -1)
                    return result;
            }
        }
    }
    return failure;
}
