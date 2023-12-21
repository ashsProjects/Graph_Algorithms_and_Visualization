#include <unistd.h>
#include "Undirected.h"
#include "Directed.h"
#include "BaseGraph.h"

using namespace std;
void start_directed() {
    int choice = 0;
    Directed d_graph;

    while (1) {
        string directed_options = "Pick from the options below for algorithms regarding an directed graph:\n";
        directed_options += "\t1. Show the directed graph\n";
        directed_options += "\t2. Single Source Shortest Path using Dijkstra\n";
        directed_options += "\t3. Single Source Shortest Path using Bellman Ford\n";
        directed_options += "\t4. Breadth First Search\n";
        directed_options += "\t5. Depth First Search\n";
        directed_options += "\t6. Print Adjacency Matrix\n";
        directed_options += "\t7. Show the graph with all edges reversed\n";
        directed_options += "\t8. Check if current graph is a DAG\n";
        directed_options += "\t9. Single Destination Shortest Path using Reversed Dijkstra\n";
        directed_options += "\t10. All Pairs Shortest Path using Floyd-Warshall\n"; 
        directed_options += "\tq to quit\n";

        cout << directed_options;
        try {
            string user_input = "";
            cin >> user_input;
            if (user_input == "q") return;
            else choice = stoi(user_input);

            if (choice > 10 || choice < 1) cout << "Entered value is not valid. Try again";
        }
        catch (...) {
            cout << "Something went wrong! Please try again" << endl;
            continue;
        }

        switch (choice) {
            case 1:
            {
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                string command = filesystem::current_path().string() + "\\Graph_PNGs\\normal_directed_graph.png";
                system(command.c_str());
                cout << "Graph was opened in another window. It can also be found under Graph_PNGs\\normal_directed_graph.png" << endl;
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            }
            case 2:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.Dijkstra();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 3:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.BellmanFord();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 4:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.BFS();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 5:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.DFS();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 6:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.AdjacencyMatrix();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 7:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.ReverseEdges();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 8:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.IsDAG();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 9:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.SDSP();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 10:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                d_graph.FloydWarshall();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
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
        undirected_options += "\t1. Show the undirected graph\n";
        undirected_options += "\t2. Single Source Shortest Path with Dijkstra\n";
        undirected_options += "\t3. Minimum Spanning Tree using Prim's\n";
        undirected_options += "\t4. Breadth First Search\n";
        undirected_options += "\t5. Depth First Search\n";
        undirected_options += "\t6. Print Adjacency Matrix\n";
        undirected_options += "\t7. Check if graph is bipartite\n";
        undirected_options += "\t8. All Pairs Shortest Path with Floyd-Warshall\n";
        undirected_options += "Enter q to quit\n";
        cout << undirected_options;

        try {
            string user_input = "";
            cin >> user_input;
            if (user_input == "q") return;
            else choice = stoi(user_input);

            if (choice > 8 || choice < 1) cout << "Entered value is not valid. Try again";
        }
        catch (...) {
            cout << "Something went wrong! Try again" << endl;
            continue;
        }

        switch (choice) {
            case 1:
            {
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                string command = filesystem::current_path().string() + "\\Graph_PNGs\\normal_undirected_graph.png";
                system(command.c_str());
                cout << "Graph was opened in another window. It can also be found under Graph_PNGs\\normal_undirected_graph.png" << endl;
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            }
            case 2:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.Dijkstra();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 3:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.MSTPrim();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 4:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.BFS();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 5:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.DFS();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 6:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.AdjacencyMatrix();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 7:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.IsBipartite();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            case 8:
                printf("\x1B[31m------------------------------BEGIN OUTPUT----------------------------\033[0m\n");
                u_graph.FloydWarshall();
                printf("\x1B[31m-------------------------------END OUTPUT----------------------------\033[0m\n");
                break;
            default:
                break;
        }
    }
    
}

int main(int argc, char **argv)
{
    #if __cplusplus >= 201603L
        #if _WIN32
            int input_option = getopt(argc, argv, "du");

            if (argc <= 1) {
                printf("\x1B[31m--------------------------BEGIN HELP----------------------------\033[0m\n");
                cout << "Use the flag -u to run the program for undirected graphs (./Graph.exe -u)" << endl;
                cout << "Use the flag -d to run the program for directed graphs (./Graph.exe -d)" << endl;
                cout << "Please make sure to fill the template for either first. Both are located under ./Input_File_Templates." << endl;
                cout << "For more information, reference the README.md" << endl;
                printf("\x1B[31m----------------------------END HELP----------------------------\033[0m\n");
            }

            switch (input_option) {
                case 'd':
                    try {
                        start_directed();
                    }
                    catch (...) {
                        cerr << "Something went wrong! Make sure your inputs are correct" << endl;
                    }
                    break;
                case 'u':
                    try {
                        start_undirected();
                    }
                    catch (...) {
                        cerr << "Something went wrong! Make sure your inputs are correct" << endl;
                    }
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

