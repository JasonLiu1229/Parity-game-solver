#ifndef SOLVER_HH
#define SOLVER_HH

#include <spot/parseaut/public.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twa/bddprint.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/translate.hh>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>

class Solver
{   
    spot::twa_graph_ptr automaton;
    std::vector<int> controllable_aps;

    public: 
        explicit Solver(const spot::twa_graph_ptr &aut) : automaton(aut) {}
        explicit Solver(const spot::twa_graph_ptr &aut, const std::vector<int> &controllable_aps) : automaton(aut), controllable_aps(controllable_aps) {}
        explicit Solver() = default;
        ~Solver() = default;

        int check_player(int state);

        void solve();

        spot::twa_graph_ptr get_automaton() const { return this->automaton; }
    
    private:
        int get_priority(int state);

        int get_priority_transition(const spot::twa_graph::edge_storage_t& transition);

        void reconstruct_transition_based_to_state_based();

        void build_state_based_game();

        void solve_state_based();
        
        std::unordered_set<int> attractor(const std::unordered_set<int> &target); // assume we are always player 0

        std::pair<std::unordered_set<int>, std::unordered_set<int>> zielonka(const std::unordered_set<int>& states);
};

#endif // SOLVER_HH
