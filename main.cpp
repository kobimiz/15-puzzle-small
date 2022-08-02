#include <stdio.h>
#include <time.h>
#include <string.h>

#include <set>
#include <vector>
#include <algorithm>

#include "puzzle15.h"

int main() {
    srand (time (NULL));

    State random;
    generateRandomState(&random);

    // int startState[GAME_SIZE * GAME_SIZE] = {
    //     2, 15, 9, 16,
    //     8, 7, 1, 6,
    //     4, 11, 12, 3,
    //     5, 10, 14, 13
    // };

    int startState[GAME_SIZE * GAME_SIZE] = {
        1,8,9,
        4,3,2,
        5,7,6
    };

    State state;
    // memcpy(&state, &random, sizeof(State));
    memcpy(state.nums, startState, sizeof(startState));
    state.emptyIndex = 2;

    std::set<State> searchedStates;
    std::set<State> searchedStates2;

    std::vector<Dir> solution;
    std::vector<Dir> solution2;
    // NOTE improvment idea: search for moves that put 1 into the right
    // spot, then 2 (without misplacing 1) etc
    search(&state, searchedStates, solution);
    searchV2(&state, searchedStates2, solution2, solvedCount(&state));
    std::reverse(solution.begin(), solution.end());
    std::reverse(solution2.begin(), solution2.end());

    printf("Starting state:\n");
    printState(&state);

    printf("\nSolution (length=%d):\n", solution.size());
    printf("\nSolution2 (length=%d):\n", solution2.size());

    iterativeDeepeningSearch(&state);
    // State* currState = &state;
    // ListNode* iter = list->head;
    // for (size_t i = 0; i < list->length; i++) {
    //     Dir* dir = (Dir*)iter->value;
        
    //     State* temp = currState;
    //     currState = nextState(currState, *dir);
    //     // if (i != 0)
    //     //     free(temp);

    //     // printDir(*dir);
    //     // printf(" ");
    //     // printf("\n");
    //     // printState(currState);
    //     // printf("\n");

    //     iter = iter->next;
    // }
    // printf("\n");

    // if (currState != &state) {
    //     free(currState);
    // }

    // deleteList(list);
}