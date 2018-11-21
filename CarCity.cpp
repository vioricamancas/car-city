// Copyright Viorica Mancas 311CA
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<iostream>
#include <stack>
#include <utility>

#include "./Graph.h"
#include "./hashtable.h"

/* Hash function pt strings. */
unsigned int hashingFunction(int nr){
    return nr;
}

/* Hash function pt strings. */
unsigned int charHashingFunction(std::string str){
	int n = 0;
	for (int i = 0; i <  str.length(); ++i)
		n += i * str[i];
    return n;
}
// conditie de oprire pt task4
bool cond(int node, Graph<bool>* map) {
	return map->getData(node);
}

void task1(int N, int M, std::ofstream& out, std::vector<int>& aux,
	Hashtable<int, std::string>& hashItoS, Hashtable<std::string,
	int>& hashStoI, Graph<bool>& map) {
	out.open("task1.out");
	// initilizari si citire noduri
	std::string s1, s2;
	int k, kmax = 0, max = 0;
	for (int i = 0; i < N; ++i) {
		std::cin >> s1;
		hashStoI.put(s1, i);
		hashItoS.put(i, s1);
	}
	// adaugare muchii si calcul gradin in vectorul aux
	for (int i = 0; i < M; ++i){
		std::cin >> s1 >> s2;
		k = hashStoI.get(s2);
		++aux[k];
		if (aux[k] > max) {
				kmax = k;
				max = aux[k];
		} else {  // verificare lexicografica
			if (aux[k] == max)
				if (hashItoS.get(kmax) > s2) {
					kmax = k;
					max = aux[k];
				}
		}
		map.addEdge(hashStoI.get(s1), k);
	}
	out << hashItoS.get(kmax) << " " << max;
	out.close();
}

void task2(int N, int& i, std::ofstream& out, std::vector<int>& aux,
	Graph<bool>& map) {
	out.open("task2.out");
	// initializari
	std::fill(aux.begin(), aux.end(), 0);
	map.numConnectedComp = 1;
	map.dfs(0, aux);
	// verificare conditie
	for (i = 1; i < N; ++i)
		if (aux[i] == 0) {
			out << "HARTA INVALIDA" << "\n";
			break;
		}
	if (i == N)
		out << "HARTA VALIDA" << "\n";
	out.close();
}

void task3(int N, int i, std::ofstream& out, std::vector<int>& aux,
	std::vector<int>& transp, Hashtable<int, std::string>& hashItoS,
	Hashtable<std::string, int>& hashStoI, Graph<bool>& map) {
	out.open("task3.out");
	// initializari
	int L, K;
	std::string s1;
	std::cin >> K;
	if (i == N) {
		out << "Exista drum intre oricare doua orase" << "\n";
		for (int i = 0; i < K; i++)
			std::cin >> s1;
	} else {
		for (int i = 0; i < K; i++) {
			std::cin >> s1;
			out << "Orase inaccesibile pentru " << s1 << ": ";
			L = hashStoI.get(s1);
			// reinintializari
			std::fill(aux.begin(), aux.end(), 0);
			std::fill(transp.begin(), transp.end(), 0);
			aux[L] = 1;
			transp[L] = 1;
			// doua parcurgeri (dus - intors)
			map.dfs(L, aux);
			map.dfsTransp(L, transp);
			// afisare
			for (int j = 0; j < N; j++)
				if (aux[j] == 0 || transp[j] == 0)
					out << hashItoS.get(j)  << " ";
			out << "\n";
		}
	}
	out.close();
}

void task4(std::ofstream& out, Hashtable<std::string, int>& hashStoI,
	Graph<bool>& map) {
	out.open("task4.out");
	// initializari
	int K, B, L, k, x, y;
	std::string s1;
	std::cin >> L;
	for (k = 0; k < L; k++) {
		std::cin >> s1 >> x >> y;
		K = hashStoI.get(s1);
		// parcurgere cu oprire
		if (x != 0)
			B = map.bfsMod(&cond, x/y, K);
		else
			B = -2;
		// afisare conditionata
		if (B == -1)
			out << "NU\n";
		else
			if (B == -2)
				out << "DA 0\n";
			else
				out << "DA " << (x - B * y) << "\n";
	}
	out.close();
}

int maxpair(int P, Hashtable<std::string, int>& hashStoI,
	std::vector<std::pair<std::pair<int, int>, int>>& pairs) {
	// initializari
	int k, x, kmax = 0, max = -1, i = 1;
	std::string s1, s2;
	std::pair<int, int> p;
	for (k = 0; k < P; k++) {
		std::cin >> s1 >> s2 >> x;
		p = std::make_pair(hashStoI.get(s1), hashStoI.get(s2));
		pairs[k] = std::make_pair(p, x);
	}
	// sortare pt gasirea duplicatelor
	std::sort(pairs.begin(), pairs.end());
	for (k = 0; k < P; k += i) {
		i = 1;
		while (i + k < P && pairs[k].first == pairs[k+i].first){
			pairs[k].second += pairs[k+i].second;
			i++;
		}
		if (max < pairs[k].second){
			max = pairs[k].second;
			kmax = k;
		}
	}
	return kmax;
}

void task5(std::ofstream& out, Hashtable<int, std::string>& hashItoS,
	Hashtable<std::string, int>& hashStoI, Graph<bool>& map) {
	out.open("task5.out");
	// initializari
	int kmax, P;
	std::pair<int, int> p;
	std::stack<int> path;
	std::vector<std::pair<std::pair<int, int>, int>> pairs;
	std::cin >> P;
	pairs.resize(P);
	// calcul valoare maxima
	kmax = maxpair(P, hashStoI, pairs);
	// calcul drum
	p = pairs[kmax].first;
	path = map.minPath(p.first, p.second);
	// afisare
	out << pairs[kmax].second << " ";
	while (!path.empty()) {
		out << hashItoS.get(path.top()) << " ";
		path.pop();
	}
	out.close();
}

int main() {
	// initializari
	int N, M, k, i, B;
	std::string s1;
	std::ofstream out;

	std::cin >> N >> M;

	std::vector<int> aux(N+1, 0), transp(N+1, 0);
	Hashtable<int, std::string> hashItoS(N+1, hashingFunction);
	Hashtable<std::string, int> hashStoI(N+1, charHashingFunction);
	Graph<bool> map(N, true, false);  // oriented graph

	// rezolvare taskuri
	task1(N, M, out, aux, hashItoS, hashStoI, map);
	task2(N, i, out, aux, map);

	std::cin >> B;
	for (k = 0; k < B; k++) {
		std::cin >> s1;
		map.setData(hashStoI.get(s1), true);
	}
	map.sort();

	task3(N, i, out, aux, transp, hashItoS, hashStoI, map);
	task4(out, hashStoI, map);
	task5(out, hashItoS, hashStoI, map);

	return 0;
}
