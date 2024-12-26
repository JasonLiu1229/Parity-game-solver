#ifndef SOLVER_HH
#define SOLVER_HH

#include <spot/parseaut/public.hh>
#include <spot/twaalgos/hoa.hh>
#include <spot/twa/bddprint.hh>
#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/translate.hh>
#include <spot/twaalgos/game.hh>
#include <spot/twaalgos/parity.hh>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>

class Vertex
{
public:
    int id;
    int priority;
    int owner;
    bdd condition;
    Vertex(int id, int priority, int owner) : id(id), priority(priority), owner(owner) {}
    ~Vertex() = default;
};

class Solver
{
    spot::twa_graph_ptr automaton;
    spot::twa_graph_ptr arena = nullptr;

    std::vector<int> controllable_aps;
    bool isMax = true;
    bool isEven = true;
    int no_priorities = 0;

public:
    explicit Solver(const spot::twa_graph_ptr &aut) : automaton(aut) {}
    explicit Solver(const spot::twa_graph_ptr &aut, const std::vector<int> &controllable_aps) : automaton(aut), controllable_aps(controllable_aps) {}
    explicit Solver(const spot::twa_graph_ptr &aut, const std::vector<int> &controllable_aps, bool isMax, bool isEven, int no_priorities) : automaton(aut), controllable_aps(controllable_aps), isMax(isMax), isEven(isEven), no_priorities(no_priorities) {}
    explicit Solver() = default;
    ~Solver() = default;

    void solve();

    spot::twa_graph_ptr get_automaton() const { return this->automaton; }
    spot::twa_graph_ptr get_arena() const { return this->arena; }

private:
    int get_priority(int state);

    int get_priority_transition(const spot::twa_graph::edge_storage_t &transition);

    void reconstruct_transition_based_to_state_based();

    void build_state_based_game();

    void solve_state_based();

    std::set<int> attractor(const std::set<int> &target); // assume we are always player 0

    void zielonka_recursive(std::unordered_set<int> &winning, std::unordered_set<int> &losing, int player);

    std::pair<std::unordered_set<int>, std::unordered_set<int>> zielonka(int player);

    void create_arena();

    Vertex *create_vertex(int id, int priority, int owner);

    int adjust_priority(int priority);

    std::vector<int> get_subset_aps_from_cond(bdd cond, const std::vector<int> &ap);

    std::vector<bool> generate_binary_combinations(int n, int size);

    bdd itbdd(int i);
};

#endif // SOLVER_HH
