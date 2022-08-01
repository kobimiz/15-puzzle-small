#include <stdlib.h>

#include "list.h"

List* createList() {
    List* res = (List*)malloc(sizeof(List));
    res->head = NULL;
    res->tail = NULL;
    res->length = 0;
    return res;
}

void deleteList(List* list) {
    if (!list) return;

    ListNode* iter = list->head;
    while (iter) {
        ListNode* temp = iter;
        iter = iter->next;
        free(temp);
    }
    free(list);
}

void appendNode(List* list, void* value) {
    if (!list) return;

    ListNode* newTail = (ListNode*)malloc(sizeof(ListNode));
    newTail->value = value;
    newTail->next = NULL;
    
    if (!list->head) {
        list->head = newTail;
        list->tail = newTail;
    } else {
        ListNode* prevTail = list->tail;
        list->tail = newTail;
        prevTail->next = newTail;
    }

    list->length++;
}

void deleteFirstNodeByValue(List* list, void* value) {
    if (!list) return;

    if (list->head->value == value) {
        ListNode* head = list->head;
        list->head = list->head->next;
        if (list->head == list->tail)
            list->tail = NULL;
        free(head);
    }

    ListNode* iter = list->head;
    while (iter->next) {
        if (iter->next->value == value) {
            ListNode* toDelete = iter->next;
            if (toDelete == list->tail)
                list->tail = iter;
            iter->next = toDelete->next;
            free(toDelete);
            break;
        }
        iter = iter->next;
    }

    list->length--;
}