#include "parser.hh"

Parser::Parser() = default;

Parser::~Parser() = default;

spot::twa_graph_ptr Parser::parse(const std::string &filename)
{
    spot::parsed_aut_ptr pa = spot::parse_aut(filename, spot::make_bdd_dict());

    if (pa == nullptr)
    {
        std::cerr << "Error: could not parse the automaton" << std::endl;
        exit(1);
    }

    if (pa->aborted)
    {
        std::cerr << "Error: parsing aborted" << std::endl;
        exit(1);
    }

    // Parse the controllable APs
    std::ifstream file(filename);
    std::vector<int> controllableAPs;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: could not open the file" << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {
        if (line.find("controllable-AP") == 0) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string values = line.substr(pos + 1);
                std::istringstream iss(values);
                int value;
                
                // Extract each entry and store it
                while (iss >> value) {
                    controllableAPs.push_back(value);
                }
            }
            break;
        }
    }

    file.close();
    this->controllable_aps = controllableAPs;

    this->automaton = pa->aut;

    return pa->aut;
}

spot::twa_graph_ptr Parser::getAutomaton() const
{
    return this->automaton;
}


spot::twa_graph_ptr Parser::getSbaccAutomaton()
{
    spot::twa_graph_ptr aut = this->automaton;

    spot::twa_graph_ptr sbacc_aut = spot::sbacc(aut);

    return sbacc_aut;
}

std::vector<int> Parser::getControllableAPs() const
{
    return this->controllable_aps;
}

