from pathlib import Path

wins = 0
ties = 0
losses = 0

for file in Path('./output').glob('*.txt'):
    with open(file) as f:
        result = f.readlines()[-1][:-1]
        if result == 'player 1 wins':
            wins += 1
        elif result == 'Tie':
            ties += 1
        elif result == 'player 1 wins':
            losses += 1

print(f'Wins  : {wins} ({100*wins/(wins+ties+losses):.02f}%)')
print(f'Ties  : {ties} ({100*ties/(wins+ties+losses):.02f}%)')
print(f'Losses: {losses} ({100*losses/(wins+ties+losses):.02f}%)')