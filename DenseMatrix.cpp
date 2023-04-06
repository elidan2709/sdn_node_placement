#include "DenseMatrix.hpp"

DenseMatrix::DenseMatrix(size_t _size) {
    if (_size == 0)
        throw "SparseMatrix: invalid size";

    this->size = _size;
    this->matrix = new double* [size];
    for (size_t i = 0; i < size; i++) {
        matrix[i] = new double [size];
        for(size_t j = 0; j < size; j++)
            matrix[i][j] = 0;
    }
}
DenseMatrix::DenseMatrix(const DenseMatrix& matrix) {
    this->size = matrix.size;
    this->matrix = new double *[size]; 
    for (size_t i = 0; i < size; i++) {
       this->matrix[i] = new double [size];
        for(size_t j = 0; j < size; j++)
            this->matrix[i][j] = matrix[i][j];
    } 
}

DenseMatrix::~DenseMatrix() {
    for(size_t i = 0; i < size; i++)
        delete[] this->matrix[i]; 
    delete[] matrix;
}

DenseMatrix& DenseMatrix::operator=(const DenseMatrix& _matrix) {
    if (this == &_matrix)
        return *this;

    if (this->matrix != nullptr) {
        for(size_t i = 0; i < size; i++)
            delete[] this->matrix[i]; 
        delete[] this->matrix;
    }
    this->size = _matrix.size;
    this->matrix = new double *[size]; 
    for (size_t i = 0; i < size; i++) {
       this->matrix[i] = new double [size];
        for(size_t j = 0; j < size; j++)
            this->matrix[i][j] = _matrix[i][j];
    }   
    return *this;
}

double DenseMatrix::get(size_t i, size_t j) const {
    if ((i > this->size) || (j > this->size)) 
        throw "DenseMatrix::operator[] segmentation fault";
    return matrix[i][j];
}

void DenseMatrix::set(size_t i, size_t j, double value) {
    if ((i > this->size) || (j > this->size)) 
        throw "DenseMatrix::operator[] segmentation fault";
    matrix[i][j] = value;
    return;  
}

size_t DenseMatrix::get_size() const {
    return this->size;
}

double* DenseMatrix::operator[](size_t index) {
    if (index >= size)
        throw "DenseMatrix::operator[] segmentation fault";
    return this->matrix[index];
}

double* DenseMatrix::operator[](size_t index) const {
    if (index >= size)
        throw "DenseMatrix::operator[] segmentation fault";
    return this->matrix[index];
}

// алгоритм Дейкстры для поиска кратчайших расстояний от вершины до всех остальных
// думаю он двольно понятен 

double* DenseMatrix::Dijkstra(size_t node) const {
    if (node > this->size)
        throw "Dijkstra: out_of_range";

    double* distance;
    size_t* parent; 
    bool* visited;
    size_t count, index, i, u;

    distance = new double[this->size];
    visited = new bool[this->size];
    parent = new size_t[this->size];

    for (i = 0; i < this->size; i++) { 
        distance[i]=MAX_DELAY; 
        visited[i]=false;
        parent[i] = this->get_size() + 1; 
    }
    distance[node] = 0;
    for (count = 0; count < this->size - 1; count++) {
        int min = MAX_DELAY;
        for (i = 0; i < this->size; i++)
            if (!visited[i] && distance[i] <= min) { 
                min = distance[i]; 
                index = i; 
            }
    u = index;
    visited[u] = true;
    for (i = 0; i < this->size; i++)
        if (!visited[i] && this->matrix[u][i] > EPS && 
            (MAX_DELAY - distance[u]) > EPS && 
                distance[u] + this->matrix[u][i] < distance[i]) {
                    parent[i] = u;
                    distance[i] = distance[u] + this->matrix[u][i];
                }
    }

    delete[] visited;
    delete[] parent; 
    return distance;
}

