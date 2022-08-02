#ifndef PUZZLE15_H
#define PUZZLE15_H

#define GAME_SIZE 3

#include <set>

struct State {
    int nums[GAME_SIZE * GAME_SIZE];
    int emptyIndex;
};

typedef enum {
    RIGHT, LEFT, UP, DOWN
} Dir;

struct StateAndScore {
    State* state;
    Dir dir;
    int score;    
};

// -- utility functions --

void generateRandomState(State* state);
int solvedCount(State* state);
State* nextState(State* state, Dir dir);
void possibleDirs(State* state, Dir* dirs, int* n);
int isEndState(State* state);

// -- operators --
int operator<(const State& s1, const State& s2);
int operator<(const StateAndScore& s1, const StateAndScore& s2);

// -- print methods --
void printState(const State* state);
void printDir(Dir dir);
void printAllStates(std::set<State>& states);

// hueristic functions
int h(State* state);
int hV2(State* state, int n);

// search methods
int search(State* state, std::set<State>& searchedStates, std::vector<Dir>& solution, int depthLimit = -1, int depth = 0);
int iterativeDeepeningSearch(State* state, int depthLimit = 200);
int searchV2(State* state, std::set<State>& searchedStates, std::vector<Dir>& solution, int highestSolved);

#endif