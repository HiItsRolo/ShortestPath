#include "Graph.h"
#include <queue>
#include <functional>
#include<iostream>
#include<vector>
using namespace std;

Graph::Graph() {
count = 0;
head = nullptr;
degree = nullptr;
}

Graph::Graph(int size) {
	count = size;
	head = new Node*[count];
	degree = new int[count];

	for(int i = 0; i < count; i++) {

		head[i] = nullptr;
		degree[i] = 0;

	}
}

Graph::~Graph() {
	for(int i = 0; i < count; i++) {

		Node* current = head[i];
		while(current) {

			Node* next = current->next;
			delete current;
			current = next;
		}

	}
	delete[] head;
	delete[] degree;

}

bool Graph::edgeExists(int u, int v) {
	if(u < count) { // if u is within bounds of graph

		Node* check = head[u];
		while(check != nullptr) { // while index being checked isn't at end of list

			if (check->value == v) { // if node value is v
				return true; // edge exists
			}
			else { // if not
				check = check->next; // check next node
			}

		}
		return false; // check reached end of list, edge does not exist
	}
		return false; // u is outside bounds of graph

}

int Graph::getEdgeWeight(int u, int v) {
	if(u < count) { // if u is within bounds of graph

		Node* check = head[u];
		while(check != nullptr) { // while  check isn't at end of list

			if (check->value == v) { // if value of node is v
				return check->weight; // return its weight
			}
			else { // if not
				check = check->next; // check next node
			}

		}
		return 0; // edge not found

	}
	return 0; //  u is outside bounds of graph

}

void Graph::addEdge(int u, int v, int w) {
	if (u < count && v < count) { //make sure edge is within bounds of graph

		if (!edgeExists(u, v)) { // if the edge exists, do nothing
			Node* tmp = new Node(v, w); // create new node tmp
			tmp->next = head[u]; // add tmp to head of list
			head[u] = tmp;
			degree[u]++; // increase degree for value u
		}

	}
}

void Graph::removeEdge(int u, int v) {
	if (u < count) { // if u is within bounds of graph
		Node* check = head[u];

		if (check != nullptr) { // if u is a node in the graph and there is at least 1 edge

			if (check->value == v) {	//check if value of edge being checked is == v
				head[u] = check->next; // if so, it is the first element in list, so set head[u] to be next element
				delete check;
				degree[u]--; // decrease degree for list of value u
			}
			else {

				if (check->next != nullptr) { // check if another element exists in list
					Node* previous = check; // keep track of previous node
					check = check->next; // check next node
					bool found = false; // edge has not been found
					while ((check != nullptr) && (!found)) { // while edge is not found, and check hasn't reached end of list

						if (check->value == v) { // check current index in list for value
							previous->next = check->next; // previous index in linked list's next is now the next value in check
							delete check; // edge can now be deleted
							found = true; // edge has been found
							degree[u]--; // decrease degree for the list of value u
						}
						else { // if not found
							previous = check; // set previous to current index checking
							check = check->next; // now check next index
						}

					}

				}

			}


		}

	}

}

bool Graph::isUndirected() {
	int v, u = 0; // will check all node values to see if undirected
	while(u < count) { // all u node values

		v = u; // avoid checking values that have already been checked
		while(v < count) {

			if (getEdgeWeight(u, v) != getEdgeWeight(v, u)) { // if edge weights aren't reversible, it is not undirected
				return false;
			}
			v++; // increase v value to check next element in list

		}
		u++; // increase u value to check next list

	}

	int i = 0;
	while (i < count) {

		if (getEdgeWeight(i, i) != 0) { // check for selfloop
			return false;
		}
		i++;

	}
	return true;

}



void Graph::Dijkstra(int s, int*& pi) {

	int* d = new int[count]; //cumulative cost of path
	pi = new int[count]; //previous node of value in graph

	Node** search = new Node*[count]; //iterator for search algorithm

	for (int i = 0; i < count; i++) {
		search[i] = nullptr;
	}

	priority_queue < pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> horizon; //horizon of search. First is d, second is node value, min heap based on d value

	for(int u = 0; u < count; u++) {
		d[u] = INT_MAX; //d for all values in graph should be INT_MAX initially
		pi[u] = -1; // previous node unknown for all nodes in graph
	}
	d[s] = 0; // start node s has distance traveled of 0
	horizon.push(make_pair(d[s], s)); // push onto horizon
    int iter = 0;
	while(iter < count) { //dijikstra will have a move for every node in graph

		pair<int,int>next = horizon.top(); //top of horizon has next move
		horizon.pop(); //pop top of horizon since move is being executed
		int u = next.second; //u = to node value we are searching
	//	cout << "Using : " << next.second << " with d : " << next.first << endl;
		search[u] = head[u]; //start search at head[u]

		while(search[u] != nullptr) { //while search[u] hasnt reach the end of the list
			int v = search[u]->value; //v = the value of adjacent node from u
			search[u] = search[u]->next; //set search to next value in list

			int w = getEdgeWeight(u, v); //find weight of edge
			if(d[v] > d[u] + w) { //relaxation

				d[v] = d[u] + w; //if edge yields a lower node distance, set d[v] to lower node distance
				pi[v] = u; //previous node of [v] is now u
				horizon.push(make_pair(d[v], v)); //push node v onto horizon for consideration for next search

			}

		}
		iter++;

	}
	horizon = priority_queue < pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> (); // clear horizon
	delete d;
}
