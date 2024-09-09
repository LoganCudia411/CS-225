/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include <ctime>
#include <random> 


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the currentent player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens)
    : g_(true, true) {
    token = startingTokens;
    startingVertex_ = "p1-" + to_string(token);

    for (int i = startingTokens; i >= 0; i--) {
       string part2 = "p2-" + to_string(i);
        string part1 = "p1-" + to_string(i);
        g_.insertVertex(part1);
        g_.insertVertex(part2);

        int num_toke = startingTokens - 1;

        auto sum1 = "p1-" + to_string(i + 1);
        auto sum2 = "p1-" + to_string(i + 2);
        auto sum3 = "p2-" + to_string(i + 1);
        auto sum4 = "p2-" + to_string(i + 2);

        if (i <= num_toke) {
            g_.insertEdge(sum1, part2);
            g_.setEdgeWeight(sum1, part2, 0);
             g_.insertEdge(sum3, part1);
            g_.setEdgeWeight(sum3, part1, 0);
        }

         g_.insertEdge(sum2, part2);
        g_.setEdgeWeight(sum2, part2, 0);
        g_.insertEdge(sum4, part1);
        g_.setEdgeWeight(sum4, part1, 0);
    }
}
    


/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
    std::vector<Edge> path;

    std::random_device rando;
    std::mt19937 gen(rando()); 

    for (auto current = "p1-" + std::to_string(token); current != "p1-0" && current != "p2-0"; ) {
        auto v = g_.getAdjacent(current);
        std::uniform_int_distribution<size_t> distrib(0, v.size() - 1);

        auto tmp = v[distrib(gen)];
        path.push_back(g_.getEdge(current, tmp));
        current =tmp;
    }

    return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
    /* Your code goes here! */
    for (const auto& val : path) { 
        auto weights = g_.getEdgeWeight(val.source, val.dest);
        if (val.source.substr(0, 2) == (path[path.size() - 1]).source.substr(0, 2)) {
            weights++;
        } else {
            weights--;
        }
        g_.setEdgeWeight(val.source, val.dest, weights);
    }
}


/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
    std::vector<Vertex> vertices = g_.getVertices();
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vertex &v = vertices[i];
        
        std::vector<Vertex> adjacentVertices = g_.getAdjacent(v);
        for (size_t j = 0; j < adjacentVertices.size(); ++j) {
            const Vertex &w = adjacentVertices[j];
            int weight = g_.getEdgeWeight(v, w);
            if (weight > threshold) {
                g_.setEdgeLabel(v, w, "WIN");
            } else if (weight < -1 * threshold) {
                g_.setEdgeLabel(v, w, "LOSE");
            }
        }
    }
}


/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
