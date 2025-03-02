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
   Move move;
   int totalMoveCount;


   static constexpr int BLACK = 1;
   static constexpr int WHITE = 2;

   MCTSNode* parent;
   std::vector<MCTSNode*> children;

   MCTSNode(int player, Move move = Move(), MCTSNode* parent = nullptr);

   double UCTValue(double c = 1.414) const;
   MCTSNode* SelectChild() const;
   MCTSNode* SelectChild(const Move& move) const;
   MCTSNode* SelectBestChild() const;
   Move ExpandNode(Board& board, const vector<vector<Move>>& possibleMoves);
   bool isFullyExpanded(Board& board);

   static void DeleteTree(MCTSNode* root);
   static Board CopyBoard(const Board& board);

   static void showTree(MCTSNode* root, int level);
};

#endif