size_t DenseMatrix::K1() const {

    std::vector <node_pair> max_nodes_vec; // вектор максимальных расстояний от заданной вершины
    node_pair tmp_pair; // создаём временную пару для вноса в вектор
    double* mas = new double[this->size]; // массив результата работы алгоритма Дейкстры
    for(size_t i = 0; i < this->size; i++) { // перебираем все вершины
        mas = Dijkstra(i);                  // для каждой вершины запускаем алгоритм  
        double max = 0;
        for(size_t j = 0; j < this->size; j++) { // находим максимальное расстояние
            if((mas[j] < MAX_DELAY)&&(mas[j] > max)) 
            max = mas[j];
        }
    if (max < EPS)             // если вершина ни с кем не связана, обозначаем её 
        max = MAX_DELAY;
    tmp_pair.first = i;     
    tmp_pair.second = max;
    max_nodes_vec.push_back(tmp_pair); // заносим вершину в вектор 
    tmp_pair.first = 0; // зануляем пару для дальнейшей работы
    tmp_pair.second = 0;
    }

    size_t num = 0;
    size_t min_max = MAX_DELAY;
    for(size_t j = 0; j < max_nodes_vec.size(); j++) { // находим минимальный макимум
        if(max_nodes_vec[j].second < min_max) {
            min_max = max_nodes_vec[j].second;
            num = max_nodes_vec[j].first;
        }
    }
    return num; // и возвращаем
}

// метод, возвращающий массив путей (из какой вершины был получен кратчайший путь)
// в общем-то код почти полностью копирет алгоритм Дейкстры, тк я не смог придумать
// как их объединить в один метод, чтобы он мог возвращать разные массивы

size_t* DenseMatrix::get_shortest_paths(size_t node) const {
    if (node > this->size)
        throw "Dijkstra: out_of_range";

    double* distance;
    size_t* parent; 
    bool* visited;
    size_t count, index, i, u;

    distance = new double[this->size];
    visited = new bool[this->size];
    parent = new size_t[this->size];

    for (i = 0; i < this->size; i++) { 
        distance[i]=MAX_DELAY; 
        visited[i]=false;
        parent[i] = this->get_size() + 1; 
    }
    distance[node] = 0;
    for (count = 0; count < this->size - 1; count++) {
        int min = MAX_DELAY;
        for (i = 0; i < this->size; i++)
            if (!visited[i] && distance[i] <= min) { 
                min = distance[i]; 
                index = i; 
            }
    u = index;
    visited[u] = true;
    for (i = 0; i < this->size; i++)
        if (!visited[i] && this->matrix[u][i] > EPS && 
            (MAX_DELAY - distance[u]) > EPS && 
                distance[u] + this->matrix[u][i] < distance[i]) {
                    parent[i] = u;
                    distance[i] = distance[u] + this->matrix[u][i];
                }
    }

    delete[] visited;
    delete[] distance; 
    return parent;
}

// рекурсивная функция восстановления пути по массиву

void get_path_vec(size_t* parent, size_t j, size_t comparator, std::vector <size_t>& vec) {
    if (parent[j] == comparator)
        return;
  
    get_path_vec(parent, parent[j], comparator, vec);

    vec.push_back(j);
}

// получение вектора пути от вершины до вершины

std::vector <size_t> DenseMatrix::get_path(size_t from, size_t to) const{
    size_t* mas = this->get_shortest_paths(from);
    std::vector<size_t> vec;
    vec.push_back(from);
    get_path_vec(mas, to, this->get_size() + 1, vec);
    return vec;
}

void DenseMatrix::clear() {
    for (size_t i = 0; i < this->size; i++) {
        for (size_t j = 0; j < this->size; j++)
            this->matrix[i][j] = 0;    
    }
}

void DenseMatrix::print() const {
    for (size_t i = 0; i < this->size; i++) {
        for (size_t j = 0; j < this->size; j++)
            std::cout << std::setw(6) << get(i, j) << " ";
        
        std::cout << std::endl;
    }
    std::cout << std::endl;
}