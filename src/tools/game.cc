#include "game.hh"


int Game::adjust_priority(int priority)
{
    int new_priority = 0;
    if (!this->isMax)
    {
        int max_priority = 2*((this->no_priorities + 1) / 2);
        new_priority = max_priority - priority;
    }
    
    new_priority += 2;

    if (!this->isEven)
    {
        new_priority = priority + 1;
    }
    return new_priority;
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

    for (unsigned int state = 0; state < this->automaton->num_states(); ++state)
    {
        bdd trans_bdd = bddfalse;

        for (auto &trans : this->automaton->out(state))
        {
            bdd lblbdd = trans.cond;
        }
    }
}
