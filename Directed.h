#ifndef DIRECTED_H_DEFINED
#define DIRECTED_H_DEFINED
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <limits>
#include <iomanip>

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
    };
    vector<Node> all_nodes;
    set<string> node_labels;
    vector<Edge*> all_edges;
    bool contains_negative_edges = false;

    public:
        /** 
         * @param None
         * @returns A copy constructor declaration. The new object will be the same as rhs.
        */
        Directed(const Directed &rhs) = default;

        /** 
         * @param None
         * @returns A copy assignment constructor declaration. The new object will be the same as rhs.
        */
        Directed &operator=(const Directed &rhs) = default;

        /** 
         * @param None
         * @returns A move assignment constructor declaration. It copies the resources of rhs to a new object.
        */
        Directed &operator=(Directed &&rhs) = default;

        /** 
         * @param None
         * @returns A move constructor declaration. It copies the resources of rhs to a new object.
        */
        Directed(Directed &&rhs) = default;

        /** 
         * @param None
         * @returns A default constructor declaration.
        */
        Directed()
        {
            ReadGraph();
            ShowGraph("normal_directed_graph.png");
        }
        /** 
         * @param None
         * @returns A default destructor declaration to delete memory allocated to edges.
        */
        ~Directed() {
            for (auto ptr: all_edges) {
                delete ptr;
            }
        }

        void ReadGraph();
        void ShowGraph(const string& output_name);
        void Dijkstra();
        void MSTPrim();
        void BFS();
        void DFS();
        void AdjacencyMatrix();
        void BellmanFord();
        void reset();
};

#endif/*DIRECTED_H_INCLUDED*/