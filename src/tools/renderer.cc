#include "renderer.hh"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

std::map<std::string, std::vector<std::string>> Renderer::stringify_transitions(spot::twa_graph_ptr& aut) {
    const spot::bdd_dict_ptr& dict = aut->get_dict();

    std::map<std::string, std::vector<std::string>> aut_str;

    unsigned int num_states = aut->num_states();
    for (unsigned int i = 0; i < num_states; i++) {
        for (auto& t: aut->out(i)) {
            std::string src = std::to_string(i);
            std::string dst = std::to_string(t.dst);
            std::string cond = spot::bdd_format_formula(dict, t.cond);
            std::string str = src + " -> " + dst + " [label=\"" + cond + "\"]";
            aut_str[src].push_back(str);
        }
    }
    return aut_str;
}

std::vector<std::string> Renderer::stringify_initial_states(spot::twa_graph_ptr& aut) {
    std::vector<std::string> initial_states;
    initial_states.push_back(std::to_string(aut->get_init_state_number()));
    return initial_states;
}

std::vector<std::string> Renderer::stringify_acceptance(spot::twa_graph_ptr& aut) {
    std::vector<std::string> acceptance;

    const spot::acc_cond& acc = aut->acc();

    for (unsigned int i = 0; i < aut->num_states(); i++) {
        if (acc.accepting(i)) {
            acceptance.push_back(std::to_string(i));
        }
    }

    return acceptance;
}

std::vector<std::string> Renderer::stringify_accepting_transitions(spot::twa_graph_ptr& aut) {
    std::vector<std::string> accepting_transitions;

    const spot::acc_cond& acc = aut->acc();

    for (unsigned int i = 0; i < aut->num_states(); i++) {
        if (acc.accepting(i)) {
            for (auto& t: aut->out(i)) {
                std::string src = std::to_string(i);
                std::string dst = std::to_string(t.dst);
                std::string cond = spot::bdd_format_formula(aut->get_dict(), t.cond);
                std::string str = src + " -> " + dst + " [label=\"" + cond + "\"]";
                accepting_transitions.push_back(str);
            }
        }
    }

    return accepting_transitions;
}

void Renderer::render_dot(std::string filename, std::string input) {
    std::ofstream file;
    file.open(filename);
    file << input;
    file.close();
}

void Renderer::render(spot::twa_graph_ptr& aut, std::string filename) {
    std::vector<std::string> initial_states = stringify_initial_states(aut);
    std::vector<std::string> acceptance = stringify_acceptance(aut);
    std::map<std::string, std::vector<std::string>> transitions = stringify_transitions(aut);

    std::string dot = "digraph G {\n";

    for (unsigned int i = 0; i < aut->num_states(); i++) {
        std::string state = std::to_string(i);
        if (std::find(acceptance.begin(), acceptance.end(), state) != acceptance.end()) {
            dot += state + " [shape=circle, peripheries=2]\n";
        } else {
            dot += state + " [shape=circle]\n";
        }
    }

    for (auto& state: transitions) {
        for (auto& transition: state.second) {
            dot += transition + "\n";
        }
    }

    for (auto& state: initial_states) {
        dot += "init [shape=point]\n";
        dot += "init -> " + state + "\n";
    }

    dot += "}\n";

    std::string::size_type pos = filename.find(".hoa") != std::string::npos ? filename.find(".hoa") : filename.find(".ehoa");
    if (pos != std::string::npos) {
        filename.replace(pos, 4, ".dot");
    }

    pos = filename.find_last_of("/");
    if (pos != std::string::npos) {
        filename = filename.substr(pos + 1);
    }

    filename = "../DOT_files/" + filename;

    render_dot(filename, dot);
}
