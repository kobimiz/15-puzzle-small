#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <set>
#include <vector>
#include <algorithm>

#include "list.h"

#define GAME_SIZE 4

struct State {
    int nums[GAME_SIZE * GAME_SIZE];
    int emptyIndex;
};

typedef enum {
    RIGHT, LEFT, UP, DOWN
} Dir;



State* nextState(State* state, Dir dir) {
    State* next = (State*)malloc(sizeof(State));
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




struct StateAndScore {
    State* state;
    Dir dir;
    int score;    
};

void printAllStates(std::set<State>& states) {
    printf("printing states\n");
    for (auto &&state : states) {
        printf("%d\n", state.emptyIndex);
    }
    printf("\n");
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

int search(State* state, std::set<State>& searchedStates, List* list, int depthLimit = -1, int depth = 0) {
    if (depthLimit != -1 && depth > depthLimit)
        return 0;

    if (searchedStates.find(*state) != searchedStates.end()) {
        return 0;
    } else {
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
        if (n == 0)
            return 0;

        int isFound = 0;
        
        std::vector<StateAndScore> stateAndScores;
        for (size_t i = 0; i < n; i++) {
            State* next = nextState(state, dirs[i]);
            stateAndScores.push_back({next, dirs[i], h(next)});
        }

        std::sort(stateAndScores.begin(), stateAndScores.end());

        for (size_t i = 0; i < n; i++) {
            int res = search(stateAndScores[i].state, searchedStates, list, depthLimit, depth + 1);
            if (res) {
                Dir* dir = (Dir*)malloc(sizeof(Dir));
                *dir = stateAndScores[i].dir;
                appendNode(list, dir);
                isFound = 1;
                break;
            } else {
                free(stateAndScores[i].state);
            }
        }

        return isFound;
    }
}


int iterativeDeepeningSearch(State* state, int depthLimit = 200) {
    for (size_t i = 0; i < depthLimit; i++) {
        std::set<State> searchedStates;
        List* list = createList();
        int res = search(state, searchedStates, list, i);

        if (res) {
            printf("found %d!\n", list->length);
            ListNode* iter = list->head;
            for (size_t i = 0; i < list->length; i++) {
                free(iter->value);
                iter = iter->next;
            }
            return 1;
        } else {
            printf("depth %d not found\n", i);
        }
        deleteList(list);
    }
    return 0;
}

void reverseList(ListNode* iter, ListNode* prev) {
    if (iter->next != NULL)
        reverseList(iter->next, iter);
    iter->next = prev;

    if (iter == prev)
        iter->next = NULL;
}

void reverseList(List* list) {
    reverseList(list->head, list->head);
    ListNode* temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

void printList(List* list) {
    ListNode* iter = list->head;

    while (iter) {
        printf("%d ", (long long)iter->value);
        iter = iter->next;
    }
    printf("\n");
}

void generateRandomState(State* state) {
    int randomMoveCount = 300;

    // starting state
    State generatedState;
    for (size_t i = 0; i < GAME_SIZE * GAME_SIZE; i++)
        generatedState.nums[i] = i + 1;
    generatedState.emptyIndex = 15;
    
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

int main() {
    srand (time (NULL));

    State random;
    generateRandomState(&random);

    int startState[GAME_SIZE * GAME_SIZE] = {
        2, 15, 9, 16,
        8, 7, 1, 6,
        4, 11, 12, 3,
        5, 10, 14, 13
    };

    // int startState[GAME_SIZE * GAME_SIZE] = {
    //     1,8,9,
    //     4,3,2,
    //     5,7,6
    // };

    State state;
    // memcpy(&state, &random, sizeof(State));
    memcpy(state.nums, startState, sizeof(startState));
    state.emptyIndex = 3;

    std::set<State> searchedStates;
    List* list = createList();
    // NOTE improvment idea: search for moves that put 1 into the right
    // spot, then 2 (without misplacing 1) etc
    search(&state, searchedStates, list);
    reverseList(list);


    printf("Starting state:\n");
    printState(&state);

    printf("\nSolution (length=%d):\n", list->length);
    State* currState = &state;
    ListNode* iter = list->head;
    for (size_t i = 0; i < list->length; i++) {
        Dir* dir = (Dir*)iter->value;
        
        State* temp = currState;
        currState = nextState(currState, *dir);
        // if (i != 0)
        //     free(temp);

        // printDir(*dir);
        // printf(" ");
        // printf("\n");
        // printState(currState);
        // printf("\n");

        iter = iter->next;
    }
    printf("\n");

    if (currState != &state) {
        free(currState);
    }

    iter = list->head;
    for (size_t i = 0; i < list->length; i++) {
        free(iter->value);
        iter = iter->next;
    }
    deleteList(list);
}