#include "ai.h"
#include <algorithm>
#include <cmath>

Node::Node(Node* parent, int action, Color color)
    : parent(parent)
    , action(action)
    , terminal(false)
    , color(color)
    , visits(0)
    , reward(0)
{
}

AI::AI(int iterations)
    : m_iterations(iterations)
    , m_engine(m_randomDevice())
{
}

int AI::getAction(Grid& grid, Color color)
{
    Node root(nullptr, -1, color);

    // Run desired number of iterations of MCTS
    for (int iteration = 0; iteration < m_iterations; ++iteration) {
        monteCarloTreeSearch(&root, grid);
    }
    return bestAction(root);
}

void AI::monteCarloTreeSearch(Node* root, Grid grid)
{
    // Selection (+ Expansion) of leaf node
    Node* leaf = select(root, grid);

    // Simulation, i.e. random playout until end
    Result result = simulate(grid, leaf->color);

    // Adjust values in game tree nodes
    backUp(leaf, result);
}

Node* AI::select(Node* node, Grid& grid) const
{
    if (node->terminal) {
        // can't go any further
        return node;
    }

    if (node->children.size() < grid.legalActions().size()) {
        // there is an action that that has not yet been explored
        return expand(node, grid);
    }

    // all actions have been explored at least once, so we can use tree policy
    // to pick child
    Node* child = treePolicy(node);
    grid.place(child->action, node->color);
    return select(child, grid);
}

Node* AI::expand(Node* node, Grid& grid) const
{
    for (int action : grid.legalActions()) {
        if (std::find_if(node->children.begin(), node->children.end(),
                [action](const Node& child) { return child.action == action; })
            == node->children.end()) {
            // No child for this action yet, generate it
            Color color
                = node->color == Color::Red ? Color::Yellow : Color::Red;
            node->children.emplace_back(node, action, color);
            grid.place(action, node->color);
            // Cache whether child is terminal for faster traversal of tree
            if (grid.hasFourConnected() || grid.isFull()) {
                node->children.back().terminal = true;
            }
            return &(node->children.back());
        }
    }
    return nullptr;
}

Result AI::simulate(Grid& grid, Color color)
{
    while (!grid.hasFourConnected() && !grid.isFull()) {
        // Pick action uniformly at random from legal actions
        auto actions = grid.legalActions();
        std::uniform_int_distribution<> uniform(0, actions.size() - 1);
        grid.place(actions[uniform(m_engine)], color);
        color = color == Color::Red ? Color::Yellow : Color::Red;
    }
    if (grid.hasFourConnected()) {
        return color == Color::Red ? Result::YellowWin : Result::RedWin;
    }
    return Result::Draw;
}

void AI::backUp(Node* node, Result result) const
{
    while (node != nullptr) {
        ++node->visits;
        // Reward is adjusted wrt parent node
        switch (result) {
        case Result::RedWin:
            node->reward += node->color == Color::Red ? -1 : 1;
            break;
        case Result::YellowWin:
            node->reward += node->color == Color::Red ? 1 : -1;
            break;
        case Result::Draw:
            break;
        }
        node = node->parent;
    }
}

Node* AI::treePolicy(Node* node) const
{
    // When all children have been explored at least once, UTC can be used
    // to traverse the tree
    float bestValue = std::numeric_limits<float>::lowest();
    Node* bestChild = nullptr;
    for (auto& child : node->children) {
        float value = uctValue(child);
        if (value > bestValue) {
            bestValue = value;
            bestChild = &child;
        }
    }
    return bestChild;
}

float AI::uctValue(const Node& node) const
{
    // Compute UCT value of node
    float exploit = node.reward / node.visits;
    float explore
        = std::sqrt(2 * std::log(node.parent->visits) / node.visits);
    return exploit + explore;
}

int AI::bestAction(const Node& node) const
{
    // Return child with most visits
    int bestAction = -1;
    int mostVisits = -1;
    for (auto& child : node.children) {
        if (child.visits > mostVisits) {
            mostVisits = child.visits;
            bestAction = child.action;
        }
    }
    return bestAction;
}
