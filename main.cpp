#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <set>
#include <vector>
#include <algorithm>

#include "list.h"
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
    List* list = createList();
    List* list2 = createList();
    // NOTE improvment idea: search for moves that put 1 into the right
    // spot, then 2 (without misplacing 1) etc
    search(&state, searchedStates, list);
    searchV2(&state, searchedStates2, list2, solvedCount(&state));
    reverseList(list);


    printf("Starting state:\n");
    printState(&state);

    printf("\nSolution (length=%d):\n", list->length);
    printf("\nSolution2 (length=%d):\n", list2->length);

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

    // iter = list->head;
    // for (size_t i = 0; i < list->length; i++) {
    //     free(iter->value);
    //     iter = iter->next;
    // }
    // deleteList(list);
}