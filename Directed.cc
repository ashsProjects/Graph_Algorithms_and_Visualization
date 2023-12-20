#include "Directed.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <set>
#include <queue>
#include <unordered_map>
#include <limits>
#include <stack>

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

void Directed::ReadGraph()
{
    fstream file("Input_File_Templates/Directed_Template.txt");
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
                edge_weight = stod(trim(a.substr(a.find(':')+1, a.size())));
            }
            catch (...) {
                throw(runtime_error("Weight could not be parsed. Please check if all weights are numeric."));
                exit(1);
            }
            if (edge_weight < 0) contains_negative_edges = true;

            Edge *obj = new Edge{node_label, edge_label, edge_weight};
            temp_edges.push_back(obj);
            node_labels.insert(edge_label);
        }
        node_labels.insert(node_label);
        all_nodes.insert({node_label, {node_label, temp_edges}});
        temp_edges.clear();
    }
    //copying all edges
    for (auto &node: all_nodes) {
        for (auto &edge: node.second.edges) {
            all_edges.push_back(edge);
            all_nodes_as_ptr.insert({node.first, &node.second});
        }
    }

    file.close();
}

void Directed::ShowGraph(const string& output_name, vector<Edge*>edges_to_print)
{
    fstream file("Graphviz_Files/graph_details.txt", ios::out);
    if (!file.is_open()) throw runtime_error("File could not be opened/created.");

    file << "strict digraph d_graph {" << endl;
    for (const auto& edge: edges_to_print) {
        file << edge->starting_node << " -> " << edge->ending_node << " [label=" << edge->weight << ", " << edge->features << "];" << endl;
    }
    file << "}";

    file.close();

    //execute command using system
    string curr_path = filesystem::current_path().string();
    string command = curr_path + "\\Graphviz\\dot.exe -Tpng -o ";//path for executable 
    command += curr_path + "\\Graph_PNGs\\" + output_name + " "; //path for output file
    command += curr_path + "\\Graphviz_Files\\graph_details.txt";//path for input file
    system(command.c_str());//execute command
    cout << "Finished creating graph. It can be found under ./Graph_PNGs/" << output_name << endl;

}

void Directed::Dijkstra()
{
    if (contains_negative_edges) {
        cerr << "This graph contains a negative edge. Dijkstra does not work with negative weights. Use the Bellman Ford implementation instead." << endl;
        return;
    }
    
    string start_node;
    cout << "Enter the starting node (case sensitive): " << endl;
    cin >> start_node;

    if (node_labels.find(start_node) == node_labels.end()) {
        cerr << "Node not found in the graph. Try again" << endl;
        return;
    }

    all_nodes_as_ptr[start_node]->distance = 0;

    priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, CompareNodeDistance> p_queue;
    p_queue.push({0, all_nodes_as_ptr[start_node]});

    while (!p_queue.empty()) {
        Node *u_node = p_queue.top().second;
        p_queue.pop();

        if (u_node->visited) continue;
        u_node->visited = true;

        for (const auto &edge: u_node->edges) {
            Node *v_node = all_nodes_as_ptr[edge->ending_node];
            if (!v_node->visited && (u_node->distance + edge->weight < v_node->distance)) {
                v_node->distance = u_node->distance + edge->weight;
                v_node->predecessor = u_node->label;
                p_queue.push({v_node->distance, v_node});
            }
        }
    }
    
    //print all nodes
    vector<Edge*> edges_in_path;

    cout << "Shortest distances from node " << start_node << " to all other nodes: " << endl;
    for (auto &a: node_labels) {
        if (a == start_node) {
            cout << "Node " << start_node << ": 0.00" << endl;
        }
        else {
            string pred_label = a;
            Node a_pred;
            while (all_nodes_as_ptr[pred_label]->predecessor != start_node) {
                a_pred = *all_nodes_as_ptr[all_nodes_as_ptr[pred_label]->predecessor];
                for (auto &edge: a_pred.edges) {
                    if (edge->ending_node == pred_label) {
                        edges_in_path.push_back(edge);
                    }
                }
                pred_label = a_pred.label;
            }
            a_pred = *all_nodes_as_ptr[start_node];
            for (auto &edge: a_pred.edges) {
                if (edge->ending_node == pred_label) {
                    edges_in_path.push_back(edge);
                }
            }
        
            cout << "Node " << all_nodes_as_ptr[a]->label << ": ";
            cout << (all_nodes_as_ptr[a]->distance == numeric_limits<double>::infinity() ? "INF" : to_string(all_nodes_as_ptr[a]->distance)) << " ";
            for (auto &edge: edges_in_path) {
                cout << edge->to_str() << " ";
                edge->features = "color=red";
            }
            cout << endl;
            edges_in_path.clear();
        
        }
    }

    ShowGraph("dijkstras_directed_graph.png", all_edges);
    Reset();

}

