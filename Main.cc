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
        cout << "Pick from the options below for algorithms regarding an directed graph:" << endl;
        cout << "\t1. Show the graph visually" << endl;
        cout << "\t2. Single Source Shortest Path with Dijkstra" << endl;
        cout << "\t3. Minimum Spanning Tree using Prim's" << endl;
        cout << "\t4. Minimum Spanning Tree with Kruskal's" << endl;
        cout << "\t5. Minimum Spanning Tree with Reverse Delete" << endl;
        cout << "\t6. Breadth First Search" << endl;
        cout << "\t7. Depth First Search" << endl;
        cout << "\t8. Print Adjacency Matrix" << endl;
        cout << "\t9. Single Destination Shortest Path with Bellman Ford" << endl;
        cout << "Enter q to quit" << endl;

        try {
            string user_input = "";
            cin >> user_input;
            if (user_input == "q") exit(0);
            else choice = stoi(user_input);

            if (choice > 9 || choice < 1) cout << "Entered value is not valid. Try again";
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
                d_graph.Dijkstra();
                break;
            case 3:
                d_graph.MSTPrim();
                break;
            case 4:
                d_graph.MSTKruskal();
                break;
            case 5:
                d_graph.MSTReverse();
                break;
            case 6:
                d_graph.BFS();
                break;
            case 7:
                d_graph.DFS();
                break;
            case 8:
                d_graph.AdjacencyMatrix();
                break;
            case 9:
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
        cout << "Pick from the options below for algorithms regarding an undirected graph:" << endl;
        cout << "\t1. Show the graph visually" << endl;
        cout << "\t2. Single Source Shortest Path with Dijkstra" << endl;
        cout << "\t3. Minimum Spanning Tree using Prim's" << endl;
        cout << "\t4. Minimum Spanning Tree with Kruskal's" << endl;
        cout << "\t5. Minimum Spanning Tree with Reverse Delete" << endl;
        cout << "\t6. Breadth First Search" << endl;
        cout << "\t7. Depth First Search" << endl;
        cout << "\t8. Print Adjacency Matrix" << endl;
        cout << "\t9. Single Destination Shortest Path with Bellman Ford" << endl;
        cout << "Enter q to quit" << endl;

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
                u_graph.MSTKruskal();
                break;
            case 5:
                u_graph.MSTReverse();
                break;
            case 6:
                u_graph.BFS();
                break;
            case 7:
                u_graph.DFS();
                break;
            case 8:
                u_graph.AdjacencyMatrix();
                break;
            case 9:
                u_graph.BellmanFord();
                break;
            default:
                break;
        }
    }
    
}

int main(int argc, char **argv)
{
    #if __cplusplus >= 201103L
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

        #else
            cout << "Sorry, this program is built to run only on Windows!" << endl;
        #endif
    #else
        cout << "Sorry, this program was built to compile on C++ version >= 11!" << endl;
    #endif
    return 0;
}

