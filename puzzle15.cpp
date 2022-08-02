#include <stdio.h>
#include <string.h>

#include <set>
#include <vector>
#include <algorithm>

#include "puzzle15.h"


void generateRandomState(State* state) {
    int randomMoveCount = 300;

    // starting state
    State generatedState;
    for (size_t i = 0; i < GAME_SIZE * GAME_SIZE; i++)
        generatedState.nums[i] = i + 1;
    generatedState.emptyIndex = GAME_SIZE * GAME_SIZE - 1;
    
    for (size_t i = 0; i < randomMoveCount; i++) {
        int n;
        Dir possibleMoves[4];
        possibleDirs(&generatedState, possibleMoves, &n);

        Dir move = possibleMoves[(rand() % n)];
        State* nState = nextState(&generatedState, move);
        memcpy(&generatedState, nState, sizeof(State));
        free(nState);
    }
    
    memcpy(state, &generatedState, sizeof(State));
}

int solvedCount(State* state) {
    int count = 0;
    for (size_t i = 0; i < GAME_SIZE * GAME_SIZE; i++) {
        if (state->nums[i] != i + 1)
            return count;
        count++;
    }

    return count;
}

int operator<(const State& s1, const State& s2) {
    for (size_t i = 0; i < GAME_SIZE * GAME_SIZE; i++) {
        if (s1.nums[i] < s2.nums[i]) {
            return 1;
        }
        else if (s1.nums[i] > s2.nums[i]) {
            return 0;
        }
    }
    return 0;
}

int operator<(const StateAndScore& s1, const StateAndScore& s2) {
    return s1.score < s2.score;
}

State* nextState(State* state, Dir dir) {
    State* next = new State;
    memcpy(next, state, sizeof(State));
    int prevEmptyIndex = next->emptyIndex;
    if (dir == RIGHT)
        next->emptyIndex += 1;
    else if (dir == LEFT)
        next->emptyIndex -= 1;
    else if (dir == UP)
        next->emptyIndex -= GAME_SIZE;
    else if (dir == DOWN)
        next->emptyIndex += GAME_SIZE;
    
    next->nums[prevEmptyIndex] = next->nums[next->emptyIndex];
    next->nums[next->emptyIndex] = GAME_SIZE * GAME_SIZE;

    return next;
}

void possibleDirs(State* state, Dir* dirs, int* n) {
    int nCache = 0;
    if ((state->emptyIndex % GAME_SIZE) != GAME_SIZE - 1) {
        dirs[nCache] = RIGHT;
        nCache++;
    }
    if ((state->emptyIndex % GAME_SIZE) != 0) {
        dirs[nCache] = LEFT;
        nCache++;
    }
    if ((state->emptyIndex / GAME_SIZE) != 0) {
        dirs[nCache] = UP;
        nCache++;
    }
    if ((state->emptyIndex / GAME_SIZE) != GAME_SIZE - 1) {
        dirs[nCache] = DOWN;
        nCache++;
    }
    *n = nCache;
}

int isEndState(State* state) {
    for (size_t i = 0; i < GAME_SIZE * GAME_SIZE - 1; i++) {
        int numInCell = i + 1;
        if (state->nums[i] != numInCell)
            return 0;
    }
    return 1;
}

void printState(const State* state) {
    for (size_t i = 0; i < GAME_SIZE; i++) {
        for (size_t j = 0; j < GAME_SIZE; j++) {
            if (state->nums[GAME_SIZE*i+j] == GAME_SIZE * GAME_SIZE)
                printf(" - ");
            else
                printf("%2d ", state->nums[GAME_SIZE*i+j]);
        }
        printf("\n");
    }
}

void printDir(Dir dir) {
    if (dir == LEFT)
        printf("LEFT");
    else if (dir == RIGHT)
        printf("RIGHT");
    else if (dir == UP)
        printf("UP");
    else if (dir == DOWN)
        printf("DOWN");
}

void printAllStates(std::set<State>& states) {
    printf("printing states\n");
    for (auto &&state : states) {
        printf("%d\n", state.emptyIndex);
    }
    printf("\n");
}

int h(State* state) {
    int distance = 0;
    for (size_t i = 0; i < GAME_SIZE * GAME_SIZE; i++) {
        int currX = i % GAME_SIZE,
            currY = i / GAME_SIZE;

        int destX, destY;
        if (state->nums[i] == GAME_SIZE * GAME_SIZE) {
            destX = currX;
            destY = currY;
        } else {
            destX = (state->nums[i] - 1) % GAME_SIZE;
            destY = (state->nums[i] - 1) / GAME_SIZE;
        }
        distance += abs(destX - currX) + abs(destY - currY);
    }

    // bonus for solved rows
    for (size_t i = 0; i < GAME_SIZE; i++) {
        bool isSolved = true;
        for (size_t j = 0; j < GAME_SIZE; j++) {
            int currectNumberInCell = GAME_SIZE * i + j + 1;
            if (state->nums[GAME_SIZE * i + j] != currectNumberInCell) {
                isSolved = false;
                break;
            }
        }
        if (isSolved) {
            distance -= GAME_SIZE / (i+1);
        }
    }
    

    
    return distance;
}