void Directed::MSTPrim()
{
    priority_queue<pair<double, Edge*>, vector<pair<double, Edge*>>, CompareEdgeWeight> p_queue;
    int min_cost = 0;
    string start_node = all_nodes.begin()->first;
    vector<string> edges_to_print;

    cout << "Initializing the starting node as " << start_node << endl;

    all_nodes[start_node].visited = true;
    for (auto &edge: all_nodes[start_node].edges) {
        p_queue.push({edge->weight, edge});
    }

    while (!p_queue.empty()) {
        double u_weight = p_queue.top().first;
        Edge *u_edge = p_queue.top().second;
        p_queue.pop();

        if (!all_nodes[u_edge->ending_node].visited) {
            min_cost += u_weight;
            all_nodes[u_edge->ending_node].visited = true;
            u_edge->features = "color=red";
            edges_to_print.push_back(u_edge->to_str());

            for (auto &edge: all_nodes[u_edge->ending_node].edges) {
                if (!all_nodes[edge->ending_node].visited) p_queue.push({edge->weight, edge});
            }
        }
    }
    cout << "Minimum Cost of MST with Prim's algorithm: " << min_cost << endl;
    cout << "Edges in MST: ";
    for (auto &edge: edges_to_print) {
        cout << edge << ", ";
    }
    cout << endl;
    ShowGraph("prims_directed_graph.png", all_edges);
    Reset();
}

void Directed::BFS()
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

void Directed::DFS()
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

void Directed::AdjacencyMatrix()
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
                    else matrix[i][j] = "0";  
                }
            }
            cout << matrix[i][j] << " ";   
        }
        cout << endl;
    }
}

void Directed::BellmanFord()
{
    string start_node;
    int num_vertices = node_labels.size();
    int num_edges = all_edges.size();

    cout << "Enter the destination node (case sensitive): ";
    cin >> start_node;
    if (node_labels.find(start_node) == node_labels.end()) {
        cerr << "Node not found in the graph. Try again" << endl;
        return;
    }

    unordered_map<string, double> distances;
    unordered_map<string, string> predecessors;

    for (const string& node: node_labels) {
        distances[node] = numeric_limits<double>::infinity();
        predecessors[node] = "";
    }

    distances[start_node] = 0;

    for (auto i = 0; i < node_labels.size()-1; i++) {
        for (const auto &edge: all_edges) {
            string u = edge->starting_node;
            string v = edge->ending_node;
            int weight = edge->weight;

            if (distances[u] != numeric_limits<double>::infinity() && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                predecessors[v] = u;
            }
        }
    }

    for (const auto& edge: all_edges) {
        string u = edge->starting_node;
        string v = edge->ending_node;
        int weight = edge->weight;

        if (distances[u] != numeric_limits<double>::infinity() && distances[u] + weight < distances[v]) {
            cout << "Graph contains a negative-weight cycle" << endl;
        }
    }

    cout << "Shortest distance from " << start_node << " to every other node: " << endl;
    for (const auto& node: distances) {
        cout << "\tDistance to node " << node.first << ": " << node.second << endl; 
    }

}

void Directed::Reset()
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

void Directed::ReverseEdges()
{
    for (auto &edge: all_edges) {
        Edge *temp = new Edge{edge->ending_node, edge->starting_node, edge->weight};
        reversed_edges.push_back(temp);
    }

    ShowGraph("reversed_directed_graph.png", reversed_edges);
}

void Directed::DetectCycles(string start_node)
{
    stack<string> node_stack;
    node_stack.push(start_node);

    while (!node_stack.empty()) {
        string current = node_stack.top();
        node_stack.pop();
        Node *current_node = all_nodes_as_ptr[current];

        if (!current_node->visited) {
            current_node->visited = true;
            for (const auto &edge: current_node->edges) {
                if (!all_nodes_as_ptr[edge->ending_node]->visited) node_stack.push(edge->ending_node);
                else {
                    contains_cycles = true;
                    return;
                }
            }
        }     
    }
}

void Directed::IsDAG()
{
    for (auto &a: node_labels) {
        if (contains_cycles) break;
       DetectCycles(a);
    }

    if (contains_cycles) cout << "This graph is not a Directed Acyclic Graph as it contains at least one cycle" << endl;
    else cout << "This graph is a Directed Acyclic Graph as it does contains any cycles" << endl;
}

void Directed::SDSP()
{
    ReverseEdges();
    unordered_map<string, Node> reversed_nodes;
    for (auto &node: node_labels) {
        vector<Edge*> temp_edges;
        for (auto &edge: reversed_edges) {
            if (edge->starting_node == node) {
                temp_edges.push_back(edge);
            }
        }
        reversed_nodes.insert({node, {node, temp_edges}});
    }

    string dest_node;
    cout << "Enter the starting node (case sensitive): ";
    cin >> dest_node;
    if (node_labels.find(dest_node) == node_labels.end()) {
        cerr << "Node not found in the graph. Try again" << endl;
        return;
    }

    unordered_map<string, double> distances;
    for (const string& node: node_labels) {
        distances[node] = numeric_limits<double>::infinity();
    }
    distances[dest_node] = 0;

    priority_queue<pair<double, Node>, vector<pair<double, Node>>, CompareNodeDistance> p_queue;
    p_queue.push({0, reversed_nodes[dest_node]});

    while (!p_queue.empty()) {
        Node u = p_queue.top().second;
        double u_dist = p_queue.top().first;
        p_queue.pop();

        if (u_dist > distances[u.label]) continue;
        for (const auto & edge: reversed_nodes[u.label].edges) {
            Node v = reversed_nodes[edge->ending_node];
            double weight = edge->weight;

            if (distances[u.label] + weight < distances[v.label]) {
                distances[v.label] = distances[u.label] + weight;
                v.predecessor = u.label;
                p_queue.push({distances[v.label], v});
            }
        }
    }

    cout << "Distance from every other node to destination node " << dest_node << ": " << endl;
    for (auto &a: distances) {
        cout << "From " << a.first << ": " << (a.second == numeric_limits<double>::infinity() ? "INF" : to_string(a.second)) << endl;
    }
}