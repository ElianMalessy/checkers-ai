import os
import subprocess
import shutil

DIR = "output"
if os.path.exists(DIR):
    shutil.rmtree(DIR)
os.makedirs(DIR)

wins = 0
ties = 0
losses = 0
for i in range(1, 21):
    print(f"Starting test {i}")
    file = f"{DIR}/{i}.txt"
    # subprocess.run(["python3", "AI_Runner.py", "8", "8", "3", "l", "../src/checkers-cpp/main", "Sample_AIs/Poor_AI/main.py"], stdout=open(file, "w"))
    # subprocess.run(["python3", "AI_Runner.py", "7", "7", "2", "l", "../src/checkers-cpp/main", "Sample_AIs/Average_AI/main.py"], stdout=open(file, "w"))
    subprocess.run(["python3", "AI_Runner.py", "7", "7", "2", "l", "../src/checkers-cpp/main2", "../src/checkers-cpp/main"], stdout=open(file, "w"))

    with open(file) as f:
        result = f.readlines()
        if len(result) == 0:
            continue
        result = result[-1][:-1]

        if result == 'player 1 wins':
            wins += 1
            print("Win")
        elif result == 'Tie':
            ties += 1
            print("Tie")
        elif result == 'player 2 wins':
            losses += 1
            print("Loss")
            
    print(f"Finished test {i}")


print(f'Wins    : {wins} ({100*wins/(wins+ties+losses):.02f}%)')
print(f'Ties    : {ties} ({100*ties/(wins+ties+losses):.02f}%)')
print('-----')
print(f'Win+Tie : {wins+ties} ({100*(wins+ties)/(wins+ties+losses):.02f}%)')
print(f'Losses  : {losses} ({100*losses/(wins+ties+losses):.02f}%)')
