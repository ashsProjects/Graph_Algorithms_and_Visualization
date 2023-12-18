#include "Directed.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>

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
    vector<Edge> temp_edges;
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
            temp_edges.push_back({node_label, edge_label, edge_weight});
            all_edges.push_back({node_label, edge_label, edge_weight});
            node_labels.insert(edge_label);
        }
        node_labels.insert(node_label);
        all_nodes.push_back({node_label, temp_edges});
    }
    file.close();
}

void Directed::ShowGraph(const string& output_name)
{
    fstream file("Graphviz_Files/graph_details.txt", ios::out);
    if (!file.is_open()) throw runtime_error("File could not be opened/created.");

    file << "strict digraph d_graph {" << endl;
    for (const auto& edge: all_edges) {
        file << edge.starting_node << " -> " << edge.ending_node << " [label=" << edge.weight << ", " << edge.features << "];" << endl;
    }
    file << "}";

    file.close();

    //execute command using system
    string curr_path = filesystem::current_path().string();
    string command = curr_path + "\\Graphviz\\dot.exe -Tpng -o ";//path for executable 
    command += curr_path + "\\Graph_PNGs\\" + output_name + " "; //path for output file
    command += curr_path + "\\Graphviz_Files\\graph_details.txt";//path for input file
    system(command.c_str());//execute command

}

void Directed::Dijkstra()
{
}

void Directed::MSTPrim()
{
}

void Directed::MSTKruskal()
{
}

void Directed::MSTReverse()
{
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