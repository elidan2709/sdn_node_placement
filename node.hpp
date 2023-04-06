#ifndef NODE_H_
#define NODE_H_

#include <utility>
#include <map>
#include <iostream>
#include <cmath>

#define R_EARTH 6371; // радиус Земли
#define PI 3.14159265358979323846 // константа пи
#define DELAY 4.8 // задержка в 4,8 мкс
#define MAX_DELAY 2000000 // максимально возможная задержка в рамках задачи
typedef std::pair <size_t, double> node_pair; // пара вершина-максимальная задержка
typedef std::pair <size_t, size_t> edge; // ребро
typedef std::pair <edge, double> weighted_edge; // взвешенное ребро

struct GML_Node {
	std::string country; // страна
	std::string city; // город
	size_t id; // номер
	double longitude, latitude; // координаты

	GML_Node(); // конструктор
	void print_data() const; // отладочная печать
	void deinit(); // деинициализация объекта (для парсера)
	~GML_Node(); // деструктор
	double count_dist(const GML_Node& node); // подсчёт расстояния по координатам
		// по формуле хаверсинуса
	double count_delay(const GML_Node& node); // подсчёт задержки
};

#endif