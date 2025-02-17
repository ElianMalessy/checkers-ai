#include "MCTSNode.h"
#include "Checker.h"

#include <cmath>
#include <functional>


MCTSNode::MCTSNode(int player, Move move, MCTSNode* parent)
    : player(player), move(move), parent(parent), visits(0), wins(0) {

}

void MCTSNode::DeleteTree(MCTSNode* root) {
    for (auto child : root->children) {
        DeleteTree(child);
    }
}

double MCTSNode::UCTValue(double c) const {
    if (visits == 0) return std::numeric_limits<double>::infinity();
    return (static_cast<double>(wins) / visits) + c * std::sqrt(std::log(parent->visits) / visits);
}

MCTSNode* MCTSNode::SelectChild() const {
    double bestValue = -std::numeric_limits<double>::infinity();
    MCTSNode* bestChild = nullptr;

    for (const auto& child : children) {
        double uctValue = child->UCTValue();
        if (uctValue > bestValue) {
            bestValue = uctValue;
            bestChild = child;
        }
    }
    return bestChild;
}

int evaluateMaterial(vector<vector<Checker>>& board) {
    int material = 0;
    int BOARD_SIZE = board.size();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].color == "W" && !board[i][j].isKing) {
                material += 1;
            } else if (board[i][j].color == "W" && board[i][j].isKing) {
                material += 3;
            } else if (board[i][j].color == "B" && !board[i][j].isKing) {
                material -= 1;
            } else if (board[i][j].color == "B" && board[i][j].isKing) {
                material -= 3;
            }
        }
    }
    return material;
}


// Heuristic function to evaluate the current board state
double evaluateBoard(vector<vector<Checker>>& board) {
    double score = 0;

    score += static_cast<double>(evaluateMaterial(board));

    return score;
}


Move MCTSNode::BestMove(Board& board, const vector<vector<Move>>& possibleMoves) {
    const Move *bestMove;
    double bestScore;

    std::function<double(double, double)> keepBestScore;
    if(player == 1) {
        keepBestScore = [](int a, int b) { return std::max(a, b); };
        bestScore = -std::numeric_limits<double>::infinity();
    }
    else if(player == 2) {
        keepBestScore = [](int a, int b) { return std::min(a, b); };
        bestScore = std::numeric_limits<double>::infinity();
    }

    const int newPlayer = player == 1 ? 2 : 1;

    for (const auto& moveSet : possibleMoves) {
        for (const auto& m : moveSet) {
            Board newBoard = CopyBoard(board);
            newBoard.makeMove(m, player);

            MCTSNode *newNode = new MCTSNode(newPlayer, m, this);
            children.push_back(newNode);

            // some heuristic
            double score = evaluateBoard(newBoard.board);
            double newBestScore = keepBestScore(bestScore, score);

            if(newBestScore != bestScore) {
                bestScore = newBestScore;
                bestMove = &m;
            }

        }
    }

    return *bestMove;
}


Board MCTSNode::CopyBoard(const Board& board) {
    Board newBoard;
    vector<vector<Checker>> boardVec;
    for(int i = 0; i < board.board.size(); i++) {
        vector<Checker> row;
        for(int j = 0; j < board.board[i].size(); j++) {
            auto checker = Checker(board.board[i][j].color, i, j);
            checker.isKing = board.board[i][j].isKing;
            row.push_back(checker);
        }
        boardVec.push_back(row);
    }
    newBoard.col = board.col;
    newBoard.row = board.row;
    newBoard.p = board.p;
    newBoard.blackCount = board.blackCount;
    newBoard.whiteCount = board.whiteCount;
    newBoard.tieCount = board.tieCount;
    newBoard.tieMax = board.tieMax;
    newBoard.board = boardVec;

    return newBoard;
}


