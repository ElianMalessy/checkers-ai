from random import randint
from BoardClasses import Move
from BoardClasses import Board
from Checker import Checker
import math

def copyBoard(oldboard):
        newBoard = Board(oldboard.col, oldboard.row, oldboard.p)
        newBoard.board = [[0 for _ in newBoard.col] for _ in newBoard.row]
        for i in range(len(newBoard.board)):
            for j in range(len(newBoard.board[0])):
                newBoard.board[i][j] = Checker(oldboard.board[i][j].color, i, j)
                newBoard.board[i][j].is_king = oldboard.board[i][j].is_king
        newBoard.col = oldboard.col
        newBoard.row = oldboard.row
        newBoard.p = oldboard.p
        newBoard.black_count = oldboard.black_count
        newBoard.tie_counter = oldboard.tie_counter
        return newBoard

class MCTSNode:

    def __init__(self, player, move=0, parent=None):
        self.player = player
        self.move = move
        self.parent = parent
        self.children = []

        self.visits = 0
        self.wins = 0

    def UCTValue(self, constant):
        if not visits: return float('inf')
        return (wins/visits) + c * math.sqrt(math.log(self.parent.visits)/visits)
    
    def SelectChild(self):
        bestValue = float('-inf')
        bestChild = None

        for child in children:
            uct = child.UCTValue()
            if (uct > bestValue):
                bestValue = uct
                bestChild = child
        return bestChild
    
    def evaluateMaterial(self, board):
        material = 0
        for i in range(len(board)):
            for j in range(len(board[0])):
                piece = board[i][j]
                if piece.is_king:
                    if piece.color == 'W':
                        material += 3
                    elif piece.color == 'B':
                        material -= 3
                else:
                    if piece.color == 'W':
                        material += 1
                    elif piece.color == 'B':
                        material -= 1
        return material
    
    def evaluateBoard(self, board):
        return float(evaluateMaterial(board))
    
    

    def bestMove(self, board, possible_moves):
        bestMove = Move(possible_moves)
        bestScore = 0

        def keepBestScore(a, b):
            return max(a,b)

        bestScore = float('-inf')
        
        newPlayer = 2 if player == 1 else 1

        for move_set in possible_moves:
            for m in move_set:
                # Try the move
                newBoard = copyBoard(board)
                newBoard.make_move(m, player)

                newNode = MCTSNode(newPlayer, m, self)
                children.append(newNode)

                # Apply heuristic
                score = evaluateBoard(newBoard.board)
                newBestScore = keepBestScore(bestScore, score)

                if newBestScore != bestScore:
                    bestScore = newBestScore
                    bestMove = m
        
        return bestMove



#The following part should be completed by students.
#Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI():

    def __init__(self,col,row,p):
        self.col = col
        self.row = row
        self.p = p
        self.board = Board(col,row,p)
        self.board.initialize_game()
        self.color = ''
        self.opponent = {1:2,2:1}
        self.color = 2

        self.player = 2
        self.MCTS_ITERATIONS = 1000
    
    def get_move(self,move):
        if len(move) != 0:
            self.board.make_move(move,self.opponent[self.color])
        else:
            self.color = 1
        moves = self.board.get_all_possible_moves(self.color)
        index = randint(0,len(moves)-1)
        inner_index =  randint(0,len(moves[index])-1)
        move = moves[index][inner_index]
        self.board.make_move(move,self.color)
        return move



    def RunMCTS(self):
        root = MCTSNode

        for i in range(MCTS_ITERATIONS):

            # Select
            node = root
            newBoard = copyBoard(self.board)

            while (node.visits > 0):
                prevPlayer = node.player
                next_node = node.SelectChild()
                if not next_node:
                    result = newBoard.is_win(prevPlayer)
                    self.backpropagate(node, result)
                    break
                
                node = next_node
                newBoard.make_move(node.move, prevPlayer)

            # Expand
            possible_moves = newBoard.get_all_possible_moves(node.player)
            winner = 0
            if len(possible_moves) == 0:
                winner = newBoard.is_win(2 if node.player == 1 else 1)
            else:
                bestMove = node.bestMove(newBoard, possible_moves)
                newBoard.make_move(bestMove, node.player)

                winner = self.rollout(newBoard, node.player)

            # Backpropagate
            self.backpropagate(node, winner)
    
    def rollout(self, board, prevPlayer):
        while True:
            winner = board.is_win(prevPlayer)
            if winner:
                return winner

            prevPlayer = 2 if prevPlayer == 1 else 1
            moves = board.get_all_possible_moves(prevPlayer)
            
            
            i = randint(0, len(moves)-1)
            j = randint(0, len(moves[i])-1)
            board.make_move(moves[i][j], prevPlayer)


    def backpropagate(self, node, result):
        while node:
            node.visits += 1
            if winner == node.player or winner == -1:
                node.wins += 1
            node = node.parent
