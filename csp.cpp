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

int degreeHeuristic(const Assignment &a, const Graph &g) {
    std::vector<int> degrees(a.size(), 0);
    int maxDegree = 0;
    for(int i=0; i<g.adjList.size(); i++) {
        degrees[i] = g.adjList[i].size();
        if(a[i] == 0 && maxDegree < degrees[i])
            maxDegree = degrees[i];
    }

    std::vector<int> maxDegreeVars;
    for(int i=0; i<a.size(); i++)
        if(a[i] == 0 && degrees[i] == maxDegree)
            maxDegreeVars.push_back(i);

    if(maxDegreeVars.size() == 0)
        return -1;

    srand(time(NULL));
    return maxDegreeVars[rand() % maxDegreeVars.size()];
}

int minimumRemainingValue(const Assignment &assignment, const std::vector< std::vector<int> > &domain) {
    int mrv = domain.size()+1;
    for(int i=0; i<domain.size(); i++)
        if(assignment[i] == 0 && mrv > domain[i].size())
            mrv = domain[i].size();

    std::vector<int> minRemainingVars;
    for(int i=0; i<assignment.size(); i++)
        if(assignment[i] == 0 && domain[i].size() == mrv)
            minRemainingVars.push_back(i);

    if(minRemainingVars.size() == 0)
        return -1;

    srand(time(NULL));
    return minRemainingVars[rand() % minRemainingVars.size()];
}

bool cmp(const std::pair<int,int> &a, const std::pair<int,int> &b) {
    return a.second < b.second;
}

std::vector<int> leastConstrainingValue(const int &var, const std::vector< std::vector<int> > &domain, const Graph &graph) {
    std::vector< std::pair<int, int> > domain_constraint(domain[var].size());
    for(int i=0; i<domain[var].size(); i++) {
        domain_constraint[i].first = domain[var][i];
        domain_constraint[i].second = 0;
        for(int j=0; j<graph.adjList[var].size(); j++)
            if(std::find(domain[graph.adjList[var][j]].begin(), domain[graph.adjList[var][j]].end(), domain_constraint[i].first) != domain[graph.adjList[var][j]].end())
                domain_constraint[i].second += 1;
    }

    sort(domain_constraint.begin(), domain_constraint.end(), cmp);

    std::vector<int> ret;
    for(int i=0; i<domain_constraint.size(); i++)
        ret.push_back(domain_constraint[i].first);
    return ret;
}
