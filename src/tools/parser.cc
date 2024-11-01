#include "parser.hh"

Parser::Parser() = default;

Parser::~Parser() = default;

spot::twa_graph_ptr Parser::parse(const std::string& filename) {
    spot::parsed_aut_ptr pa = spot::parse_aut(filename, spot::make_bdd_dict());
    if (pa == nullptr) {
        std::cerr << "Error: could not parse the automaton" << std::endl;
        exit(1);
    }
    if (pa->aborted) {
        std::cerr << "Error: parsing aborted" << std::endl;
        exit(1);
    }
    
    return pa->aut;
}
