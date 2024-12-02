#ifndef SOLVER_HH
#define SOLVER_HH

#include <spot/parseaut/public.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twa/bddprint.hh>
#include <spot/twa/twagraph.hh>

#include <iostream>
#include <vector>
#include <unordered_map>
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

        void solve();
    
    private:
        int getPriority(int state);

        void getControllableAPs();



        
};

#endif // SOLVER_HH
