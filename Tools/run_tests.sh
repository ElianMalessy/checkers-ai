#!/bin/bash

mkdir output
for i in {1..100}
do
    touch output/$i.txt
    echo "Starting test $i"
    python3 AI_Runner.py 8 8 3 l ../src/checkers-python/main.py Sample_AIs/Poor_AI/main.py > output/$i.txt
    echo "Finished test $i"
done

python3 parse_tests.py