int hV2(State* state, int n) {
    // dont give penalty for missplaced pieces on the last row
    if (n > GAME_SIZE * (GAME_SIZE - 1) - 1)
        return h(state);
    
    int distance = 0;
    // give penalty for missplaced already solved pieces
    if (n != 0) {
        for (size_t i = 0; i < n - 1; i++) {
            if (state->nums[i] != i + 1)
                distance += 1;
        }
    }

    // try to give higher weight to placing the nth piece
    // int currX = n % GAME_SIZE,
    //     currY = n / GAME_SIZE;

    // int destX, destY;
    // if (state->nums[n] == GAME_SIZE * GAME_SIZE) {
    //     destX = currX;
    //     destY = currY;
    // } else {
    //     destX = (state->nums[n] - 1) % GAME_SIZE;
    //     destY = (state->nums[n] - 1) / GAME_SIZE;
    // }
    // distance += 10 * (abs(destX - currX) + abs(destY - currY));

    return distance + h(state);
}

int search(State* state, std::set<State>& searchedStates,  std::vector<Dir>& solution, int depthLimit, int depth) {
    if (depthLimit != -1 && depth > depthLimit)
        return 0;

    if (searchedStates.find(*state) != searchedStates.end())
        return 0;
    else {
        if (searchedStates.size() % 100000 == 0) {
            // printf("h=%d size=%d emptyindex=%d sizeBytes=%d\n",
            //     h(state),
            //     searchedStates.size(),
            //     state->emptyIndex,
            //     sizeof(searchedStates));
        }
    }

    searchedStates.insert(*state);
    if (isEndState(state)) {
        return 1;
    } else {
        Dir dirs[4];
        int n;
        possibleDirs(state, dirs, &n);
        int isFound = 0;
        
        std::vector<StateAndScore> stateAndScores;
        for (size_t i = 0; i < n; i++) {
            State* next = nextState(state, dirs[i]);
            stateAndScores.push_back({next, dirs[i], h(next)});
        }

        std::sort(stateAndScores.begin(), stateAndScores.end());

        for (size_t i = 0; i < n; i++) {
            int res = search(stateAndScores[i].state, searchedStates, solution, depthLimit, depth + 1);
            if (res) {
                solution.push_back(stateAndScores[i].dir);
                isFound = 1;
                break;
            } else {
                free(stateAndScores[i].state);
            }
        }

        return isFound;
    }
}

int iterativeDeepeningSearch(State* state, int depthLimit) {
    for (size_t i = 0; i < depthLimit; i++) {
        std::set<State> searchedStates;
        std::vector<Dir> solution;
        int res = search(state, searchedStates, solution, i);

        if (res) {
            printf("found %d!\n", solution.size());
            // NOTE i dont have to reverse, can traverse backwards
            std::reverse(solution.begin(), solution.end());
            for (auto &&dir : solution) {
                
            }
            
            return 1;
        } else
            printf("depth %d not found\n", i);
    }
    return 0;
}

int searchV2(State* state, std::set<State>& searchedStates,  std::vector<Dir>& solution, int highestSolved) {
    if (searchedStates.find(*state) != searchedStates.end())
        return 0;

    searchedStates.insert(*state);
    if (isEndState(state))
        return 1;

    Dir dirs[4];
    int n;
    possibleDirs(state, dirs, &n);
    int isFound = 0;
    
    std::vector<StateAndScore> stateAndScores;
    for (size_t i = 0; i < n; i++) {
        State* next = nextState(state, dirs[i]);
        stateAndScores.push_back({next, dirs[i], hV2(next, highestSolved)});
    }

    std::sort(stateAndScores.begin(), stateAndScores.end());

    for (size_t i = 0; i < n; i++) {
        int solved = solvedCount(stateAndScores[i].state);
        // TODO try without the max (maybe +1)
        int res = searchV2(stateAndScores[i].state, searchedStates, solution, std::max(solved, highestSolved));
        if (res) {
            solution.push_back(stateAndScores[i].dir);
            isFound = 1;
            break;
        } else {
            free(stateAndScores[i].state);
        }
    }

    return isFound;
}