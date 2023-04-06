#include "node.hpp"

GML_Node::GML_Node() {
    this->id = 0;
    this->longitude = 0;
    this->latitude = 0;
}

void GML_Node::print_data() const {
    std::cout << "Country: " << this->country 
        << "  City: " << this->city << " id: " << this->id 
            << " Longitude: "  << this->longitude  
                << " Latitude: " << this->latitude << std::endl;
}

void GML_Node::deinit() {
    this->id = 0;
    this->longitude = 0;
    this->latitude = 0;
    this->country = "";
    this->city = "";
}

// угол в радиан

double deg2rad(double ang) { 
  return ang * (PI/180);
}

// подсчёт расстояния по формуле хаверсинуса
// https://en.wikipedia.org/wiki/Haversine_formula

double GML_Node::count_dist(const GML_Node& node) {
    if(((node.latitude < 1e-15)&&(node.longitude < 1e-15)) || 
        ((this->latitude < 1e-15)&&(this->longitude < 1e-15)))
        return 0; // если одна из вершин бракованная то выходим

    // приводим данные к радианам    
    double lat1 = this->latitude, lon1 = this->longitude;
    double lat2 = node.latitude, lon2 = node.longitude;
    double dlat = deg2rad(lat2-lat1);
    double dlon = deg2rad(lon2-lon1);
    // сама формула 
    double a = sin(dlat/2) * sin(dlat/2) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * sin(dlon/2) * sin(dlon/2); 
    double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
    // домножаем на радиус земли
    double d = c * R_EARTH; 
    return d;
}

// подсчёт задержки (расстояние * DELAY)

double GML_Node::count_delay(const GML_Node& node) {
    return (DELAY * this->count_dist(node));
}

GML_Node::~GML_Node(){
}
