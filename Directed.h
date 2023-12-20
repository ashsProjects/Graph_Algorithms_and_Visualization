#ifndef DIRECTED_H_DEFINED
#define DIRECTED_H_DEFINED
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <limits>
#include <iomanip>
#include <unordered_map>

using namespace std;
class Directed {
    struct Edge {
        string starting_node;
        string ending_node;
        double weight;
        string features = "color=black";
        string to_str() const {
            stringstream ss;
            ss << fixed << setprecision(2) << weight;
            return "(" + starting_node + "," + ending_node + "," + ss.str() + ")";
        }
    };
    struct Node {
        string label;
        vector<Edge*> edges;
        bool visited = false;
        double distance = numeric_limits<double>::infinity();
        string predecessor = "";
    };
    struct CompareEdgeWeight {
        bool operator()(const pair<double, Edge*> &p1, const pair<double, Edge*> &p2) {return p1.first > p2.first;}
    };
    struct CompareNodeDistance {
        bool operator()(const pair<double, Node*> &p1, const pair<double, Node*> &p2) {return p1.first > p2.first;}
        bool operator()(const pair<double, Node> &p1, const pair<double, Node> &p2) {return p1.first > p2.first;}
    };
    unordered_map<string, Node> all_nodes;
    unordered_map<string, Node*> all_nodes_as_ptr;
    set<string> node_labels;
    vector<Edge*> all_edges;
    vector<Edge*> reversed_edges;
    bool contains_negative_edges = false;
    bool contains_cycles = false;

    public:
        //constructor definitions
        Directed(const Directed &rhs) = default;
        Directed &operator=(const Directed &rhs) = default;
        Directed &operator=(Directed &&rhs) = default;
        Directed(Directed &&rhs) = default;
        Directed()
        {
            ReadGraph();
            ShowGraph("normal_directed_graph.png", all_edges);
        }
        ~Directed()//destructor to free all heap memory
        {
            for (auto ptr: all_edges) {
                delete ptr;
            }
            for (auto ptr: reversed_edges) {
                delete ptr;
            }
        }

        //method definitions
        void ReadGraph();
        void ShowGraph(const string& output_name, vector<Edge*>edges_to_print);
        void Dijkstra();
        void MSTPrim();
        void BFS();
        void DFS();
        void AdjacencyMatrix();
        void BellmanFord();
        void ReverseEdges();
        void Reset();
        void DetectCycles(string start_node);
        void IsDAG();
        void SDSP();
};

#endif/*DIRECTED_H_INCLUDED*/