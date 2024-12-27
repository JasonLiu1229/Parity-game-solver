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

    if (acc_set == "{}")
    {
        return -1;
    }

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

            new_automaton->new_edge(intermediate, dst, bdd_true(), t.acc);
        }
    }

    // copy aps
    for (auto &ap : this->automaton->ap())
    {
        new_automaton->register_ap(ap);
    }

    // copy initial states
    new_automaton->set_init_state(this->automaton->get_init_state_number());

    // copy acceptance
    new_automaton->set_acceptance(this->automaton->get_acceptance());

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

Vertex *Solver::create_vertex(int id, int priority, int owner)
{
    return new Vertex(id, priority, owner);
}

std::vector<bool> Solver::generate_binary_combinations(int n, int size)
{
    std::vector<bool> values(size, false);

    for (int i = 0; i < n; i++)
    {
        values[i] = n % 2;
        n /= 2;
    }

    return values;
}

// int to bdd
bdd Solver::itbdd(int i)
{
    spot::bdd_dict_ptr dict = this->automaton->get_dict();

    auto variable_map = dict->var_map;

    assert(i >= variable_map.size());

    for (auto &it : variable_map)
    {
        if (i == it.second)
        {
            return bdd_ithvar(it.second);
        }
    }
    return bdd_true();
}

std::vector<int> Solver::get_subset_aps_from_cond(bdd cond, const std::vector<int> &ap)
{
    spot::bdd_dict_ptr dict = this->automaton->get_dict();

    std::string cond_str = spot::bdd_format_formula(dict, cond);
    std::vector<int> subset_aps;

    auto variable_map = dict->var_map;

    for (auto &var : variable_map)
    {
        if (!(std::find(ap.begin(), ap.end(), var.second) != ap.end()))
        {
            continue;
        }
        std::ostringstream oss;
        oss << var.first;
        std::string var_str = oss.str();

        if (cond_str.find(var_str) != std::string::npos)
        {
            subset_aps.push_back(var.second);
        }
    }

    // ============================

    return subset_aps;
}

