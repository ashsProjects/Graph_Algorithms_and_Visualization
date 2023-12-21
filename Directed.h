#ifndef DIRECTED_H_DEFINED
#define DIRECTED_H_DEFINED
#include "BaseGraph.h"

using namespace std;
class Directed: public BaseGraph {
    public:
        //constructor definitions
        Directed(const Directed &rhs) = default;
        Directed &operator=(const Directed &rhs) = default;
        Directed &operator=(Directed &&rhs) = default;
        Directed(Directed &&rhs) = default;
        ~Directed() = default;
        Directed();

        //method definitions
        void ShowGraph(const string& output_name, vector<Edge*>edges_to_print);
        void Dijkstra();
        void BellmanFord();
        void ReverseEdges();
        void DetectCycles(string start_node);
        void IsDAG();
        void SDSP();
};

#endif/*DIRECTED_H_INCLUDED*/