#!/bin/bash

DIR="output"
[ -d "$DIR" ] && rm -rf "$DIR"
mkdir "$DIR"
for i in {1..20}
do
    touch output/$i.txt
    echo "Starting test $i"
    # python3 AI_Runner.py 8 8 3 l ../src/checkers-python/main.py Sample_AIs/Poor_AI/main.py > output/$i.txt
    python3 AI_Runner.py 8 8 3 l ../src/checkers-cpp/main Sample_AIs/Random_AI/main.py > output/$i.txt
    echo "Finished test $i"
done

python3 parse_tests.py
