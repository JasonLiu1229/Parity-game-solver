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

BTTree* recursive_btree(std::string condition){
    BTTree *tree = new BTTree();

    if(condition.find("&") != std::string::npos){
        std::string left = condition.substr(0, condition.find("&"));
        std::string right = condition.substr(condition.find("&") + 1);
        tree->root->left = recursive_btree(left)->root;
        tree->root->right = recursive_btree(right)->root;
        tree->root->type = NodeType::AND;
    } else if(condition.find("|") != std::string::npos){
        std::string left = condition.substr(0, condition.find("|"));
        std::string right = condition.substr(condition.find("|") + 1);
        tree->root->left = recursive_btree(left)->root;
        tree->root->right = recursive_btree(right)->root;
        tree->root->type = NodeType::OR;
    } else if(condition.find("!") != std::string::npos){
        std::string right = condition.substr(condition.find("!") + 1);
        tree->root->right = recursive_btree(right)->root;
        tree->root->type = NodeType::NOT;
    } else {
        tree->root->type = NodeType::AP;
    }

}

BTTree *Game::create_transition_based_tree(bdd condition)
{
    BTTree *tree = new BTTree();
    const spot::bdd_dict_ptr &dict = this->automaton->get_dict();
    std::string condition_str = spot::bdd_format_formula(dict, condition);

    std::cout << "Condition: " << condition_str << std::endl;

    // Split the condition string based on the operator
    std::vector<std::string> split_condition;
    std::string temp = "";
    for (char &c : condition_str)
    {
        if (c == '&' || c == '|')
        {
            split_condition.push_back(temp);
            temp = "";
        }
        else
        {
            temp += c;
        }
    }
    return tree;
}

void Game::construct_game()
{
    // Convert the atomic propositions to strings
    this->convert_ap();

    // convert transitions condition to tree
    for (int i = 0; i < this->automaton->num_states(); i++)
    {
        State *state = new State(i);
        this->states.push_back(state);
    }

    for (int i = 0; i < this->automaton->num_states(); i++)
    {
        State *state = this->states[i];
        for (auto &trans : this->automaton->out(i))
        {
            State *next_state = this->states[trans.dst];
            BTTree *tree = this->create_transition_based_tree(trans.cond);
        }
    }
}
