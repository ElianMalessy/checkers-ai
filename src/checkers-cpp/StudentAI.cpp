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

<<<<<<< Updated upstream
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

        // Simulation
        Board simBoard = node->board;
        int simPlayer = node->turn;
        int result = Simulate(simBoard, simPlayer);

        // Backpropagation
        while (node != nullptr) {
            node->visits++;
            if ((result == 1 && turn == 1) || (result == 2 && turn == 2)) {
                node->wins++;
            }
            node = node->parent;
        }
||||||| constructed merge base
Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
=======
Move StudentAI::GetMove(Move move) {
    if (move.seq.empty()) {
        turn = 1;
    } else {
        board.makeMove(move, turn == 1 ? 2 : 1);
>>>>>>> Stashed changes
    }

<<<<<<< Updated upstream
    // Select best move based on most visits
    MCTSNode* bestChild = nullptr;
    int mostVisits = -1;

    for (const auto& child : root.children) {
        if (child->visits > mostVisits) {
            mostVisits = child->visits;
            bestChild = child.get();
        }
    }
||||||| constructed merge base
=======
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
>>>>>>> Stashed changes

<<<<<<< Updated upstream
    return bestChild ? bestChild->move : root.untriedMoves[0];
||||||| constructed merge base
=======
        // Expansion
        if (!node->IsTerminal() && !node->untriedMoves.empty()) {
            Move move = node->BestMove();
            node = node->AddChild(move);
        }

        // Simulation
        Board simBoard = node->board;
        int simPlayer = node->turn;
        int result = Simulate(simBoard, simPlayer);

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
>>>>>>> Stashed changes
}

int StudentAI::Simulate(Board& board, int currentPlayer) {
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
