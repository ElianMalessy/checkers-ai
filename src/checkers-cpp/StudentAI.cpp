#include "StudentAI.h"


//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = WHITE;
    root = new MCTSNode(player, board);
}

StudentAI::~StudentAI() {
    MCTSNode::DeleteTree(root);
}

Move StudentAI::GetMove(Move move) {
    if (move.seq.empty()) {
        player = BLACK;
        delete root;
        root = new MCTSNode(player, board);

    } else {
        board.makeMove(move, player == WHITE ? BLACK : WHITE);
        
        MCTSNode* node = root->SelectChild(move);

        if(node == nullptr) {
            node = new MCTSNode(player, board, move, nullptr);
            MCTSNode::DeleteTree(root);
        }
        else {
            for(auto child : root->children) {
                if(child != node) {
                    MCTSNode::DeleteTree(child);
                }
            }
            root->children.clear();

            delete root;
        }

        root = node;
        node->parent = nullptr;
    }

    Move bestMove = RunMCTS();
    board.makeMove(bestMove, player);
    return bestMove;
}


Move StudentAI::RunMCTS() {
    std::srand(std::time(0));

    // Run MCTS for a fixed number of iterations
    for (int i = 0; i < MCTS_ITERATIONS; i++) {
        // Selection
        MCTSNode* node = root;
        while (node->isFullyExpanded()) {
            auto next = node->SelectChild();
            if(next == nullptr) {
                // Found a terminal state
                int result = node->board.isWin(node->player);
                backpropagate(node, result);
                break;
            }

            node = next;
        }

        // Expansion
        const int newPlayer = node->player == BLACK ? WHITE : BLACK;
        int winner = node->board.isWin(newPlayer);
        if (winner == 0) {
            Board newBoard = node->ExpandNode();
            winner = Rollout(newBoard, node->player);
        }

        // Backpropagation
        backpropagate(node, winner);
    }

    MCTSNode* bestChild = root->SelectBestChild();

    for(auto child : root->children) {
        if(child != bestChild) {
            MCTSNode::DeleteTree(child);
        }
    }
    delete root;
    root = bestChild;
    bestChild->parent = nullptr;
    Move bestMove = bestChild->move;

    return bestMove;
}

int StudentAI::Rollout(Board& board, int prevPlayer) const {
    // maybe do early stopping after a certain number of moves its just whoever has more material or a tie
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
        // if (winner == player || winner == -1) {
        if (winner == player) {
            node->wins++;
        }
        else if (winner == -1) {
            node->wins += 0.5;
        }

        node = node->parent;
    }
}




MCTSNode::MCTSNode(int player, Board board, Move move, MCTSNode* parent)
    : player(player), move(move), board(board), parent(parent), visits(0), wins(0) {
    numPossibleMovesLeft = 0;
    for(const auto& moves : board.getAllPossibleMoves(player)) {
        for(const auto& m : moves) {
            numPossibleMovesLeft++;
        }
    }
}

void MCTSNode::DeleteTree(MCTSNode* root) {
    if(root == nullptr) {
        return;
    }

    for (auto child : root->children) {
        DeleteTree(child);
    }
    delete root;
}

double MCTSNode::UCTValue(double c) const {
    if (visits == 0) return std::numeric_limits<double>::infinity();
    return (static_cast<double>(wins) / visits) + c * std::sqrt(std::log(parent->visits) / visits);
}

