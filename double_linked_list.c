#include <stdlib.h>
#include <assert.h>
#include "double_linked_list.h"

struct linkedNode {
    NodeData data;
    LinkedNode prev;
    LinkedNode next;
};

struct linkedList {
    LinkedNode head;
    int size;
};

/* Creates a new list node with it's data pointing to node_data */
static NodeData createNode(NodeData node_data) {
    LinkedNode node = malloc(sizeof(struct linkedNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = node_data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}


LinkedList listCreate() {
    LinkedList list = malloc(sizeof(struct linkedList));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->size = 0;

    return list;
}


void listDestroy(LinkedList list) {
    if(list == NULL) {
        return;
    }

    LIST_FOREACH(list, node){
        free(node);
    }

    free(list);
}

LinkedNode listRemove(LinkedList list, LinkedNode node) {
    if (node == NULL) {
        return NULL;
    }

    if (list->head == node) {
        list->head = node->next;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    free(node);
    list->size--;
    return node;
}

LinkedNode listRemoveFirst(LinkedList list) {
    return listRemove(list, list->head);
}

LinkedNode listInsertAfter(LinkedList list, LinkedNode target_node, NodeData node_data) {
    if (target_node == NULL) {
        return NULL;
    }

    LinkedNode node = createNode(node_data);
    if (node == NULL) {
        return NULL;
    }

    if(target_node->next != NULL) {
        target_node->next->prev = node;
    }
    node->prev = target_node;
    node->next = target_node->next;
    target_node->next = node;
    list->size++;

    return node;
}

LinkedNode listInsertStart(LinkedList list, NodeData node_data) {
    if (list == NULL) {
        return NULL;
    }
    LinkedNode node = createNode(node_data);
    if (node == NULL) {
        return NULL;
    }

    if(list->head != NULL) {
        list->head->prev = node;
    }

    node->next = list->head;
    list->head = node;
    list->size++;
    return node;
}

NodeData listGetData(LinkedNode node) {
    if (node == NULL) {
        return NULL;
    }
    return node->data;
}

int listGetSize(LinkedList list) {
    if(list == NULL) {
        return -1;
    }
    return list->size;
}

LinkedNode listGetNext(LinkedNode node) {
    if (node == NULL) {
        return NULL;
    }
    return node->next;
}

LinkedNode listGetPrev(LinkedNode node) {
    if (node == NULL) {
        return NULL;
    }
    return node->prev;
}

LinkedNode listGetFirst(LinkedList list) {
    if (list == NULL) {
        return NULL;
    }
    return list->head;
}

