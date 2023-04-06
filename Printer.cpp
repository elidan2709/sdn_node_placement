#include "Printer.hpp"

Printer::Printer(const char* name, DenseMatrix* matr, std::map <size_t, GML_Node> map, 
    std::vector <weighted_edge> vec, bool upd) : 
        node_map(map), adj_vec(vec), is_upd(upd), matrix(matr) {
            this->topology_name = std::string(name);
}

// открытие фала для записи
// два режима before-updated

void Printer::assign_file(){
    std::string filename(this->topology_name);
    if (is_upd) {
        std::string upd = "routes_" + this->topology_name;
        this->topology_name = upd;
    }
    else {
        this->topology_name += "_topo.csv";
    }
}

Printer::~Printer(){
}

// заполнение матрицы смежности по вектору рёбер

void Printer::fill_matrix() {
    for(auto it = this->adj_vec.begin(); it != this->adj_vec.end(); it++) {
        this->matrix->set(it->first.first, it->first.second, it->second);
        this->matrix->set(it->first.second, it->first.first, it->second);   
    }
}

// update-функция для создания файла 
// с выбранным контроллером для первого критерия

void Printer::update() { this->is_upd = true; }

// update-функция для создания файла 
// с выбранным контроллером для второго критерия

void Printer::update(std::vector <weighted_edge> vec){
    this->is_upd = true;
    this->adj_vec = vec;
    this->matrix->clear();
    this->fill_matrix();
}

// печать матрицы смежности

void Printer::print_matrix() const {
    this->matrix->print();
    return;
}

// отладочная печать вектора пути

void Printer::print_vec() const {
    for(auto it = this->adj_vec.begin(); it != this->adj_vec.end(); it++)
        std::cout << "source: " << it->first.first 
            << " target: " << it->first.second << " delay: "
                 << it->second << " mks"  << std::endl; 
}

// отладочная печать информации о вершинах

void Printer::print_nodes() const {
    for(auto it = this->node_map.begin(); it != this->node_map.end(); it++)
    it->second.print_data();
    return;
}

// печать в CSV-файл топологии до обработки

void Printer::topology_to_csv() const {
    std::ofstream topology;
    topology.open(this->topology_name);
    topology << " ,Node 1 (id),Node 1 (label),Node 1 (longitude),Node 1 (latitude)," 
        "Node 2 (id),Node 2 (label),Node 2 (longitude),Node 2 (latitude)," 
        "Distance (km),Delay (mks)"<< std::endl;
    
    for(size_t i = 0; i < this->matrix->get_size(); i++) 
        for(size_t j = 0; j < this->matrix->get_size(); j++) {
            double delay = matrix->get(i, j);
            double dist = delay / DELAY;
            if(delay > EPS) {
                topology << " ," << i << "," << this->node_map.at(i).city << ","
                    << this->node_map.at(i).longitude << "," << this->node_map.at(i).latitude << ","
                        << j << "," << this->node_map.at(j).city << "," << this->node_map.at(j).longitude 
                            << "," << this->node_map.at(j).latitude << "," << dist << "," << delay << std::endl;
            }
        } 
    topology.close();     
}

// печать топологии в CSV-файл после обработки

void Printer::updated_topology_to_csv() const {
    std::ofstream topology;
    topology.open(this->topology_name);
    topology << " ,   Node 1 (id)   ,   Node 2 (id)   ,   Path   ,  Delay (mks)  "<< std::endl;
    size_t commutator = this->matrix->K1();
    double* dist_mas = this->matrix->Dijkstra(commutator); 
    for(size_t i = 0; i < this->matrix->get_size(); i++) {
        if(dist_mas[i] < EPS || MAX_DELAY - dist_mas[i] < EPS )
            continue;
        std::vector <size_t> path_vec = this->matrix->get_path(commutator, i);
        topology << " ," << commutator << "," << i << "," << "[ ";
            for(auto it = path_vec.begin(); it != path_vec.end(); it++)
                topology << *it << " ";
        topology << "]," << dist_mas[i];
        topology << std::endl;
    } 
    topology.close();
}