#include "tester.hh"

Tester::Tester(std::string directory) {
    this->directory = directory;
}

void Tester::run_render() {
    std::string path = this->directory;

    if (!std::filesystem::exists(path)) {
        std::cerr << "Directory does not exist" << std::endl;
        return;
    }

    std::cout << "Running parsing/render test" << std::endl;

    for (const auto& dirEntry : recursive_directory_iterator(path)){
        if (dirEntry.path().extension() == ".ehoa" || dirEntry.path().extension() == ".hoa") {
            std::string inputPath = dirEntry.path().string();

            std::ifstream inputFile(inputPath);

            if (!inputFile.is_open()) {
                std::cerr << "Could not open input file: " << inputPath << std::endl;
                continue;
            }

            std::cout << "Parsing " << inputPath << std::endl;

            try {
                Parser::parse(inputPath);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing input file: " << inputPath << std::endl;
                std::cerr << e.what() << std::endl;
                continue;
            }

            auto automaton = Parser::parse(inputPath);

            std::cout << "Rendering " << inputPath << std::endl;
            Renderer::render(automaton, inputPath);

            std::cout << std::endl;
        }
    }
}

void Tester::clear_render(){
    std::string path = this->directory;

    if (!std::filesystem::exists(path)) {
        std::cerr << "Directory does not exist" << std::endl;
        return;
    }

    std::cout << "Clearing render files" << std::endl;

    for (const auto& dirEntry : recursive_directory_iterator(path)){
        if (dirEntry.path().extension() == ".dot") {
            std::string inputPath = dirEntry.path().string();

            if (std::remove(inputPath.c_str()) != 0) {
                std::cerr << "Error deleting file: " << inputPath << std::endl;
            } else {
                std::cout << "Deleted file: " << inputPath << std::endl;
            }
        }
    }
}
