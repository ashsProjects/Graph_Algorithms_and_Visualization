#ifndef UNDIRECTED_H_DEFINED
#define UNDIRECTED_H_DEFINED
#include "BaseGraph.h"

using namespace std;
class Undirected : public BaseGraph {
    public:
        //constructor definitions
        Undirected(const Undirected &rhs) = default;
        Undirected &operator=(const Undirected &rhs) = default;
        Undirected &operator=(Undirected &&rhs) = default;
        Undirected(Undirected &&rhs) = default;
        ~Undirected() = default;
        Undirected();
        
        //method definitions
        void ShowGraph(const string& output_name, vector<Edge*>edges_to_print, const string &title);
        void Dijkstra();
        void MSTPrim();
        void IsBipartite();
        void FindReverseEdge(Edge *edge);
};
#endif/*UNDIRECTED_H_INCLUDED*/