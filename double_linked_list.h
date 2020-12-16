#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

/* Type for defining the list nodes */
typedef struct linkedNode *LinkedNode;
/* Type for defining the list controller object */
typedef struct linkedList *LinkedList;

/* Data element data type for the list */
typedef void *NodeData;


/* Creates a new list */
LinkedList listCreate();

/* Returns the amount of nodes currently in the list */
int listGetSize(LinkedList list);

/* Frees all the memory allocated by the list (does not free the data inserted to the list) */
void listDestroy(LinkedList list);

/* Removes the given node parameter from the list */
LinkedNode listRemove(LinkedList list, LinkedNode node);

/* Removes the first node in the list */
LinkedNode listRemoveFirst(LinkedList list);

/* Inserts a node after the parameter target_node inside the list */
LinkedNode listInsertAfter(LinkedList list, LinkedNode target_node, NodeData node_data);

/* Inserts a node at the start of the list */
LinkedNode listInsertStart(LinkedList list, NodeData node_data);

/* Gets a node parameter and returns the data it points to */
NodeData listGetData(LinkedNode node);

/* Gets a node parameter and returns the next node inline (the one node points to) */
LinkedNode listGetNext(LinkedNode node);

/* Gets a node parameter and returns the next node inline (the one node points to forwards) */
LinkedNode listGetPrev(LinkedNode node);

/* Gets a node parameter and returns the previous node inline (the one node points to backwards) */
LinkedNode listGetFirst(LinkedList list);

/* Macro to iterate over all of the list nodes */
#define LIST_FOREACH(list, iter) \
    for(LinkedNode iter = listGetFirst(list); iter != NULL; iter = listGetNext(iter))


#endif //LINKED_LIST_H
