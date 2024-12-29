#include "arena_graph.hh"

void ArenaGraph::parse_arena()
{
    std::cout << "Parsing arena" << std::endl;

    for (int i = 0; i < this->arena->num_states(); i++)
    {
        Node node;
        node.id = i;

        spot::get_state_player(this->arena, i) == 1 ? node.owner = 1 : node.owner = 0;

        for (auto &t : this->arena->out(i))
        {
            node.successors.push_back(t.dst);
        }
    }

    std::cout << "Parsing done" << std::endl;
}

void ArenaGraph::render()
{
    std::cout << "Rendering arena" << std::endl;

    std::string dot = "digraph G {\n";

    for (auto &node : this->nodes)
    {
        std::string owner = node.owner == 1 ? "blue" : "red";
        std::string owner_shape = node.owner == 1 ? "diamond" : "square";
        dot += std::to_string(node.id) + " [shape=" + owner_shape + ",color=" + owner + "]\n";

        for (auto &succ : node.successors)
        {
            dot += std::to_string(node.id) + " -> " + std::to_string(succ) + "\n";
        }
    }

    dot += "}\n";

    std::ofstream file("../output/arena.dot");
    file << dot;
    file.close();

    std::cout << "Rendering done" << std::endl;
}
