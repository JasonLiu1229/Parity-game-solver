#ifndef GAME_HH
#define GAME_HH

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
#include <cassert>
#include <stack>

class edge
{
    int src;
    int dst;
    bdd condition;
    int priority;
    std::string acc_set;

public:
    edge(int src, int dst, bdd condition, int priority, std::string acc_set) : src(src), dst(dst), condition(condition), priority(priority), acc_set(acc_set) {}
    ~edge() = default;

    int get_src() const { return this->src; }
    int get_dst() const { return this->dst; }
    bdd get_condition() const { return this->condition; }
    int get_priority() const { return this->priority; }
    std::string get_acc_set() const { return this->acc_set; }
};

class vertex
{
    int id;
    std::vector<edge *> edges;
    int owner;
    int priority;

public:
    explicit vertex(int id) : id(id) {}
    ~vertex() = default;

    int get_id() const { return this->id; }
    std::vector<edge *> get_edges() const { return this->edges; }

    void add_edge(edge *e) { this->edges.push_back(e); }

    void set_owner(int owner) { this->owner = owner; }
    void set_priority(int priority) { this->priority = priority; }
    int get_owner() const { return this->owner; }
    int get_priority() const { return this->priority; }
};

class Game
{
    spot::twa_graph_ptr automaton;

    vertex *initial_state = nullptr;
    std::vector<vertex *> vertices;

    bool isMax = true;
    bool isEven = true;
    std::vector<int> controllable_aps;

    int no_priorities = 0;

public:
    explicit Game(const spot::twa_graph_ptr &aut, bool isMax, bool isEven) : automaton(aut), isMax(isMax), isEven(isEven) { this->construct_game(); }
    ~Game() = default;

    spot::twa_graph_ptr get_automaton() const { return this->automaton; }

private:
    // Game creation helper functions
    void construct_game();
    int adjust_priority(int priority);
    int extract_priority(std::string acc_set);
    void collect_sub_roots(bdd root, int firstVar, std::set<bdd> &sub_roots);

    void extract_vars(bdd root, std::vector<int> &vars);
    bdd encode_state(int state, bdd statevars);
    bdd encode_priority(int priority, int priobits);
    bdd encode_priostate(int state, int priority, bdd statevars, bdd priovars);
    bdd collect_targets(bdd trans, std::set<uint64_t> &res, bdd statevars, bdd priovars);

    // Other helper functions
    void add_vertex(vertex *v) { this->vertices.push_back(v); }
    vertex *get_vertex(int id)
    {
        for (auto &v : this->vertices)
        {
            if (v->get_id() == id)
                return v;
        }
        return nullptr;
    }
};

#endif // GAME_HH
