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
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

class Transition_Identifier
{
public:
    int src;
    int dst;
    int hash_cond;
    int priority;
    Transition_Identifier(int src, int dst, int hash_cond) : src(src), dst(dst), hash_cond(hash_cond) {}
    ~Transition_Identifier() = default;

    bool operator==(const Transition_Identifier &other) const noexcept
    {
        return this->src == other.src && this->dst == other.dst && this->hash_cond == other.hash_cond;
    }
};

namespace std
{
    template <>
    struct hash<Transition_Identifier>
    {
        std::size_t operator()(const Transition_Identifier &t) const noexcept
        {
            return std::hash<int>()(t.src) ^ std::hash<int>()(t.dst) ^ std::hash<int>()(t.hash_cond);
        }
    };
}

class Vertex
{
public:
    int id;
    int automaton_id;
    int priority;
    int owner;

    std::unordered_map<Transition_Identifier, bool> conditions;

    Vertex(int id, int priority, int owner) : id(id), priority(priority), owner(owner) {}
    ~Vertex() = default;
};

class Solver
{
    spot::twa_graph_ptr automaton;
    spot::twa_graph_ptr arena = nullptr;

    std::vector<bdd> partial_evaluations;
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

    void create_arena();

    int adjust_priority(int priority);

    std::vector<int> get_subset_aps_from_cond(bdd cond, const std::vector<int> &ap);

    std::vector<bool> generate_binary_combinations(int n, int size);
};

#endif // SOLVER_HH
