#ifndef STUDENTAI_H
#define STUDENTAI_H
#include "AI.h"
#include "Board.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
    int player;

    StudentAI(int col, int row, int p);
    virtual Move GetMove(Move board);
    const int MCTS_ITERATIONS = 1000;

private:
    Move RunMCTS();
    int Rollout(Board& board, int turn);
};

#endif //STUDENTAI_H
