#ifndef BASEGRAPH_H_DEFINED
#define BASEGRAPH_H_DEFINED
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <limits>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;
class BaseGraph {
    public:
        //variables and data sturctures
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

        //constructors
        BaseGraph() {}
        virtual ~BaseGraph()
        {
            for (auto ptr: all_edges) {
                delete ptr;
            }
            for (auto ptr: reversed_edges) {
                delete ptr;
            }
        }

        //methods
        void ReadGraph(string file_name)
        {
            fstream file(file_name);
            if (!file.is_open()) throw runtime_error("File could not be opened/does not exist.");
            
            string line;
            string node_label;
            double edge_weight;
            string edge_label;
            vector<Edge*> temp_edges;
            vector<string> splits;
            
            while (getline(file, line)) {
                if (line.front() == '#') continue;
                else if (line.find('#')) line = line.substr(0, line.find('#'));

                node_label = line.substr(0,line.find_first_of(':'));
                line = line.substr(line.find('[')+1, line.find(']'));
                splits = split(line, ',');

                for (auto a: splits) {
                    edge_label = trim(a.substr(0, a.find(':')));
                    try {
                        if (edge_label == "]") break;
                        edge_weight = stod(trim(a.substr(a.find(':')+1, a.size())));
                    }
                    catch (...) {
                        throw(runtime_error("Weight could not be parsed. Please check if all weights are numeric."));
                        exit(1);
                    }
                    if (edge_weight < 0) contains_negative_edges = true;

                    Edge *obj = new Edge{node_label, edge_label, edge_weight};
                    temp_edges.push_back(obj);
                    if (edge_label != "") node_labels.insert(edge_label);
                }
                node_labels.insert(node_label);
                all_nodes.insert({node_label, {node_label, temp_edges}});
                temp_edges.clear();
            }
            //copying all edges
            for (auto &node: all_nodes) {
                for (auto &edge: node.second.edges) {
                    all_edges.push_back(edge);
                }
                all_nodes_as_ptr.insert({node.first, &node.second});
            }

            file.close();
        }

        vector<string> split(const string &s, char delim)
        {
            vector<string> result;
            stringstream ss (s);
            string item;

            while (getline (ss, item, delim)) {
                result.push_back (item);
            }
            return result;
        }

        string trim(const string& str)
        {
            size_t start = str.find_first_not_of(" \t\r\n");
            size_t end = str.find_last_not_of(" \t\r\n");
            if (start == string::npos || end == string::npos) return "";
            return str.substr(start, end - start + 1);
        }

        void Reset()
        {
            //Reset all labels
            for (auto &edge: all_edges) {
                edge->features = "color=black";
            }
            for (auto &node: all_nodes) {
                node.second.visited = false;
                node.second.predecessor = "";
                node.second.distance = numeric_limits<double>::infinity();;
            }
        }

        void AdjacencyMatrix()
        {
            int matrix_size = node_labels.size() + 1;
            string matrix[matrix_size][matrix_size];
            deque<string> d(node_labels.begin(), node_labels.end());

            cout << " |";
            int i = 1;
            for (const auto &a: d) {
                matrix[0][i] = a;
                cout << matrix[0][i] << " ";
                i++;
            }
            cout << endl;

            for (i = 1; i < matrix_size; i++) {
                for (int j = 0; j < matrix_size; j++) {
                    matrix[i][j] = "0";

                    if (j == 0) {
                        matrix[i][j] = d.front();
                        d.pop_front();
                    }
                    else {
                        for (const auto &edge: all_nodes.at(matrix[i][0]).edges) {
                            if (edge->starting_node == matrix[i][0] && edge->ending_node == matrix[0][j]) {
                                matrix[i][j] = "1";
                                break;
                            }  
                        }
                    }
                    cout << matrix[i][j] << " ";   
                }
                cout << endl;
            }
        }

        void BFS()
        {
            string start_node;
            cout << "Enter the starting node (case sensitive): ";
            cin >> start_node;
            if (node_labels.find(start_node) == node_labels.end()) {
                cerr << "Node not found in the graph. Try again" << endl;
                return;
            }

            queue<string> node_queue;
            node_queue.push(start_node);

            cout << "Nodes visited in order using BFS: ";
            while (!node_queue.empty()) {
                string current = node_queue.front();
                node_queue.pop();
                Node *current_node = all_nodes_as_ptr[current];

                if (!current_node->visited) {
                    cout << current;
                    current_node->visited = true;
                    for (const auto &edge: current_node->edges) {
                        if (!all_nodes_as_ptr[edge->ending_node]->visited) node_queue.push(edge->ending_node);
                    }
                    cout << " -> ";
                }     
            }
            cout << "END" << endl;
            Reset();
        }

        void DFS()
        {
            string start_node;
            cout << "Enter the starting node (case sensitive): ";
            cin >> start_node;
            if (node_labels.find(start_node) == node_labels.end()) {
                cerr << "Node not found in the graph. Try again" << endl;
                return;
            }

            stack<string> node_stack;
            node_stack.push(start_node);

            cout << "Nodes visited in order using DFS: ";
            while (!node_stack.empty()) {
                string current = node_stack.top();
                node_stack.pop();
                Node *current_node = all_nodes_as_ptr[current];

                if (!current_node->visited) {
                    cout << current;
                    current_node->visited = true;
                    for (const auto &edge: current_node->edges) {
                        if (!all_nodes_as_ptr[edge->ending_node]->visited) node_stack.push(edge->ending_node);
                    }
                    cout << " -> ";
                }     
            }
            cout << "END" << endl;
            Reset();
        }

        void FloydWarshall()
        {
            int num_vertices = node_labels.size();
            double matrix[num_vertices][num_vertices];
            vector<string> vec(node_labels.begin(), node_labels.end());

            for (int i = 0; i < vec.size(); i++) {
                for (int j = 0; j < vec.size(); j++) {
                    if (i == j) matrix[i][j] = 0;
                    else {
                        for (auto &edge: all_nodes[vec.at(i)].edges) {
                            if (edge->ending_node == vec.at(j)) {
                                matrix[i][j] = edge->weight;
                                break;
                            }
                            else matrix[i][j] = numeric_limits<double>::infinity();
                        }
                    } 
                }
            }

            int i, j, k;
            for (k = 0; k < num_vertices; k++) {
                for (i = 0; i < num_vertices; i++) {
                    for (j = 0; j < num_vertices; j++) {
                        if (matrix[i][j] > (matrix[i][k] + matrix[k][j])
                            && (matrix[k][j] != numeric_limits<double>::infinity()
                                && matrix[i][k] != numeric_limits<double>::infinity()))
                            matrix[i][j] = matrix[i][k] + matrix[k][j];
                    }
                }
            }

            cout << "Shortest distances between every pair of vertices: " << endl;
            cout << "\t";
            for (const auto &v: vec) {
                cout << v << "\t";
            }
            cout << endl;
            
            for (i = 0; i < num_vertices; i++) {
                for (j = 0; j < num_vertices; j++) {
                    if (j == 0) cout << vec[i] << "\t";
                    if (matrix[i][j] == numeric_limits<double>::infinity()) cout << "INF" << "\t";
                    else {
                        stringstream ss;
                        ss << fixed << setprecision(1) << matrix[i][j];
                        cout << ss.str() << "\t";
                        ss.clear();
                    }
                }
                cout << endl;
            }
        }
};

#endif/*BASEGRAPH_H_INCLUDED*/