#include "StudentAI.h"
#include "MCTSNode.h"

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    turn = 2;
}

Move StudentAI::GetMove(Move move) {
    if (move.seq.empty()) {
        turn = 1;
    } else {
        board.makeMove(move, turn == 1 ? 2 : 1);
    }

    Move bestMove = RunMCTS();
    board.makeMove(bestMove, turn);
    return bestMove;
}


Move StudentAI::RunMCTS() {
    MCTSNode root(board, turn);

    // Run MCTS for a fixed number of iterations
    for (int i = 0; i < MCTS_ITERATIONS; i++) {
        // Selection
        MCTSNode* node = &root;
        while (!node->IsTerminal() && node->IsFullyExpanded()) {
            node = node->SelectChild();
        }

        // Expansion
        if (!node->IsTerminal() && !node->untriedMoves.empty()) {
            Move move = node->BestMove();
            node = node->AddChild(move);
        }

        // Rollout
        Board simBoard = node->board;
        int simPlayer = node->turn;
        int result = Rollout(simBoard, simPlayer);

        // Backpropagation
        while (node != nullptr) {
            node->visits++;
            if ((result == 1 && turn == 1) || (result == 2 && turn == 2)) {
                node->wins++;
            }
            node = node->parent;
        }
    }

    MCTSNode* bestChild = root.SelectChild();
    return bestChild ? bestChild->move : root.untriedMoves[0];
}

int StudentAI::Rollout(Board& board, int currentPlayer) {
    while (true) {
        int winner = board.isWin(currentPlayer);
        if (winner != 0) {
            return winner;
        }

        auto moves = board.getAllPossibleMoves(currentPlayer);
        if (moves.empty()) {
            return currentPlayer == 1 ? 2 : 1;
        }

        // Random move selection
        int i = rand() % moves.size();
        int j = rand() % moves[i].size();
        board.makeMove(moves[i][j], currentPlayer);

        currentPlayer = currentPlayer == 1 ? 2 : 1;
    }
}
