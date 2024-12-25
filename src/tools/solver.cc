#include "solver.hh"

int Solver::get_priority_transition(const spot::twa_graph::edge_storage_t &transition)
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

            // int priority = this->get_priority_transition(t);

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

int Solver::adjust_priority(int priority)
{
    int new_priority = 0;
    if (!this->isMax)
    {
        int max_priority = 2 * ((this->no_priorities + 1) / 2);
        new_priority = max_priority - priority;
    }

    new_priority += 2;

    if (!this->isEven)
    {
        new_priority = priority - 1;
    }
    return new_priority;
}

Vertex* Solver::create_vertex(int id, int priority, int owner)
{
    return new Vertex(id, priority, owner);
}

spot::twa_graph_ptr Solver::create_arena(){

    // create arena
    this->arena = spot::make_twa_graph(this->automaton->get_dict());
    this->arena->new_states(this->automaton->num_states());
    this->arena->set_init_state(this->automaton->get_init_state_number());

    std::vector<Vertex*> queue;

    std::vector<Vertex*> vertices; // new vertices 
    std::vector<Vertex*> visited;

    std::vector<int> uap; // uncontrollable aps

    auto aps = this->automaton->ap();

    for (int i = 0; i < aps.size(); i++)
    {
        bool found = false;
        int ap_int = this->automaton->register_ap(aps[i]);

        for (int j = 0; j < this->controllable_aps.size(); j++)
        {
            if (ap_int == this->controllable_aps[j])
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            uap.push_back(ap_int);
        }
    }

    // convert the init state to vertex
    int init_state = this->automaton->get_init_state_number();
    int priority = this->get_priority(init_state);
    int owner = 1;
    Vertex* init_vertex = this->create_vertex(init_state, this->adjust_priority(priority), owner);

    queue.push_back(init_vertex);
    vertices.push_back(init_vertex);

    /*
    Three cases for the transitions:
    1. Only controllable transitions are present, this is player 0's turn
    2. Only uncontrollable transitions are present, this is player 1's turn
    3. Both controllable and uncontrollable transitions are present, this is player 1's turn first and then we convert to player 0's turn
        - This creates two states, one for player 1 and one for player 0 
    */

    while (!queue.empty())
    {
        Vertex* current = queue.front();
        queue.erase(queue.begin());

        if (std::find(visited.begin(), visited.end(), current) != visited.end())
        {
            continue;
        }

        visited.push_back(current);

        // get the state
        int state = current->id;
        
    }

}


void Solver::solve()
{
    // if (this->automaton->prop_state_acc() != true)
    // {
    //     this->reconstruct_transition_based_to_state_based();
    //     std::cout << "Transition-based automaton reconstructed to state-based automaton" << std::endl;
    // }
    // this->solve_state_based();
    
    std::vector<bool> owners;

    for (int i = 0; i < this->automaton->num_states(); i++)
    {
        owners.push_back(true);
    }
    spot::set_state_players(this->automaton, owners);

    bool output = spot::solve_parity_game(this->automaton);
    std::cout << output << std::endl;
}
