#include "StudentAI.h"
#include "MCTSNode.h"

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

Move StudentAI::GetMove(Move move) {
    if (move.seq.empty()) {
        player = 1;
    } else {
        board.makeMove(move, player == 1 ? 2 : 1);
    }

    Move bestMove = RunMCTS();
    board.makeMove(bestMove, player);
    return bestMove;
}


Move StudentAI::RunMCTS() {
    MCTSNode root(player);

    // Run MCTS for a fixed number of iterations
    for (int i = 0; i < MCTS_ITERATIONS; i++) {
        // Selection
        MCTSNode* node = &root;
        while (node->visits > 0) {
            node = node->SelectChild();
        }

        // Expansion
        Board newBoard = node->BestMove(board);

        // Rollout
        int simPlayer = node->player;
        int result = Rollout(newBoard, simPlayer);

        // Backpropagation
        while (node != nullptr) {
            node->visits++;
            if ((result == 1 && player == 1) || (result == 2 && player == 2)) {
                node->wins++;
            }
            node = node->parent;
        }
    }

    MCTSNode* bestChild = root.SelectChild();
    return bestChild->move;
}

int StudentAI::Rollout(Board& board, int currentPlayer) {
    while (true) {
        int winner = board.isWin(currentPlayer);
        if (winner > 0) {
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
