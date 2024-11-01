#include <iostream>

#include "tools/tester.hh"

#define INPUT_FILE "../HOA_files/lilydemo01.tlsf.ehoa"
#define TEST_DIR "../HOA_files"

int main() {
    // Parser parser;
    // auto aut = Parser::parse(INPUT_FILE);
    // Renderer::render(aut, INPUT_FILE);

    Tester tester(TEST_DIR);
    tester.run_render();
}
