#ifndef AI_H
#define AI_H

#include "game.h"
#include <memory>
#include <random>
#include <vector>

// A playout can have 3 different results
enum class Result { YellowWin, RedWin, Draw };

// A node in the game tree
struct Node {
    Node(Node* parent, int action, Color color);
    Node* parent;
    std::vector<Node> children;
    int action;
    bool terminal;
    Color color;
    int visits;
    float reward;
};

// An artificial intelligence that uses Monte-Carlo Tree-Search
class AI {
public:
    AI(int iterations);

    // Get an action from the AI given the grid and the move color
    int getAction(Grid& grid, Color color);

private:
    // One iteration of Monte-Carlo Tree Search
    void monteCarloTreeSearch(Node* root, Grid grid);

    // Selection: Find a promising leaf of the explored game tree
    Node* select(Node* node, Grid& grid) const;

    // Expansion: If a node is not fully explored, generate one of the missing
    // children
    Node* expand(Node* node, Grid& grid) const;

    // Simulation: From the leaf of the game tree, perform a random playout
    // where moves are picked uniformly at random
    Result simulate(Grid& grid, Color color);

    // Back up: Propagate result back up through thre tree
    void backUp(Node* node, Result result) const;

    // Choose a child of the node according to the tree policy
    Node* treePolicy(Node* node) const;

    // Compute the Upper Confidence Bound for the node
    float uctValue(const Node& node) const;

    // Get the action that leads to the child node with the most visits
    int bestAction(const Node& node) const;

    int m_iterations;
    std::random_device m_randomDevice;
    std::mt19937 m_engine;
};

#endif