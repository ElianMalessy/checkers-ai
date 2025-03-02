#include "StudentAI.h"

#include <iostream>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
    // root = new MCTSNode(player);
}

StudentAI::~StudentAI() {
    // MCTSNode::DeleteTree(root);
}

Move StudentAI::GetMove(Move move) {
    if (move.seq.empty()) {
        player = 1;
        // root->player = 1;
    } else {
        board.makeMove(move, player == 1 ? 2 : 1);
        // if(root->visits > 0) {
        //     MCTSNode* node = root->SelectChild(move);
        //     delete root;
        //     root = node;
        // }
    }

    Move bestMove = RunMCTS();
    board.makeMove(bestMove, player);
    return bestMove;
}


Move StudentAI::RunMCTS() {
    std::srand(std::time(0));
    MCTSNode* root = new MCTSNode(player);

    // Run MCTS for a fixed number of iterations
    for (int i = 0; i < MCTS_ITERATIONS; i++) {
        // Selection
        MCTSNode* node = root;
        Board newBoard = MCTSNode::CopyBoard(board);
        while (node->isFullyExpanded(newBoard)) {
            const int prevPlayer = node->player;
            auto next = node->SelectChild();
            if(next == nullptr) {
                int result = newBoard.isWin(prevPlayer);
                backpropagate(node, result);
                break;
            }

            node = next;
            newBoard.makeMove(node->move, prevPlayer);
        }

        // Expansion
        auto possibleMoves = newBoard.getAllPossibleMoves(node->player);
        int winner = 0;
        if (possibleMoves.empty()) {
            winner = newBoard.isWin(node->player == 1 ? 2 : 1);
        }
        else {
            Move bestMove = node->ExpandNode(newBoard, possibleMoves);
            newBoard.makeMove(bestMove, node->player);

            // Rollout
            winner = Rollout(newBoard, node->player);
        }

        // Backpropagation
        backpropagate(node, winner);
    }

    MCTSNode* bestChild = root->SelectBestChild();
    Move bestMove = bestChild->move;

    // root = new MCTSNode(player);
    // for(auto child : root->children) {
    //     if(child != bestChild) {
    //         MCTSNode::DeleteTree(child);
    //     }
    // }
    // delete root;
    // root = bestChild;
    MCTSNode::DeleteTree(root);

    return bestMove;
}

int StudentAI::Rollout(Board& board, int prevPlayer) {
    while (true) {
        int winner = board.isWin(prevPlayer);
        if (winner != 0) {
            return winner;
        }

        prevPlayer = prevPlayer == 1 ? 2 : 1;
        auto moves = board.getAllPossibleMoves(prevPlayer);

        // Random move selection
        int i = rand() % moves.size();
        int j = rand() % moves[i].size();
        board.makeMove(moves[i][j], prevPlayer);

    }
}

// taking forever
void StudentAI::backpropagate(MCTSNode* node, int winner) {
    while (node != nullptr) {
        node->visits++;
        if (winner == node->player || winner == -1) {
            node->wins++;
        }
        node = node->parent;
    }
}
