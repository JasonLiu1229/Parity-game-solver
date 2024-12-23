#include "game.hh"

int Game::adjust_priority(int priority)
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
        new_priority = priority + 1;
    }
    return new_priority;
}

int Game::extract_priority(std::string acc_set)
{
    // extract priority from the acceptance set, so {value} -> value
    std::string priority = acc_set.substr(1, acc_set.size() - 2);
    assert("Priority is not a number" && std::all_of(priority.begin(), priority.end(), ::isdigit));
    assert("More than one priority is present" && priority.find(",") == std::string::npos);
    return std::stoi(priority);
}

void Game::collect_sub_roots(bdd root, int firstVar, std::set<bdd> &collector)
{
    std::stack<bdd> stack;           
    std::unordered_set<int> visited; // Track visited nodes to avoid cycles

    stack.push(root);
    visited.insert(root.id());

    while (!stack.empty())
    {
        bdd current = stack.top();
        stack.pop();

        if (current == bdd_false() || current == bdd_true())
        {
            collector.insert(current);
            continue;
        }

        int var = bdd_var(current);

        if (var < firstVar)
        {
            // Continue exploring the branches
            bdd high = bdd_high(current);
            bdd low = bdd_low(current);

            if (visited.insert(high.id()).second)
            {
                stack.push(high);
            }
            if (visited.insert(low.id()).second)
            {
                stack.push(low);
            }
        }
        else
        {
            // Stop and add current node as a subroot
            collector.insert(current);
        }
    }
}

void Game::extract_vars(bdd root, std::vector<int> &vars){
    if (root == bddfalse || root == bddtrue)
    {
        return;
    }

    int var = bdd_var(root);
    vars.push_back(var);

    extract_vars(bdd_high(root), vars);
    extract_vars(bdd_low(root), vars);
}

bdd Game::encode_state(int state, bdd statevars)
{
    std::vector<int> vars;
    this->extract_vars(statevars, vars);
    
    bdd cube = bddtrue;

    uint32_t state_binary = state;

    for (auto it = vars.rbegin(); it != vars.rend(); ++it)
    {
        int var = *it;

        if (state_binary & 1){
            cube &= bdd_ithvar(var);
        } else {
            cube &= bdd_nithvar(var);
        }

        state_binary >>= 1;
    }

    return cube;
}

bdd Game::encode_priority(int priority, int priobits)
{
    bdd cube = bddtrue;

    // binary version of priority
    uint32_t prio_binary = priority;
    for (int i = 0; i < priobits; ++i)
    {
        if (prio_binary & 1)
        {
            cube &= bdd_ithvar(i);
        }
        else
        {
            cube &= bdd_nithvar(i);
        }
        prio_binary >>= 1;
    }
    return cube;
}

bdd Game::encode_priostate(int state, int priority, bdd statevars, bdd priovars)
{
    std::vector<int> state_var_list;
    std::vector<int> prio_var_list;

    // Extract variables from priovars (priority bits)
    extract_vars(priovars, prio_var_list);

    // Extract variables from statevars (state bits)
    extract_vars(statevars, state_var_list);   
}

void Game::construct_game()
{
    auto bdd_dict = this->automaton->get_dict();

    std::unordered_map<int, bdd> ap_map;
    for (unsigned int i = 0; i < this->automaton->ap().size(); ++i)
    {
        ap_map[i] = bdd_ithvar(i);
    }

    int nextIndex = 0;
    std::vector<int> succ_state;
    std::vector<int> succ_inter;

    std::set<uint64_t> targets;
    std::map<bdd, int> inter_vertices;
    std::map<uint64_t, int> target_vertices;

    const auto cap_count = controllable_aps.size();
    const auto uap_count = this->automaton->ap().size() - cap_count;

    for (unsigned int state = 0; state < this->automaton->num_states(); ++state)
    {
        bdd trans_bdd = bddfalse;
        std::ostringstream oss;
        std::string t_acc_set;

        for (auto &trans : this->automaton->out(state))
        {
            bdd lblbdd = trans.cond;
            int priority = 0;
            int new_priority = 0;
            if (this->automaton->prop_state_acc() != true)
            {
                oss << trans.acc;
                t_acc_set = oss.str();
                priority = this->extract_priority(t_acc_set);
                new_priority = this->adjust_priority(priority);
            }
            uint64_t target_val = ((uint64_t)new_priority << 32) | (uint64_t)trans.dst;

            bdd leaf = bdd_ithvar(target_val);
            trans_bdd |= lblbdd & leaf;
        }

        std::set<bdd> subroots;
        
        this->collect_sub_roots(trans_bdd, uap_count, subroots);

        for (auto &subroot : subroots)
        {
            
        }
    }
}
