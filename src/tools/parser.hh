#ifndef PARSER_HH
#define PARSER_HH

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cctype>

#include <spot/parseaut/public.hh>
#include <spot/twaalgos/postproc.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/sbacc.hh>

class Parser
{
    spot::twa_graph_ptr automaton;
    std::vector<int> controllable_aps;

    bool is_max = true;
    bool is_even = true;

    int no_priorities = 0;

public:
    Parser();
    ~Parser();

    spot::twa_graph_ptr parse(const std::string &filename);

    [[nodiscard]] spot::twa_graph_ptr getAutomaton() const;

    spot::twa_graph_ptr getSbaccAutomaton();

    [[nodiscard]] std::vector<int> getControllableAPs() const;

    [[nodiscard]] bool isMax() const { return this->is_max; }

    [[nodiscard]] bool isEven() const { return this->is_even; }
};

#endif // PARSER_HH
