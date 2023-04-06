#ifndef DENSEMATRIX_H_
#define DENSEMATRIX_H_

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector> 
#include "node.hpp"

const double EPS = 1e-15; // точность для сравнения вещественных чисел

class DenseMatrix {
    size_t size; // размер матрицы
    double **matrix;
public:
    DenseMatrix(size_t _size); // конструктор из размера
    DenseMatrix(const DenseMatrix& matrix); // конструктор копирования
    ~DenseMatrix(); // деструктор
    DenseMatrix& operator=(const DenseMatrix& matrix); // оператор присваивания

    double get(size_t i, size_t j) const; // получение элемента по индексу
    size_t get_size() const; // размер
    void set(size_t i, size_t j, double value); // изменение элемента по индексу

    double* operator[](size_t index); // индексация
    double* operator[](size_t index) const; // индексация

    double* Dijkstra(size_t node) const; // алгоритм Дейкстры 
    size_t* get_shortest_paths(size_t node) const; // массив кратчайших путей
    std::vector <size_t> get_path(size_t from, size_t to) const; // восстановление пути по массиву
    size_t K1() const; // критерий 1 

    void clear(); // зануление матрицы
    void print() const; // печать матрицы
};

#endif