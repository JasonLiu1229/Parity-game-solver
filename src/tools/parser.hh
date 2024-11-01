#ifndef PARSER_HH
#define PARSER_HH

#include <iostream>
#include <spot/parseaut/public.hh>
#include <spot/twaalgos/postproc.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/hoa.hh>

class Parser {
public:
    Parser();
    ~Parser();

    static spot::twa_graph_ptr parse(const std::string& filename);
};



#endif // PARSER_HH
