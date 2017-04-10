#ifndef _CSP_H_
#define _CSP_H_

#include <iostream>
#include <vector>
#include <cstdlib>

#include "graph.cpp"

typedef std::vector<int> Assignment;

class CSP {
public:
    int colors;
    Graph graph;
    std::vector<std::vector<int>> domain;
    Assignment assignment; // 0 -> unassigned

    CSP(int colors, Graph graph);
    virtual ~CSP();

    bool isAssignmentComplete();
    bool isAssignmentSolution();
    bool isConsistent(int var, int value);
    bool addAssignment(int var, int color, bool infer);

    virtual int selectUnassignmentVariable() = 0;
    virtual std::vector<int> orderDomainValues(int var) = 0;

protected:
    virtual bool inference(int var, int color) = 0;
};

template<typename T>
Assignment backtrack(T csp);

#endif // _CSP_H_
