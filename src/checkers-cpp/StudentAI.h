#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#include "MCTSNode.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
    int player;
    // MCTSNode *root;

    StudentAI(int col, int row, int p);
    ~StudentAI();
    virtual Move GetMove(Move board);
    static constexpr int MCTS_ITERATIONS = 100;
    static constexpr int BLACK = 1;
    static constexpr int WHITE = 2;

private:
    Move RunMCTS();
    int Rollout(Board& board, int turn);
    void backpropagate(MCTSNode* node, int result);
};

#endif //STUDENTAI_H
