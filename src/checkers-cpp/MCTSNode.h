#ifndef MCTSNODE_H
#define MCTSNODE_H

#include <memory>
#include <vector>

#include "Board.h"
#include "Move.h"

class MCTSNode {
   public:
    Move move;
    Board board;
    int turn;
    int visits;
    double wins;
    MCTSNode* parent;
    std::vector<std::unique_ptr<MCTSNode>> children;
    std::vector<Move> untriedMoves;

    MCTSNode(Board board, int player, Move move = Move(), MCTSNode* parent = nullptr);

    double UCTValue(double c = 1.414) const;
    MCTSNode* SelectChild() const;
    Move BestMove();
    MCTSNode* AddChild(const Move& move);
    bool IsFullyExpanded() const;
    bool IsTerminal() const;
};

#endif
