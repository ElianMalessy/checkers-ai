#include "MCTSNode.h"
#include "Checker.h"

#include <cmath>
#include <functional>
#include <iomanip>


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
    MCTSNode* bestChild = nullptr;

    double bestValue;
    std::function<double(double, double)> keepBestScore;
    bestValue = -std::numeric_limits<double>::infinity();

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
                material -= 1;
            } else if (board[i][j].color == "W" && board[i][j].isKing) {
                material -= 3;
            } else if (board[i][j].color == "B" && !board[i][j].isKing) {
                material += 1;
            } else if (board[i][j].color == "B" && board[i][j].isKing) {
                material += 3;
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
    keepBestScore = [](int a, int b) { return std::max(a, b); };
    bestScore = -std::numeric_limits<double>::infinity();
    // if(player == BLACK) {
    //     keepBestScore = [](int a, int b) { return std::max(a, b); };
    //     bestScore = -std::numeric_limits<double>::infinity();
    // }
    // else if(player == WHITE) {
    //     keepBestScore = [](int a, int b) { return std::min(a, b); };
    //     bestScore = std::numeric_limits<double>::infinity();
    // }

    const int newPlayer = player == BLACK ? WHITE : BLACK;

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



void MCTSNode::showTree(MCTSNode* root, int level) {
    MCTSNode* current = root;
    if(current->visits > 0) {
        cout << std::setw(level) << "" << " Wins: " << current->wins << " Player: " << current->player << endl;
    }

    for(auto child : current->children) {
        if(level < 2) {
            showTree(child, level + 1);
        }
    }
}

