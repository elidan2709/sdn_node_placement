#include <iostream>
#include "DenseMatrix.hpp"
#include "WebGraph.hpp"
#include "Printer.hpp"

int main(int argc, char** argv) {
    // проверка пользовательского ввода и режима обработки
    // программы K1 или K2
    // правильный запуск: ./main -t "topology name" (-k 1/2)
    // если -k нет, то запускается default K2 режим
    try {
        if (argc < 3 || argc > 5 || strcmp(argv[1], "-t") != 0) {
            throw "main: wrong input. expected -t topology name (-k 1/2)";
        }
        else if (argc == 5 && !strcmp(argv[4], "1")) {
            std::cout << "main: first method was chosen" << std::endl;

            WebGraph g(argv[2]);
            g.parse();

            DenseMatrix matrix(g.node_map.size());
            DenseMatrix* pm = &matrix;
            Printer p(argv[2], pm, g.node_map, g.adj_vec);
            p.fill_matrix();
            p.assign_file();
            p.topology_to_csv();
            p.update();
            p.assign_file();

            size_t commutator = p.matrix->K1();
            p.print_matrix();
            std::cout << "main: commutator placed on " << commutator << "th node" << std::endl;

            p.updated_topology_to_csv();
            std::cout << "main: end of execution. check files" << std::endl;

        }
        else if (argc == 5 && !strcmp(argv[4], "2")) {
            std::cout << "main: second method was chosen" << std::endl;
            
            WebGraph g(argv[2]);
            g.parse();

            DenseMatrix matrix(g.node_map.size());
            DenseMatrix* pm = &matrix;
            Printer p(argv[2], pm, g.node_map, g.adj_vec);
            p.fill_matrix();
            p.assign_file();
            p.topology_to_csv();
            g.K2();
            p.update(g.adj_vec);
            p.assign_file();

            size_t commutator = p.matrix->K1();
            p.print_matrix();
            std::cout << "main: commutator placed on " << commutator << "th node" << std::endl;

            p.updated_topology_to_csv();
            std::cout << "main: end of execution. check files" << std::endl;

        }
        else if (argc == 5 && (strcmp(argv[4], "2") != 0 || strcmp(argv[4], "1") != 0)) {
            std::cout << "main: no method with such number" << std::endl;
        }
        else {
            std::cout << "main: no chosen method. executing default k2 + k1" << std::endl;
            
            WebGraph g(argv[2]);
            g.parse();

            DenseMatrix matrix(g.node_map.size());
            DenseMatrix* pm = &matrix;
            Printer p(argv[2], pm, g.node_map, g.adj_vec);
            p.fill_matrix();
            p.assign_file();
            p.topology_to_csv();
            g.K2();
            p.update(g.adj_vec);
            p.assign_file();
            
            size_t commutator = p.matrix->K1();
            std::cout << "main: commutator placed on " << commutator << "th node" << std::endl;
    
            p.updated_topology_to_csv();
            std::cout << "main: end of execution. check files" << std::endl;
        }
    }  
    catch(const char* str) {
        std::cout << str << std::endl;
    }
}
