#ifndef TESTER_HH
#define TESTER_HH


#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "renderer.hh"
#include "parser.hh" 

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

class Tester {
private:
    std::string directory = "";
public:
    Tester(std::string directory);
    void run_render();

    void clear_render();

    void test_solver();
};



#endif //TESTER_HH
