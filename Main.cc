#include <iostream>
#include <unistd.h>
#include <filesystem>
#include "Undirected.h"
#include "Directed.h"

using namespace std;
void start_directed() {
    int choice = 0;
    Directed d_graph;

    while (1) {
        string directed_options = "Pick from the options below for algorithms regarding an directed graph:\n";
        directed_options += "\t1. Show the graph visually\n";
        directed_options += "\t2. Single Source Shortest Path with Dijkstra\n";
        directed_options += "\t3. Minimum Spanning Tree using Prim's\n";
        directed_options += "\t4. Breadth First Search\n";
        directed_options += "\t5. Depth First Search\n";
        directed_options += "\t6. Print Adjacency Matrix\n";
        directed_options += "\t7. Single Source Shortest Path with Bellman Ford\n";
        directed_options += "Enter q to quit\n";
        cout << directed_options;

        try {
            string user_input = "";
            cin >> user_input;
            if (user_input == "q") exit(0);
            else choice = stoi(user_input);

            if (choice > 7 || choice < 1) cout << "Entered value is not valid. Try again";
        }
        catch (...) {
            cout << "Something went wrong! Please try again" << endl;
            continue;
        }

        switch (choice) {
            case 1:
            {
                string command = filesystem::current_path().string() + "\\Graph_PNGs\\normal_directed_graph.png";
                system(command.c_str());
                break;
            }
            case 2:
                break;
            case 3:
                d_graph.MSTPrim();
                break;
            case 4:
                d_graph.BFS();
                break;
            case 5:
                d_graph.DFS();
                break;
            case 6:
                d_graph.AdjacencyMatrix();
                break;
            case 7:
                d_graph.BellmanFord();
                break;
            default:
                break;
        }
    }

}

void start_undirected() {
    int choice = 0;
    Undirected u_graph;

    while (1) {
        string undirected_options = "Pick from the options below for algorithms regarding an undirected graph:\n";
        undirected_options += "\t1. Show the graph visually\n";
        undirected_options += "\t2. Single Source Shortest Path with Dijkstra\n";
        undirected_options += "\t3. Minimum Spanning Tree using Prim's\n";
        undirected_options += "\t4. Breadth First Search\n";
        undirected_options += "\t5. Depth First Search\n";
        undirected_options += "\t6. Print Adjacency Matrix\n";
        undirected_options += "\t7. Single Source Shortest Path with Bellman Ford\n";
        undirected_options += "Enter q to quit\n";
        cout << undirected_options;

        try {
            string user_input = "";
            cin >> user_input;
            if (user_input == "q") exit(0);
            else choice = stoi(user_input);

            if (choice > 9 || choice < 1) cout << "Entered value is not valid. Try again";
        }
        catch (...) {
            cout << "Something went wrong! Try again" << endl;
            continue;
        }

        switch (choice) {
            case 1:
                u_graph.ShowGraph();
                break;
            case 2:
                u_graph.Dijkstra();
                break;
            case 3:
                u_graph.MSTPrim();
                break;
            case 4:
                u_graph.BFS();
                break;
            case 5:
                u_graph.DFS();
                break;
            case 6:
                u_graph.AdjacencyMatrix();
                break;
            case 7:
                u_graph.BellmanFord();
                break;
            default:
                break;
        }
    }
    
}

int main(int argc, char **argv)
{
    #if __cplusplus >= 201703L
        #if _WIN32
            int input_option = getopt(argc, argv, "du");

            if (argc <= 1) {
                cout << "-------------------------------------" << endl;
                cout << "Help regarding running this program: " << endl;
                cout << "\t-Use the flag -u to run the program for undirected graphs (./Main.exe -u)" << endl;
                cout << "\t-Use the flag -d to run the program for directed graphs (./Main.exe -d)" << endl;
                cout << "\t*Please make sure to fill the template for either first. Both are located under ./Input_File_Templates." << endl;
            }

            switch (input_option) {
                case 'd':
                    start_directed();
                    break;
                case 'u':
                    start_undirected();
                    break;
                default: 
                    return 1;
            }
            cout << "Exiting..." << endl;

        #else
            cout << "Sorry, this program is built to run only on Windows!" << endl;
        #endif
    #else
        cout << "Sorry, this program was built to compile on C++ version >= 17!" << endl;
    #endif
    return 0;
}

