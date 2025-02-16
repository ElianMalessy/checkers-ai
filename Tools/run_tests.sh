#!/bin/bash

for i in {1..20}
do
    echo "Starting test $i"
    python3 AI_Runner.py 8 8 3 l ../src/checkers-cpp/main Sample_AIs/Random_AI/main.py > output/$i.txt
    echo "Finished test $i"
done