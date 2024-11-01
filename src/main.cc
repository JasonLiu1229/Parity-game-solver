#include <iostream>

#include "tools/parser.hh"
#include "tools/renderer.hh"


int main() {
    Parser parser;
    auto aut = Parser::parse("../HOA_files/abcg_arbiter.tlsf.ehoa");
    // Renderer::render(aut);
    std::vector<std::string> acceptance = Renderer::stringify_acceptance(aut);
}
