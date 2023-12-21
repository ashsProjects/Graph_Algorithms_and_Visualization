#include "Directed.h"

Directed::Directed()
{
    ReadGraph("Input_File_Templates/Directed_Template.txt");
    ShowGraph("normal_directed_graph.png", all_edges, "Normal Directed Graph");
}

void Directed::ShowGraph(const string& output_name, vector<Edge*>edges_to_print, const string& title)
{
    fstream file("Graphviz_Files/graph_details.txt", ios::out);
    if (!file.is_open()) throw runtime_error("File could not be opened/created.");

    file << "strict digraph d_graph {" << endl;
    for (const auto& edge: edges_to_print) {
        file << edge->starting_node << " -> " << edge->ending_node << " [label=" << edge->weight << ", " << edge->features << "];" << endl;
    }
    file << "labelloc=\"t\"" << endl;
    file << "label=\"" << title << "\"" << endl;
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
        else if (all_nodes_as_ptr[a]->predecessor == "") {
            cout << "Node " << all_nodes_as_ptr[a]->label << ": ";
            cout << (all_nodes_as_ptr[a]->distance == numeric_limits<double>::infinity() ? "INF" : to_string(all_nodes_as_ptr[a]->distance)) << endl;
            continue;
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

    string title = "Directed Dijkstra starting at " + start_node;
    ShowGraph("dijkstras_directed_graph.png", all_edges, title);
    Reset();

}

void Directed::BellmanFord()
{
    string start_node;
    int num_vertices = node_labels.size();
    int num_edges = all_edges.size();

    cout << "Enter the starting node (case sensitive): ";
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

void Directed::ReverseEdges()
{
    for (auto &edge: all_edges) {
        Edge *temp = new Edge{edge->ending_node, edge->starting_node, edge->weight};
        reversed_edges.push_back(temp);
    }

    ShowGraph("reversed_directed_graph.png", reversed_edges, "Directed Reversed Edges");
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