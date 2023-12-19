#ifndef UNDIRECTED_H_DEFINED
#define UNDIRECTED_H_DEFINED

using namespace std;
class Undirected {
    public:
        /** 
         * @param None
         * @returns A copy constructor declaration. The new object will be the same as rhs.
        */
        Undirected(const Undirected &rhs) = default;

        /** 
         * @param None
         * @returns A copy assignment constructor declaration. The new object will be the same as rhs.
        */
        Undirected &operator=(const Undirected &rhs) = default;

        /** 
         * @param None
         * @returns A move assignment constructor declaration. It copies the resources of rhs to a new object.
        */
        Undirected &operator=(Undirected &&rhs) = default;

        /** 
         * @param None
         * @returns A move constructor declaration. It copies the resources of rhs to a new object.
        */
        Undirected(Undirected &&rhs) = default;

        /** 
         * @param None
         * @returns A default constructor declaration.
        */
        Undirected();

        void ReadGraph();
        void ShowGraph();
        void Dijkstra();
        void MSTPrim();
        void BFS();
        void DFS();
        void AdjacencyMatrix();
        void BellmanFord();
};
#endif/*UNDIRECTED_H_INCLUDED*/