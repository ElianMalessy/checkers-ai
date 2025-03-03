#!/bin/bash

cd output
rm "*.txt"
cd ../

for i in {1..10}
do
    echo "Starting test $i"
    # python3 AI_Runner.py 8 8 3 l ../src/checkers-python/main.py Sample_AIs/Poor_AI/main.py > output/$i.txt
    python3 AI_Runner.py 8 8 3 l ../src/checkers-cpp/main Sample_AIs/Poor_AI/main.py > output/$i.txt
    echo "Finished test $i"
done

python3 parse_tests.py