MCTSNode* MCTSNode::SelectChild() const {
    MCTSNode* bestChild = nullptr;
    double bestValue;
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

MCTSNode* MCTSNode::SelectChild(const Move& move) const {
    for (const auto& child : children) {
        if(child->move.seq.size() != move.seq.size()) {
            continue;
        }

        bool same = true;
        for(int i = 0; i < move.seq.size(); i++) {
            if(child->move.seq[i].x != move.seq[i].x || child->move.seq[i].y != move.seq[i].y) {
                same = false;
            }
        }

        if(same) {
            return child;
        }
    }

    return nullptr;
}

MCTSNode* MCTSNode::SelectBestChild() const {
    MCTSNode* bestChild = nullptr;
    int bestValue = 0;
    for (const auto& child : children) {
        if (child->visits > bestValue) {
            bestValue = child->visits;
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

int evaluateCenterControl(vector<vector<Checker>>& board) {
    int eval = 0;
    int BOARD_SIZE = board.size();

    if (BOARD_SIZE < 5) {
        return eval;
    }

    for (int i = BOARD_SIZE/2 - 1; i < BOARD_SIZE/2 + 1 + (BOARD_SIZE % 2); ++i) {
        for (int j = BOARD_SIZE/2 - 1; j < BOARD_SIZE/2 + 1 + (BOARD_SIZE % 2); ++j) {
            if (board[i][j].color == "W" && !board[i][j].isKing) {
                eval -= 1;
            } else if (board[i][j].color == "W" && board[i][j].isKing) {
                eval -= 3;
            } else if (board[i][j].color == "B" && !board[i][j].isKing) {
                eval += 1;
            } else if (board[i][j].color == "B" && board[i][j].isKing) {
                eval += 3;
            }
        }
    }

    return eval;
}

double evaluateProgression(vector<vector<Checker>>& board) {

    double avgW = 0;
    double avgB = 0;

    int numW = 0;
    int numB = 0;

    int eval = 0;
    int BOARD_SIZE = board.size();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].color == "W" && !board[i][j].isKing) {
                avgW += BOARD_SIZE-1-i;
                numW += 1;
            } else if (board[i][j].color == "B" && !board[i][j].isKing) {
                avgB += i;
                numB += 1;
            }
        }
    }

    if (numW == 0 || numB == 0) {
        return eval;
    }

    avgW /= static_cast<double>(numW);
    avgB /= static_cast<double>(numB);

    return avgB - avgW;
}

// Heuristic function to evaluate the current board state
double evaluateBoard(vector<vector<Checker>>& board) {
    double score = 0;

    score += static_cast<double>(evaluateMaterial(board));
    score += static_cast<double>(evaluateCenterControl(board));
    score += 7*evaluateProgression(board);

    return score;
}

void MCTSNode::createChildren() {
    for(auto& moves : board.getAllPossibleMoves(player)) {
        for(auto& m : moves) {
            Board newBoard = CopyBoard(board);
            newBoard.makeMove(m, player);

            MCTSNode *child = new MCTSNode(player == BLACK ? WHITE : BLACK, newBoard, m, this);
            children.push_back(child);
        }
    }
}

Board MCTSNode::ExpandNode() {
    if(children.empty()) {
        createChildren();
    }


    double bestScore;
    const int newPlayer = player == BLACK ? WHITE : BLACK;

    std::function<double(double, double)> keepBestScore;
    if(player == BLACK) {
        keepBestScore = [](double a, double b) { return std::max(a, b); };
        bestScore = -std::numeric_limits<double>::infinity();
    }
    else if(player == WHITE) {
        keepBestScore = [](double a, double b) { return std::min(a, b); };
        bestScore = std::numeric_limits<double>::infinity();
    }

    int bestMoveIndex = 0;
    int i = 0;
    for (; i < children.size(); i++) {
        MCTSNode* child = children[i];
        if(child->visits > 0) {
            continue;
        }

        // some heuristic
        double score = evaluateBoard(child->board.board);
        double newBestScore = keepBestScore(bestScore, score);

        if(newBestScore != bestScore) {
            bestScore = newBestScore;
            bestMoveIndex = i;
        }
    }

    Board newBoard = children[bestMoveIndex]->board;
    numPossibleMovesLeft--;
    return newBoard;
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


inline bool MCTSNode::isFullyExpanded() {
    return numPossibleMovesLeft == 0;
}



// void MCTSNode::showTree(MCTSNode* root, int level) {
//     MCTSNode* current = root;
//     if(current->visits > 0) {
//         cout << std::setw(level) << "" << " Wins: " << current->wins << " Player: " << current->player << " Move: " << current->move.toString() << endl;
//     }
//
//     for(auto child : current->children) {
//         if(level < 2) {
//             showTree(child, level + 1);
//         }
//     }
// }

