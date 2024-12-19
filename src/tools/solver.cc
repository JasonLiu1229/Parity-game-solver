#include "solver.hh"

int Solver::get_priority_transition(const spot::twa_graph::edge_storage_t& transition)
{
    int priority = 0;

    std::ostringstream oss;
    oss << transition.acc;
    std::string acc_set = oss.str();

    acc_set = acc_set.substr(1, acc_set.size() - 2);

    priority = std::stoi(acc_set);

    return priority;
}

int Solver::get_priority(int state)
{
    int priority = 0;
    auto acc_sets = this->automaton->state_acc_sets(state);

    std::ostringstream oss;
    oss << acc_sets;
    std::string acc_set = oss.str();

    acc_set = acc_set.substr(1, acc_set.size() - 2);

    priority = std::stoi(acc_set);

    return priority;
}

void Solver::reconstruct_transition_based_to_state_based()
{
    const spot::bdd_dict_ptr &original_dict = this->automaton->get_dict();
    spot::twa_graph_ptr new_automaton = spot::make_twa_graph(original_dict);

    // copy states
    for (int i = 0; i < this->automaton->num_states(); i++)
    {
        new_automaton->new_state();
    }

    // process states
    for (unsigned int i = 0; i < this->automaton->num_states(); i++)
    {
        for (auto &t : this->automaton->out(i))
        {
            int src = i;
            int dst = t.dst;
            std::string cond = spot::bdd_format_formula(original_dict, t.cond);

            //int priority = this->get_priority_transition(t);

            int intermediate = new_automaton->new_state();

            new_automaton->new_edge(src, intermediate, t.cond);

            new_automaton->new_edge(intermediate, dst, bddtrue, t.acc);
        }
    }

    // copy aps
    for (auto &ap : this->automaton->ap())
    {
        new_automaton->register_ap(ap);
    }

    // copy initial states
    new_automaton->set_init_state(this->automaton->get_init_state_number());

    this->automaton = new_automaton;
}

int Solver::check_player(int state)
{
    std::vector<std::string> converted_ap;
    const spot::bdd_dict_ptr &dict = this->automaton->get_dict();

    std::vector<bdd> con_aps;

    for (auto &ap : this->controllable_aps)
    {
        con_aps.push_back(bdd_ithvar(ap));
    }

    for (auto &t : this->automaton->out(state)) {
        if (t.cond == bddtrue) {
            return 1;
        }
        else {
            std::string cond = spot::bdd_format_formula(dict, t.cond);
            for (auto &ap : con_aps) {
                std::string con_ap_str = spot::bdd_format_formula(dict, ap);
                // if con ap is in the condition
                if (cond.find(con_ap_str) != std::string::npos)
                {
                    return 0;
                }

            }
        
        }
    }
    return 1;
}

void Solver::solve()
{
    if (this->automaton->prop_state_acc() != true)
    {
        this->reconstruct_transition_based_to_state_based();
        std::cout << "Transition-based automaton reconstructed to state-based automaton" << std::endl;
    }
    // this->solve_state_based();
    std::cout << this->check_player(1) << std::endl;
}

std::unordered_set<int> Solver::attractor(const std::unordered_set<int> &target)
{
    
}
