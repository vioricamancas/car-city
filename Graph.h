// Copyright
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <utility>


template <typename Tdata> class Graph {
 private:
	std::vector<std::vector<int> > A;
	// matricea de adiacenta
	std::vector<std::vector<int> > T;
	// matricea de adiacenta a grafului transpus
	std::vector<Tdata> data;
	// vector de valori ale elementelor
	int numNodes;
	bool oriented;

 public:
	int numConnectedComp;
/*
	Constructor cu initializare
*/
	Graph(int n, bool oriented, Tdata val) {
		this->numNodes = n;
		this->oriented = oriented;
		data.resize(n, val);
		A.resize(n);
		T.resize(n);
	}

/* 
	Adauga muchie in graf, daca este neorientat o adauga si invers,
	iar pentru graf orientat o adauga si in lista de adiacenta
	a grafului transpus 
*/
	void addEdge(int i, int j) {
		A[i].push_back(j);
		if (!oriented) {
			A[j].push_back(i);
		} else {
			T[j].push_back(i);
		}
	}
/* 
	Adauga muchie definita prin valoarea nodurilor
*/
	void addEdge(Tdata val1, Tdata val2) {
		int i = getNode(val1),
		j = getNode(val2);
		A[i].push_back(j);
		if (!oriented) {
			A[j].push_back(i);
		} else {
			T[j].push_back(i);
		}
	}
/*
	Verifica daca exista muchie intre doua noduri
*/
	bool isEdge(int i, int j) {
		std::vector<int> v = A[i];
		return std::find(v.begin(), v.end(), j) != v.end();
	}

/*
	Realizeaza parcurgerea in adancime in mod recursiv
*/
	void dfs(int node, std::vector<int>& visited) {
		for(int i = 0; i < A[node].size(); i++) {
			if(!visited[A[node][i]]) {
				visited[A[node][i]] = numConnectedComp;
				Graph::dfs(A[node][i], visited);
			}
		}
	}
/*
	Realizeaza parcurgerea in adancime pe graful transpus
	in mod recursiv
*/
	void dfsTransp(int node, std::vector<int>& visited) {
		for(int i = 0; i < T[node].size(); i++) {
			if(!visited[T[node][i]]) {
				visited[T[node][i]] = numConnectedComp;
				Graph::dfsTransp(T[node][i], visited);
			}
		}
	}
/*
	Adauga o valoare unui element
*/
	void setData(int node, Tdata Data) {
		data[node] =  Data;
	}
/*
	Returneaza valoarea unui element
*/
	Tdata getData(int node) {
		return data[node];
	}
/*
	Returneaza indexul asociat unei valori
*/
	int getNode(Tdata Data) {
		return std::find(data.begin(), data.end(), Data) - data.begin();
	}
/*
	Sorteaza lista de adiacenta astfel incat vecinii sa
	fie in ordine crescatoare
*/
	void sort(){
		for (int i = 0; i < numNodes; i++)
			std::sort(A[i].begin(), A[i].end());
	}
/*
	Realizeaza o parcurgere in latime pentru a gasi cel mai apropiat
	nod care indeplineste conditia, in limita unei distante maxime
*/
	int bfsMod(bool (*cond)(int nod, Graph<Tdata>* map), int dmax,
		 int source) {
		// initializari
		int k = 0, min = dmax + 1, dist[numNodes];
		std::queue<int> q;
		bool *visited = new bool[numNodes];
		for (int i = 0; i < numNodes; i++) {
			dist[i] = 0;
			visited[i] = false;
		}

		visited[source] = true;
		q.push(source);
		// parcurgerea cozii
		while (!q.empty()) {
			int node = q.front();
			q.pop();
			k = dist[node] + 1;
			if (k > dmax)
				break;
			for(int i = 0; i < A[node].size(); i++){
				if(!visited[A[node][i]]){
					// verificare conditie
					if (cond(A[node][i], this) && min > k){
						min = k;
					}
					// vizitare
					dist[A[node][i]] = k;
					visited[A[node][i]] = true;
					q.push(A[node][i]);
				}
			}
		}
		delete[] visited;
		// verificare staisfacere conditie
		if (min != dmax + 1)
			return min;
		return -1;
	}
/*
	Calculeaza dumul de lungime minima de la sursa la destinatie
*/
	std::stack<int> minPath(int source, int dest) {
		// initializari
		int k = 0;
		std::vector<bool> visited;
		std::queue<int> q;
		std::vector<int> dist;
		std::vector<int> parent;

		parent.resize(numNodes + 1, -1);
		visited.resize(numNodes, false);
		dist.resize(numNodes, -1);
		dist[source] = 0;

		// parcurgere coada
		q.push(source);
		while (!q.empty()) {
			int node = q.front();
			q.pop();
			visited[node] = true;
			k++;
			for (int i = 0; i < A[node].size(); i++) {
				if (!visited[A[node][i]]) {
					// vizitare
					visited[A[node][i]] = true;
					dist[A[node][i]] = k;
					parent[A[node][i]] = node;
					q.push(A[node][i]);
				}
			}
		}
		std::stack<int> path;
		// Nu exista drum de la sursa la destinarie
		if (parent[dest] == -1) {
			return path;
		}
		// Consturirea stivei (din nodurile parcurse, in ordine)
		int node = dest;
		while (node != source) {
			path.push(node);
			node = parent[node];
		}
		path.push(node);
		return path;
	}
};

#endif  // GRAPH_H_
