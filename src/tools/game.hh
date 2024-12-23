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

enum class NodeType
{
    AND,
    OR,
    NOT,
    BOOL,
    AP,
    ALIAS
};

typedef struct BTTree
{
    struct Node
    {
        NodeType type;
        Node *left;
        Node *right;
        Node *parent;
        int value;

        Node(NodeType type, int priority) : type(type), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node *root;
} BTTree;

class Edge
{
    int src;
    int dst;
    BTTree *label;
    int priority;
    std::string acc_set;

public:
    Edge(int src, int dst, BTTree *label, int priority, std::string acc_set) : src(src), dst(dst), label(label), priority(priority), acc_set(acc_set) {}
    ~Edge() = default;

    int get_src() const { return this->src; }
    int get_dst() const { return this->dst; }
    int get_priority() const { return this->priority; }
    std::string get_acc_set() const { return this->acc_set; }
    BTTree *get_label() const { return this->label; }
};

class State
{
    int id;
    std::vector<Edge *> out_edges;
    std::vector<Edge *> in_edges;
    int priority = 0;

public:
    explicit State(int id) : id(id) {}
    ~State() = default;

    int get_id() const { return this->id; }

    void add_out_edge(Edge *e) { this->out_edges.push_back(e); }
    void add_in_edge(Edge *e) { this->in_edges.push_back(e); }

    std::vector<Edge *> get_out_edges() const { return this->out_edges; }
    std::vector<Edge *> get_in_edges() const { return this->in_edges; }

    void set_priority(int priority) { this->priority = priority; }
    int get_priority() const { return this->priority; }
};

class Vertex
{
    int id;
    int owner;
    int priority;

    std::vector<Vertex *> adj_list;

public:
    explicit Vertex(int id) : id(id) {}
    ~Vertex() = default;

    int get_id() const { return this->id; }

    void set_owner(int owner) { this->owner = owner; }
    void set_priority(int priority) { this->priority = priority; }
    int get_owner() const { return this->owner; }
    int get_priority() const { return this->priority; }
};

class Game
{
    spot::twa_graph_ptr automaton;

    State *initial_state_hoa = nullptr;
    std::vector<State *> states;
    std::vector<Edge *> edges;

    Vertex *initial_state_game = nullptr;
    std::vector<Vertex *> vertices;

    bool isMax = true;
    bool isEven = true;
    std::vector<int> controllable_aps;

    int no_priorities = 0;

    // other info
    std::vector<std::string> converted_ap;

public:
    explicit Game(
        const spot::twa_graph_ptr &aut,
        bool isMax,
        bool isEven,
        std::vector<int> controllable_aps) : automaton(aut), isMax(isMax), isEven(isEven), controllable_aps(controllable_aps) { this->construct_game(); }
    ~Game() = default;

    spot::twa_graph_ptr get_automaton() const { return this->automaton; }

private:
    // Game creation helper functions
    void construct_game();

    BTTree *create_transition_based_tree(bdd condition);

    int adjust_priority(int priority);
    int extract_priority(std::string acc_set);

    // Other helper functions
    void add_vertex(Vertex *v) { this->vertices.push_back(v); }
    Vertex *get_vertex(int id)
    {
        for (auto &v : this->vertices)
        {
            if (v->get_id() == id)
                return v;
        }
        return nullptr;
    }

    void convert_ap()
    {
        for (auto &ap : this->automaton->ap())
        {
            std::string ap_str = spot::bdd_format_formula(this->automaton->get_dict(), bdd_ithvar(this->automaton->register_ap(ap)));
            this->converted_ap.push_back(ap_str);
        }
    }
};

#endif // GAME_HH
