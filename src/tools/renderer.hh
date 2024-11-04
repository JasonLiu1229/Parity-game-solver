#ifndef RENDERER_HH
#define RENDERER_HH

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include <spot/twa/twagraph.hh>
#include <spot/twa/bddprint.hh>
#include <spot/twaalgos/sbacc.hh>

#include <graphviz/gvc.h>

class Renderer {
private:
    /* Helper functions */
    static std::map<std::string, std::vector<std::string>> stringify_transitions(spot::twa_graph_ptr& aut);
    static std::vector<std::string> stringify_initial_states(spot::twa_graph_ptr& aut);
    static std::vector<std::string> stringify_acceptance(spot::twa_graph_ptr& aut);
    static std::vector<std::string> stringify_accepting_transitions(spot::twa_graph_ptr& aut);

    static void render_dot(std::string filename, std::string input);
    static bool render_image(std::string filename);
public:
    Renderer();
    ~Renderer();

    static void render(spot::twa_graph_ptr& aut, std::string filename = "some_random_name.hoa", bool render = false);
};



#endif //RENDERER_HH
