#pragma once

using namespace std;


struct Node {
	Node() :
		value(0), weight(0), next(nullptr){}
	Node(int val, int w) :
		value(val), weight(w), next(nullptr){}
	int value; //name of node in graph (0,1,2,3...count-1)
	int weight; //weight of edge (from head to node)
	Node* next; //next node pointer for list
};



class Graph { //graph using singly-chained linked list
public:
	Graph(); //constructor
	Graph(int size); //copy constructor
	~Graph(); //destructor
	bool edgeExists(int u, int v); //does edge exist?
	int getEdgeWeight(int u, int v); //weight value of edge
	void addEdge(int u, int v, int w); //add a weighted edge to the graph
	void removeEdge(int u, int v); //remove a weighted edge from the graph
	bool isUndirected(); //is the graph undirected?
	void Dijkstra(int s, int*& pi); //perform dijikstra's algorithm for shortest path.

private :
	Node** head; // linked list array for vertices
	int* degree; // degree for vertices
	int count; //count for number of vertices
};

void DisplayShortestPath(int* d, int* pi, int n);
