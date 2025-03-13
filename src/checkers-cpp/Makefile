CXX = g++
# CXXFLAGS = -std=c++20 -O3 -g -fsanitize=address
CXXFLAGS = -std=c++20 -O3
OBJ = Utils.o Checker.o main.o Board.o ManualAI.o Move.o GameLogic.o
TARGET = main

# Default target: only recompiles StudentAI.cpp
all: $(TARGET)

$(TARGET): $(OBJ) StudentAI.o
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET)

# Compile other files once
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for StudentAI.cpp (recompile frequently)
StudentAI.o: StudentAI.cpp StudentAI.h
	$(CXX) $(CXXFLAGS) -c StudentAI.cpp -o StudentAI.o

# Full recompile target
full:
	$(CXX) $(CXXFLAGS) Utils.cpp Checker.cpp main.cpp Board.cpp StudentAI.cpp ManualAI.cpp Move.cpp GameLogic.cpp -o $(TARGET)

# Clean compiled files
clean:
	rm -f $(OBJ) StudentAI.o $(TARGET)
