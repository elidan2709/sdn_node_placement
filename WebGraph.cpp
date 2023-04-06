#include "WebGraph.hpp"

WebGraph::WebGraph(const std::string& filename) {
    this->f.open(filename, std::fstream::in);
    if(this->f.fail())
        throw "gml_parser: error opening file";
}

WebGraph::~WebGraph() {
    this->f.close();
}

// функция сравнения для сортировки вектора рёбер

bool comparator(std::pair <edge, double> a, std::pair <edge, double> b) { 
    return a.second < b.second;
}

// алгоритм Краскала, преобразующий вектор смежности графа в остовное дерево
// он берёт минимальные возможные рёбра графа и проверяет граф на цикличность
// создаётся вектор остовного леса, заносятся данные о вершинах, входящих 
// в данный остовный лес. если концы очередного ребра принадлежат одному дереву,
// то добавление приведёт к появлению цикла. пропускаем ребро
// если очередное ребро относится к разным деревьям, то вносим его и обновляем информацию 

std::vector <weighted_edge> WebGraph::Kruskal() {
    sort(this->adj_vec.begin(), this->adj_vec.end(), comparator); // сортируем вектор весов
    size_t num_edges = this->adj_vec.size();
    size_t num_nodes = this->node_map.size();
    std::vector <weighted_edge> tmp_vec; // делаем новый вектор
    std::vector<size_t> tree_id (num_nodes); // массив номеров деревьев остовного леса 

    for (size_t i = 0; i < num_nodes; i++) // изначально каждая вершина имеет свой номер
        tree_id[i] = i;                    // остовного дерева
    // сам алгоритм 
    for (size_t i = 0; i < num_edges; i++) { 
        size_t from = this->adj_vec[i].first.first,  to = this->adj_vec[i].first.second; 
        double weight = this->adj_vec[i].second;
        if (tree_id[from] != tree_id[to])
        {
            tmp_vec.push_back (std::make_pair(std::make_pair (to, from), weight));
            int old_id = tree_id[to],  new_id = tree_id[from];
            for (size_t j = 0; j < num_nodes; j++)
                if (tree_id[j] == old_id)
                    tree_id[j] = new_id;
        }
    }
    return tmp_vec;
}

// критерий 2 выбора дерева

void WebGraph::K2() {
    this->adj_vec = this->Kruskal();
}

// парсер

void WebGraph::parse(){    
    GML_Node tmp_node; // элементы для вноса в 
    weighted_edge tmp_edge; // карту вершин и вектор рёбер

    // цикл парсера GML файла

    while(1) {
		if(this->f.eof()) 
			break;
		std::string buf;

		this->f >> buf;
        
        if(buf == "node") {
            while(buf != "]") {
                this->f >> buf;
                if(this->f.eof()){ 
			        throw "wrong file structure"; 
                    break;
                }
                if(buf == "id") {
                    this->f >> buf;
                    tmp_node.id = stoi(buf); 
                }
                if(buf == "label") {
                    this->f >> buf;
                    std::string buf2 = buf;
                    while(buf[buf.size() - 1] != '"') {
                        f >> buf;
                        buf2 += ' ';
                        buf2 += buf;
                    }
                    tmp_node.city = buf2;
                }
                if(buf == "Country") {
                    this->f >> buf;
                    std::string buf2 = buf;
                    while(buf[buf.size() - 1] != '"') {
                        f >> buf;
                        buf2 += ' ';
                        buf2 += buf;
                    }
                    tmp_node.country = buf2;
                }
                if(buf == "Longitude") {
                    this->f >> buf;
                    double lng = std::stod(buf);
                    tmp_node.longitude = lng;
                }
                if(buf == "Latitude") {
                    this->f >> buf;
                    double ltd = std::stod(buf);
                    tmp_node.latitude = ltd;
                }
        }
        // вносим в карту вершин очередную вершину
        this->node_map.emplace(std::make_pair(tmp_node.id, tmp_node));
        // и зануляем структуру вершины
        tmp_node.deinit();
        }

        if(buf == "edge") {
            while(buf != "]") {
                this->f >> buf;
                if(this->f.eof()){ 
			    throw "wrong file structure"; 
                break;
                }
                if(buf == "source") {
                    this->f >> buf;
                    tmp_edge.first.first = stoi(buf);
                }
                if(buf == "target") {
                    this->f >> buf;
                    tmp_edge.first.second = stoi(buf);  
                }
            }
        // вносим ребро в вектор
        tmp_edge.second = this->node_map[tmp_edge.first.first].count_delay(this->node_map[tmp_edge.first.second]);
        // если ребро имеет 0 вес (одна из вершин бракованная)
        // то такую вершину надо исключить, так как она будет мешать
        // работе алгоритма Краскала
        // при этом такие вершины вносятся в карту вершин, чтобы не портить 
        // структуру номеров вершин и, соответственно, удобного ориентирования
        // по матрице смежности
        if(tmp_edge.second > EPS) 
            this->adj_vec.push_back(tmp_edge);
        // зануляем ребро    
        tmp_edge.first.first = tmp_edge.first.second = tmp_edge.second = 0; 
        }    
	}
}
