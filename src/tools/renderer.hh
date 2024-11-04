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

typedef std::pair<std::string, std::string> stringpair_t;

// Max support of 21 colors

const stringpair_t map_start_values[] = {
    stringpair_t("red", "1"),
    stringpair_t("green", "2"),
    stringpair_t("blue", "3"),
    stringpair_t("yellow", "4"),
    stringpair_t("orange", "5"),
    stringpair_t("purple", "6"),
    stringpair_t("brown", "7"),
    stringpair_t("pink", "8"),
    stringpair_t("gray", "9"),
    stringpair_t("cyan", "10"),
    stringpair_t("magenta", "11"),
    stringpair_t("turquoise", "12"),
    stringpair_t("gold", "13"),
    stringpair_t("silver", "14"),
    stringpair_t("black", "15"),
    stringpair_t("white", "16"),
    stringpair_t("maroon", "17"),
    stringpair_t("lime", "18"),
    stringpair_t("olive", "19"),
    stringpair_t("navy", "20"),
    stringpair_t("black", "21"),
};

const int map_start_values_size = sizeof(map_start_values) / sizeof(map_start_values[0]);

const std::map<std::string, std::string> color_map(map_start_values, map_start_values + map_start_values_size);

class Renderer
{
private:
    /* Helper functions */
    static std::map<std::string, std::vector<std::string>> stringify_transitions(spot::twa_graph_ptr &aut);
    static std::vector<std::string> stringify_initial_states(spot::twa_graph_ptr &aut);
    static std::vector<std::string> stringify_acceptance(spot::twa_graph_ptr &aut);
    static std::vector<std::string> stringify_accepting_transitions(spot::twa_graph_ptr &aut);

    static std::map<std::string, std::vector<std::string>> optimize_transitions(const std::map<std::string, std::vector<std::string>> &transitions);

    static void render_dot(std::string filename, std::string input);
    static bool render_image(std::string filename);

public:
    Renderer();
    ~Renderer();

    static void render(spot::twa_graph_ptr &aut, std::string filename = "some_random_name.hoa", bool render = false);
};

#endif // RENDERER_HH
