#include "Undirected.h"

Undirected::Undirected()
{
    ReadGraph("Input_File_Templates/Undirected_Template.txt");
    ShowGraph("normal_undirected_graph.png", all_edges, "Normal Undirected Graph");
}

void Undirected::ShowGraph(const string& output_name, vector<Edge*>edges_to_print, const string &title)
{
    fstream file("Graphviz_Files/graph_details.txt", ios::out);
    if (!file.is_open()) throw runtime_error("File could not be opened/created.");

    file << "strict graph d_graph {" << endl;
    for (const auto& edge: edges_to_print) {
        file << edge->starting_node << " -- " << edge->ending_node << " [label=" << edge->weight << ", " << edge->features << "];" << endl;
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

void Undirected::Dijkstra()
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
                FindReverseEdge(edge);
            }
            cout << endl;
            edges_in_path.clear();
        
        }
    }

    string title = "Undirected Dijkstra Starting at " + start_node;
    ShowGraph("dijkstras_undirected_graph.png", all_edges, title);
    Reset();
}

void Undirected::MSTPrim()
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
            FindReverseEdge(u_edge);
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
    ShowGraph("prims_undirected_graph.png", all_edges, "MST using Prim");
    Reset();
}

void Undirected::FindReverseEdge(Edge *edge)
{
    for (auto &e: all_nodes[edge->ending_node].edges) {
        if (e->ending_node == edge->starting_node) {
            e->features = "color=red";
        }
    }
}

void Undirected::IsBipartite()
{
    int num_vertices = node_labels.size();
    double matrix[num_vertices][num_vertices];
    vector<string> vec(node_labels.begin(), node_labels.end());
    vector<int> b_w_arr(num_vertices, -1);
    queue<int> q;

    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec.size(); j++) {
            if (i == j) matrix[i][j] = 0;
            else {
                for (auto &edge: all_nodes[vec.at(i)].edges) {
                    if (edge->ending_node == vec.at(j)) {
                        matrix[i][j] = 1;
                        break;
                    }
                    else matrix[i][j] = 0;
                }
            } 
        }
    }

    b_w_arr[0] = 1;
    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (matrix[u][u] == 1) {
            cout << "This graph is not bipartite as there is a self-loop" << endl;
            return;
        }
        for (int i = 0; i < num_vertices; i++) {
            if (matrix[u][i] && b_w_arr[i] == -1) {
                b_w_arr[i] = 1 - b_w_arr[u];
                q.push(i);
            }
            else if (matrix[u][i] && b_w_arr[i] == b_w_arr[u]) {
                cout << "This graph is not bipartite" << endl;
                return;
            }  
        }
    }
    cout << "This graph is bipartite" << endl;
}

