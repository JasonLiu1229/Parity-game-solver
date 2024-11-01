#include <iostream>

#include "tools/parser.hh"
#include "tools/renderer.hh"

#define INPUT_FILE "../HOA_files/aut1.hoa"

int main() {
    Parser parser;
    auto aut = Parser::parse(INPUT_FILE);
    Renderer::render(aut, INPUT_FILE);
}
