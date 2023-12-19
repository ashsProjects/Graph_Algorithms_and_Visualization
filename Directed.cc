#include "Directed.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <set>
#include <queue>
#include <unordered_map>

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
        all_nodes.push_back({node_label, temp_edges});
        temp_edges.clear();
    }
    //copying all edges
    for (auto &node: all_nodes) {
        for (auto &edge: node.edges) {
            all_edges.push_back(edge);
        }
    }

    file.close();
}

void Directed::ShowGraph(const string& output_name)
{
    fstream file("Graphviz_Files/graph_details.txt", ios::out);
    if (!file.is_open()) throw runtime_error("File could not be opened/created.");

    file << "strict digraph d_graph {" << endl;
    for (const auto& edge: all_edges) {
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
        cout << "This graph contains a negative edge. Please use Bellman Ford instead." << endl;
        return;
    }

    string start_node;
    cout << "Enter the starting node (case sensitive): ";
    cin >> start_node;
    if (node_labels.find(start_node) == node_labels.end()) {
        cerr << "Node not found in the graph. Try again" << endl;
        return;
    }

    unordered_map<string, Node*> all_nodes_map;
    for (auto &node: all_nodes) {
        all_nodes_map.insert({node.label, &node});
    }
    all_nodes_map[start_node]->distance = 0;

    priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, CompareNodeDistance> p_queue;
    p_queue.push({0, all_nodes_map[start_node]});

    while (!p_queue.empty()) {
        Node *u_node = p_queue.top().second;
        p_queue.pop();

        if (u_node->visited) continue;
        u_node->visited = true;

        for (const auto &edge: u_node->edges) {
            Node *v_node = all_nodes_map[edge->ending_node];
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
            while (all_nodes_map[pred_label]->predecessor != start_node) {
                a_pred = *all_nodes_map[all_nodes_map[pred_label]->predecessor];
                for (auto &edge: a_pred.edges) {
                    if (edge->ending_node == pred_label) {
                        edges_in_path.push_back(edge);
                    }
                }
                pred_label = a_pred.label;
            }
            a_pred = *all_nodes_map[start_node];
            for (auto &edge: a_pred.edges) {
                if (edge->ending_node == pred_label) {
                    edges_in_path.push_back(edge);
                }
            }
        
            cout << "Node " << all_nodes_map[a]->label << ": ";
            cout << (all_nodes_map[a]->distance == numeric_limits<double>::infinity() ? "INF" : to_string(all_nodes_map[a]->distance)) << " ";
            for (auto &edge: edges_in_path) {
                cout << edge->to_str() << " ";
                edge->features = "color=red";
            }
            cout << endl;
            edges_in_path.clear();
        
        }
    }

    ShowGraph("dijkstras_directed_graph.png");
    reset();

}

void Directed::MSTPrim()
{
    unordered_map<string, Node> all_nodes_map;
    for (auto &node: all_nodes) {
        all_nodes_map.insert({node.label, node});
    }

    priority_queue<pair<double, Edge*>, vector<pair<double, Edge*>>, CompareEdgeWeight> p_queue;
    int min_cost = 0;
    string start_node = all_nodes_map.begin()->first;
    vector<string> edges_to_print;

    cout << "Initializing the starting node as " << start_node << endl;

    all_nodes_map[start_node].visited = true;
    for (auto &edge: all_nodes_map[start_node].edges) {
        p_queue.push({edge->weight, edge});
    }

    while (!p_queue.empty()) {
        double u_weight = p_queue.top().first;
        Edge *u_edge = p_queue.top().second;
        p_queue.pop();

        if (!all_nodes_map[u_edge->ending_node].visited) {
            min_cost += u_weight;
            all_nodes_map[u_edge->ending_node].visited = true;
            u_edge->features = "color=red";
            edges_to_print.push_back(u_edge->to_str());

            for (auto &edge: all_nodes_map[u_edge->ending_node].edges) {
                if (!all_nodes_map[edge->ending_node].visited) p_queue.push({edge->weight, edge});
            }
        }
    }
    cout << "Minimum Cost of MST with Prim's algorithm: " << min_cost << endl;
    cout << "Edges in MST: ";
    for (auto &edge: edges_to_print) {
        cout << edge << ", ";
    }
    cout << endl;
    ShowGraph("prims_directed_graph.png");
    reset();
}

void Directed::BFS()
{
}

void Directed::DFS()
{
}

void Directed::AdjacencyMatrix()
{
}

void Directed::BellmanFord()
{
}

void Directed::reset() {
    //reset all labels
    for (auto &edge: all_edges) {
        edge->features = "color=black";
    }
    for (auto &node: all_nodes) {
        node.visited = false;
        node.predecessor = "";
    }
}