void Solver::create_arena()
{
    spot::bdd_dict_ptr dict = this->automaton->get_dict();

    // create arena
    this->arena = spot::make_twa_graph(dict);
    this->arena->new_states(10); // start with 10 states because it is HUUUUUUUGE
    this->arena->set_init_state(this->automaton->get_init_state_number());

    std::vector<Vertex *> queue;

    std::vector<Vertex *> vertices; // new vertices

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
    Vertex *init_vertex = this->create_vertex(init_state, this->adjust_priority(priority), owner);

    queue.push_back(init_vertex);
    vertices.push_back(init_vertex);

    while (!queue.empty())
    {
        Vertex *current = queue.front();
        queue.erase(queue.begin());

        if (current->owner == 0)
        {
            // check if conditions are processed
            bool condition_check = true;
            for (auto &cond : current->conditions)
            {
                if (cond.second == false)
                {
                    condition_check = false;
                    break;
                }
            }

            if (condition_check)
            {
                continue;
            }
        }

        // get the state
        int state = current->id;
        int current_owner = current->owner;
        for (auto &t : this->automaton->out(state))
        {
            int dst = t.dst;

            std::vector<int> cond_uap = this->get_subset_aps_from_cond(t.cond, uap);
            std::vector<int> cond_cap = this->get_subset_aps_from_cond(t.cond, this->controllable_aps);

            unsigned int src_priority = current->priority;
            std::initializer_list<unsigned int> src_prio_formatted = {};
            if (src_priority != -1)
            {
                src_prio_formatted = {src_priority};
            }

            if (current->owner == 1)
            {
                /*
                generate every possible combination of uncontrolled aps,
                if the bdd results in true,
                then we can go to the next state
                else check if bdd has controlled aps
                then go to next state with owner 0
                else do nothing
                */

                std::vector<bool> values(cond_uap.size(), false);

                for (int i = 0; i < cond_uap.size(); i++)
                {
                    values = this->generate_binary_combinations(i, cond_uap.size());

                    bdd assignment = t.cond;
                    // assign values to the condition
                    for (int j = 0; j < values.size(); ++j)
                    {
                        if (values[j])
                        {
                            assignment = bdd_restrict(assignment, bdd_ithvar(cond_uap[j]));
                        }
                        else
                        {
                            assignment = bdd_restrict(assignment, bdd_nithvar(cond_uap[j]));
                        }
                    }

                    // check if the condition is true
                    if (assignment == bddtrue)
                    {
                        owner = 1;
                        // check if dst is already in the vertices with owner 1
                        bool found = false;
                        for (auto &v : vertices)
                        {
                            if (v->id == dst && v->owner == 1)
                            {
                                this->arena->new_edge(current->id, v->id, t.cond, src_prio_formatted);
                                found = true;
                                break;
                            }
                        }
                        if (found)
                        {
                            continue;
                        }
                        auto dst_priority = this->get_priority(dst);
                        Vertex *new_vertex = this->create_vertex(dst, this->adjust_priority(dst_priority), owner);
                        new_vertex->automaton_id = dst;
                        new_vertex->conditions.try_emplace(t.cond, true);
                        queue.push_back(new_vertex);
                        vertices.push_back(new_vertex);

                        // check if the id is already in the arena
                        if (new_vertex->id >= this->arena->num_states())
                        {
                            int new_state = this->arena->new_state();
                            new_vertex->id = new_state;
                        }

                        this->arena->new_edge(current->id, new_vertex->id, t.cond, src_prio_formatted);
                        continue;
                    }

                    // check if the condition has controlled aps -> partial evaluation
                    if (cond_cap.size() > 0)
                    {
                        owner = 0;
                        // check if dst is already in the vertices with owner 1
                        bool found = false;
                        for (auto &v : vertices)
                        {
                            if (v->id == dst && v->owner == 1)
                            {
                                v->conditions.try_emplace(t.cond, false);
                                this->arena->new_edge(current->id, v->id, t.cond, src_prio_formatted);
                                found = true;
                                queue.push_back(v);
                                break;
                            }
                        }
                        if (found)
                        {
                            continue;
                        }
                        auto dst_priority = this->get_priority(dst);
                        Vertex *new_vertex = this->create_vertex(dst, this->adjust_priority(dst_priority), owner);
                        new_vertex->automaton_id = dst;
                        new_vertex->conditions.try_emplace(t.cond, false);
                        queue.push_back(new_vertex);
                        vertices.push_back(new_vertex);

                        // check if the id is already in the arena
                        if (new_vertex->id >= this->arena->num_states())
                        {
                            int new_state = this->arena->new_state();
                            new_vertex->id = new_state;
                        }

                        this->arena->new_edge(current->id, new_vertex->id, t.cond, src_prio_formatted);
                        continue;
                    }
                }
            } // case 2 if our current owner is player 0 => then we can only manipulate controlled ap
            else if (current->owner == 0)
            {
                /*
                generate every possible combination of controlled aps,
                if the bdd results in true,
                then we can go to the next state
                else do nothing
                */
                std::vector<bool> values(cond_cap.size(), false);
            }
        }
    }
}

void Solver::solve()
{
    if (this->automaton->prop_state_acc() != true)
    {
        this->reconstruct_transition_based_to_state_based();
        std::cout << "Transition-based automaton reconstructed to state-based automaton" << std::endl;
    }

    // std::vector<bool> owners;

    // for (int i = 0; i < this->automaton->num_states(); i++)
    // {
    //     owners.push_back(true);
    // }
    // spot::set_state_players(this->automaton, owners);

    this->create_arena();

    bool output = spot::solve_parity_game(this->arena);
    // spot::highlight_strategy(this->arena);
    std::cout << output << std::endl;
}
