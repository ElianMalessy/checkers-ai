#include "MCTSNode.h"

#include <algorithm>
#include <cmath>
#include <random>

MCTSNode::MCTSNode(Board board, int turn, Move move, MCTSNode* parent)
    : board(board), turn(turn), move(move), parent(parent), visits(0), wins(0) {
    // Get all possible moves for this state
    auto possibleMoves = board.getAllPossibleMoves(turn);
    for (const auto& moveSet : possibleMoves) {
        for (const auto& m : moveSet) {
            untriedMoves.push_back(m);
        }
    }

    // Shuffle untried moves for random selection
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(untriedMoves.begin(), untriedMoves.end(), g);
}

double MCTSNode::UCTValue(double c) const {
    if (visits == 0) return std::numeric_limits<double>::infinity();
    return (wins / visits) + c * std::sqrt(std::log(parent->visits) / visits);
}

MCTSNode* MCTSNode::SelectChild() const {
    double bestValue = -std::numeric_limits<double>::infinity();
    MCTSNode* bestChild = nullptr;

    for (const auto& child : children) {
        double uctValue = child->UCTValue();
        if (uctValue > bestValue) {
            bestValue = uctValue;
            bestChild = child.get();
        }
    }
    return bestChild;
}

// Move MCTSNode::BestMove() {
//     // some heuristic
//
//     Board newBoard = board;
//     newBoard.makeMove(move, turn);
//
//
// }

MCTSNode* MCTSNode::AddChild(const Move& move) {
    Board newBoard = board;
    newBoard.makeMove(move, turn);
    int nextPlayer = (turn == 1) ? 2 : 1;

    children.push_back(std::make_unique<MCTSNode>(newBoard, nextPlayer, move, this));

    // Remove the move from untried moves
    auto it = std::find(untriedMoves.begin(), untriedMoves.end(), move);
    if (it != untriedMoves.end()) {
        untriedMoves.erase(it);
    }

    return children.back().get();
}

bool MCTSNode::IsFullyExpanded() const {
    return untriedMoves.empty();
}

bool MCTSNode::IsTerminal() const {
    Board tempBoard = board;
    return tempBoard.isWin(turn) != 0;
}
