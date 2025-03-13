#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

#include <memory>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>

#include "Board.h"
#include "Move.h"
#include "Checker.h"

class MCTSNode {
public:
   int player;
   int visits;
   int wins;
   Move move;
    Board board;
    int  numPossibleMovesLeft;


   static constexpr int BLACK = 1;
   static constexpr int WHITE = 2;

   MCTSNode* parent;
   std::vector<MCTSNode*> children;

   MCTSNode(int player, Board board, Move move = Move(), MCTSNode* parent = nullptr);

   double UCTValue(double c = 1.414) const;
   MCTSNode* SelectChild() const;
   MCTSNode* SelectChild(const Move& move) const;
   MCTSNode* SelectBestChild() const;
   Board ExpandNode() ;
   inline bool isFullyExpanded();
		void createChildren();

   static void DeleteTree(MCTSNode* root);
   static Board CopyBoard(const Board& board);

   static void showTree(MCTSNode* root, int level);
};



//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
    int player;
    MCTSNode *root;

    StudentAI(int col, int row, int p);
    ~StudentAI();
    virtual Move GetMove(Move board);
    static constexpr int MCTS_ITERATIONS = 1000;
    static constexpr int BLACK = 1;
    static constexpr int WHITE = 2;

private:
    Move RunMCTS();
    int Rollout(Board& board, int turn) const;
    void backpropagate(MCTSNode* node, int result);
};

#endif //STUDENTAI_H
