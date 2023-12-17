#ifndef DIRECTED_H_DEFINED
#define DIRECTED_H_DEFINED

using namespace std;
class Directed {
    public:
        /** 
         * @param None
         * @returns A copy constructor declaration. The new object will be the same as rhs.
        */
        Directed(const Directed &rhs) = default;

        /** 
         * @param None
         * @returns A copy assignment constructor declaration. The new object will be the same as rhs.
        */
        Directed &operator=(const Directed &rhs) = default;

        /** 
         * @param None
         * @returns A move assignment constructor declaration. It copies the resources of rhs to a new object.
        */
        Directed &operator=(Directed &&rhs) = default;

        /** 
         * @param None
         * @returns A move constructor declaration. It copies the resources of rhs to a new object.
        */
        Directed(Directed &&rhs) = default;

        /** 
         * @param None
         * @returns A default constructor declaration.
        */
        Directed();

        void ReadGraph();
        void ShowGraph();
        void Dijkstra();
        void MSTPrim();
        void MSTKruskal();
        void MSTReverse();
        void BFS();
        void DFS();
        void AdjacencyMatrix();
        void BellmanFord();
};

#endif/*DIRECTED_H_INCLUDED*/