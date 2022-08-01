#ifndef LIST_H
#define LIST_H

typedef struct ListNode {
    struct ListNode* next;
    void* value;
} ListNode; 



typedef struct List {
    ListNode *head, *tail;
    int length;
} List;

List* createList();
void deleteList(List* list);
void appendNode(List* list, void* value);
void deleteFirstNodeByValue(List* list, void* value);

#endif