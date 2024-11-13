#ifndef PARSER_HH
#define PARSER_HH

#include <iostream>
#include <spot/parseaut/public.hh>
#include <spot/twaalgos/postproc.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twaalgos/sbacc.hh>

class Parser
{
    spot::twa_graph_ptr automaton;

public:
    Parser();
    ~Parser();

    spot::twa_graph_ptr parse(const std::string &filename);

    [[nodiscard]] spot::twa_graph_ptr getAutomaton() const;

    spot::twa_graph_ptr getSbaccAutomaton();
};

#endif // PARSER_HH
