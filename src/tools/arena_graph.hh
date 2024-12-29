#ifndef ARENA_GRAPH_HH
#define ARENA_GRAPH_HH

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>

#include <spot/twa/twagraph.hh>
#include <spot/twaalgos/game.hh>

typedef struct Node
{
    int id;
    int owner;
    std::set<int> successors;
} Node;

class ArenaGraph
{
    spot::twa_graph_ptr arena;

    std::vector<Node> nodes;

private:
public:
    ArenaGraph() = default;
    ArenaGraph(spot::twa_graph_ptr arena) : arena(arena) { this->parse_arena(); }
    ~ArenaGraph() = default;

    void render();

    void parse_arena();
};

#endif // ARENA_GRAPH_HH
