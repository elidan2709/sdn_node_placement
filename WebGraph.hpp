#ifndef WEB_GRAPH_H_
#define WEB_GRAPH_H_

#include <iostream>
#include <fstream>
#include <ios>
#include <map>
#include <vector>
#include "node.hpp"
#include "DenseMatrix.hpp"

class WebGraph {
	std::fstream f; // файл для парсинга
public:
	std::map <size_t, GML_Node> node_map; // карта вершин (понадобится для построения графа)
	std::vector <weighted_edge> adj_vec; // вектор пар связанных вершин (понадобится для графа)
	
	std::vector <weighted_edge> Kruskal(); // алгоритм Краскала построения минимального остовного дерева
	void K2(); // критерий 2 нахождения дерева
	void parse(); // парсер

	WebGraph() = delete; // обязательно нужно имя файла
	WebGraph(const std::string& filename); // конструктор с параметром
	~WebGraph(); // деструктор

};

#endif