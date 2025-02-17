#ifndef MCTSNODE_H
#define MCTSNODE_H

#include <memory>
#include <vector>

#include "Board.h"
#include "Move.h"

class MCTSNode {
public:
   int player;
   int visits;
   int wins;
   double value;
   Move move;

   MCTSNode* parent;
   std::vector<MCTSNode*> children;

   MCTSNode(int player, Move move = Move(), MCTSNode* parent = nullptr);

   double UCTValue(double c = 1.414) const;
   MCTSNode* SelectChild() const;

   Move BestMove(Board& board, const vector<vector<Move>>& possibleMoves);

   static void DeleteTree(MCTSNode* root);
   static Board CopyBoard(const Board& board);
};

#endif
