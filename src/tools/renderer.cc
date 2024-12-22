#include "renderer.hh"

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

std::map<std::string, std::vector<std::string>> Renderer::stringify_transitions(spot::twa_graph_ptr &aut)
{
    const spot::bdd_dict_ptr &dict = aut->get_dict();

    std::map<std::string, std::vector<std::string>> aut_str;

    unsigned int num_states = aut->num_states();
    for (unsigned int i = 0; i < num_states; i++)
    {
        for (auto &t : aut->out(i))
        {
            std::string src = std::to_string(i);
            std::string dst = std::to_string(t.dst);
            std::string cond = spot::bdd_format_formula(dict, t.cond);
            std::ostringstream oss;
            std::string t_acc_set;
            if (aut->prop_state_acc() != true)
            {
                oss << t.acc;
                t_acc_set = oss.str();
            }
            else
            {
                t_acc_set = "";
            }
            std::string str = state_name_map[src] + " -> " + state_name_map[dst] + " [label=\"" + cond + t_acc_set + "\"]";
            aut_str[src].push_back(str);
        }
    }
    return aut_str;
}

std::vector<std::string> Renderer::stringify_initial_states(spot::twa_graph_ptr &aut)
{
    std::vector<std::string> initial_states;
    initial_states.push_back(std::to_string(aut->get_init_state_number()));
    return initial_states;
}

std::vector<std::string> Renderer::stringify_acceptance(spot::twa_graph_ptr &aut)
{
    std::vector<std::string> acceptance;

    const spot::acc_cond &acc = aut->acc();

    for (unsigned int i = 0; i < aut->num_states(); i++)
    {
        if (acc.accepting(i))
        {
            acceptance.push_back(std::to_string(i));
        }
    }

    return acceptance;
}

std::vector<std::string> Renderer::stringify_accepting_transitions(spot::twa_graph_ptr &aut)
{
    std::vector<std::string> accepting_transitions;

    const spot::acc_cond &acc = aut->acc();

    for (unsigned int i = 0; i < aut->num_states(); i++)
    {
        if (acc.accepting(i))
        {
            for (auto &t : aut->out(i))
            {
                std::string src = std::to_string(i);
                std::string dst = std::to_string(t.dst);
                std::string cond = spot::bdd_format_formula(aut->get_dict(), t.cond);
                std::ostringstream oss;
                std::string t_acc_set;
                if (!aut->prop_state_acc())
                {
                    oss << t.acc;
                    t_acc_set = oss.str();
                }
                else
                {
                    t_acc_set = "";
                }
                std::string str = state_name_map[src] + " -> " + state_name_map[dst] + " [label=\"" + cond + t_acc_set + "\"]";
                accepting_transitions.push_back(str);
            }
        }
    }

    return accepting_transitions;
}

std::map<std::string, std::vector<std::string>> Renderer::optimize_transitions(const std::map<std::string, std::vector<std::string>> &transitions)
{
}

void Renderer::render_dot(std::string filename, std::string input)
{
    std::ofstream file;
    file.open(filename);
    file << input;
    file.close();
}

bool Renderer::render_image(std::string filename)
{
    std::string new_filename = filename + ".png";
    std::string o_arg = "-o" + new_filename;

    std::vector<char *> args = {
        const_cast<char *>("dot"),
        const_cast<char *>("-Tpng"),
        const_cast<char *>(filename.c_str()),
        const_cast<char *>(o_arg.c_str()),
        nullptr // Ensure null termination
    };

    const int argc = args.size() - 1; // Exclude null terminator
    Agraph_t *g, *prev = NULL;
    GVC_t *gvc = gvContext();

    if (!gvc)
    {
        return false;
    }

    gvParseArgs(gvc, argc, args.data());

    while ((g = gvNextInputGraph(gvc)))
    {
        if (prev)
        {
            gvFreeLayout(gvc, prev);
            agclose(prev);
        }
        gvLayoutJobs(gvc, g);
        gvRenderJobs(gvc, g);
        prev = g;
    }

    if (prev)
    {
        gvFreeLayout(gvc, prev);
        agclose(prev);
    }

    return !gvFreeContext(gvc);
}

void Renderer::render(spot::twa_graph_ptr &aut, std::string filename, bool render)
{
    map_states(aut);

    std::vector<std::string> initial_states = stringify_initial_states(aut);
    std::vector<std::string> acceptance = stringify_acceptance(aut);
    std::map<std::string, std::vector<std::string>> transitions = stringify_transitions(aut);

    std::string dot = "digraph G {\n";

    dot += "{\n";
    for (unsigned int i = 0; i < aut->num_states(); i++)
    {
        std::string state = std::to_string(i);

        dot += state_name_map[state] + "[shape=circle]\n";
    }

    dot += "init [shape=point]\n";

    dot += "}\n";

    for (auto &state : transitions)
    {
        for (auto &transition : state.second)
        {
            dot += transition + "\n";
        }
    }

    for (auto &state : initial_states)
    {
        dot += "init -> " + state_name_map[state] + "\n";
    }

    dot += "}\n";

    std::string::size_type pos = filename.find(".hoa") != std::string::npos ? filename.find(".hoa") : filename.find(".ehoa");
    if (pos != std::string::npos)
    {
        filename = filename.substr(0, pos);
        filename += ".dot";
    }

    pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        filename = filename.substr(pos + 1);
    }

    filename = "../DOT_files/" + filename;

    render_dot(filename, dot);

    if (render == true)
    {
        render_image(filename);
    }
}

void Renderer::map_states(spot::twa_graph_ptr &aut)
{
    for (unsigned int i = 0; i < aut->num_states(); i++)
    {
        std::string state = std::to_string(i);
        std::ostringstream oss;
        std::string acc_str;
        if (aut->prop_state_acc())
        {
            auto acc_sets = aut->state_acc_sets(i);
            oss << acc_sets;
            acc_str = oss.str();
            if (acc_str != "{}")
            {
                state_name_map[state] = "\"" + state + ", " + acc_str + "\"";
            }
            else
            {
                state_name_map[state] = "\"" + state + "\"";
            }
        }
        else
        {
            state_name_map[state] = state;
        }
    }
}
