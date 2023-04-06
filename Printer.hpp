#ifndef PRINTER_H_
#define PRINTER_H_

#include <fstream>
#include <iostream>
#include "WebGraph.hpp"
#include "DenseMatrix.hpp"

class Printer {

	std::string topology_name; // имя топологии 
	std::map <size_t, GML_Node> node_map; // карта вершин
	std::vector <weighted_edge> adj_vec; // вектор рёбер
	bool is_upd; // до или после обработки программы 

public: 
	DenseMatrix* matrix; // матрица смежности
	Printer(const char* name, DenseMatrix* matr, std::map <size_t, GML_Node> map, 
		std::vector <weighted_edge> vec, bool upd = false); // конструктор с параметрами
	void assign_file(); // создание csv файла
	~Printer(); //ltcnhernjh

	void fill_matrix(); // заполнение матрицы смежности
	void update(std::vector <weighted_edge> vec); // обновление после обработки
	void update();  // обновление для критерия 1

	void print_matrix() const; // печать матрицы
	void print_vec() const; // печать вектора рёбер
	void print_nodes() const; // печать информации о вершинах

	void topology_to_csv() const; // вывод в csv файл до обработки 
	void updated_topology_to_csv() const; // вывод в csv файл после обработки
};

#endif