#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"
#include "double_linked_list.h"

#define LIST_NULL_NO_SIZE -1

struct PriorityQueue_t {
    LinkedList elements_list;
    LinkedNode iterator;
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priorities;
};


typedef struct {
    PQElement pq_element;
    PQElementPriority pq_element_priority;
} *CombinedElement;


/* Gets a queue and element and returns the first node in the queue's
 * elements_list that has its element value equal to the element parameter */
static LinkedNode getFirstOccurenceByElement(PriorityQueue queue, PQElement element) {
    if (queue == NULL || element == NULL) {
        return NULL;
    }

    LIST_FOREACH(queue->elements_list, node) {
        if (queue->equal_elements(((CombinedElement) listGetData(node))->pq_element, element)) {
            return node;
        }
    }

    return NULL;
}

/* Gets a queue, element and priority and returns the first node in the queue's
 * elements_list that has its element and priority values equal to the element and priority parameters */
static LinkedNode
getFirstOccurenceByElementAndPriority(PriorityQueue queue, PQElement element, PQElementPriority priority) {
    if (queue == NULL || element == NULL || priority == NULL) {
        return NULL;
    }

    LinkedNode current_node = listGetFirst(queue->elements_list);
    while (current_node != NULL) {
        PQElement current_element = ((CombinedElement) listGetData(current_node))->pq_element;
        PQElementPriority current_priority = ((CombinedElement) listGetData(current_node))->pq_element_priority;

        if (queue->equal_elements(current_element, element) &&
            queue->compare_priorities(current_priority, priority) == 0) {
            return current_node;
        }

        current_node = listGetNext(current_node);
    }

    return NULL;
}

/* Gets a queue and a priority and gets the smallest node in the
 * queues elements_list that it's priority is bigger than the priority parameter */
static LinkedNode getPreviousBiggestNode(PriorityQueue queue, PQElementPriority priority) {
    if (queue == NULL || priority == NULL) {
        return NULL;
    }

    LinkedNode current_node = listGetFirst(queue->elements_list);
    LinkedNode last_node = NULL;
    while (current_node != NULL) {
        PQElementPriority current_priority = ((CombinedElement) listGetData(current_node))->pq_element_priority;

        if (queue->compare_priorities(current_priority, priority) < 0) {
            return last_node;
        }

        last_node = current_node;
        current_node = listGetNext(current_node);
    }

    return last_node;
}

/* Gets a CombinedElement and frees all the data linked to it (the pointer itself and both the element and priority it contains) */
static void destroyCombinedElement(PriorityQueue queue, CombinedElement combined_element) {
    queue->free_element(combined_element->pq_element);
    queue->free_priority(combined_element->pq_element_priority);
    free(combined_element);
}

/* Gets a list node from the elements_list and removes it (freeing the CombinedElement and both of its fields) */
static PriorityQueueResult pqRemoveByNode(PriorityQueue queue, LinkedNode target_node) {
    if (queue == NULL || target_node == NULL) {
        return PQ_NULL_ARGUMENT;
    }
    destroyCombinedElement(queue, listGetData(target_node));
    listRemove(queue->elements_list, target_node);
    queue->iterator = NULL;
    return PQ_SUCCESS;
}

/* Gets an element and a priority and creates a CombinedElement that holds the both of them together */
static CombinedElement createCombinedElement(PriorityQueue queue, PQElement element, PQElementPriority priority) {
    CombinedElement combined_element = malloc(sizeof(*combined_element));
    if (combined_element == NULL) {
        return NULL;
    }

    combined_element->pq_element = queue->copy_element(element);
    combined_element->pq_element_priority = queue->copy_priority(priority);
    if (combined_element->pq_element == NULL || combined_element->pq_element_priority == NULL) {
        destroyCombinedElement(queue, combined_element);
        return NULL;
    }

    return combined_element;
}

/* Gets a queue and returns a copy of its elements_list */
static LinkedList copyElementsList(PriorityQueue queue) {
    LinkedList list = queue->elements_list;
    LinkedList new_list = listCreate();

    LinkedNode last_node = NULL;
    LIST_FOREACH(list, node) {
        CombinedElement current_combined_element = listGetData(node);
        CombinedElement new_combined_element = createCombinedElement(queue, current_combined_element->pq_element,
                                                                     current_combined_element->pq_element_priority);
        if (new_combined_element == NULL) {
            listDestroy(new_list);
            return NULL;
        }

        if (last_node == NULL) {
            last_node = listInsertStart(new_list, new_combined_element);
        } else {
            last_node = listInsertAfter(new_list, last_node, new_combined_element);
        }

        if (last_node == NULL) {
            listDestroy(new_list);
            return NULL;
        }
    }

    return new_list;
}


PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities) {
    if (copy_element == NULL || free_element == NULL || equal_elements == NULL || copy_priority == NULL ||
        free_priority == NULL || compare_priorities == NULL) {
        return NULL;
    }

    PriorityQueue queue = malloc(sizeof(struct PriorityQueue_t));

    LinkedList element_list = listCreate();
    if (queue == NULL || element_list == NULL) {
        free(queue);
        free(element_list);
        return NULL;
    }

    queue->elements_list = element_list;
    queue->iterator = NULL;
    queue->copy_element = copy_element;
    queue->free_element = free_element;
    queue->equal_elements = equal_elements;
    queue->copy_priority = copy_priority;
    queue->free_priority = free_priority;
    queue->compare_priorities = compare_priorities;

    return queue;
}

void pqDestroy(PriorityQueue queue) {
    pqClear(queue);
    free(queue->elements_list);
    free(queue);
}

PriorityQueue pqCopy(PriorityQueue queue) {
    if (queue == NULL) {
        return NULL;
    }

    PriorityQueue new_queue = pqCreate(queue->copy_element, queue->free_element, queue->equal_elements,
                                       queue->copy_priority, queue->free_priority, queue->compare_priorities);
    LinkedList new_list = copyElementsList(queue);
    if (new_queue == NULL || new_list == NULL) {
        listDestroy(new_list);
        pqDestroy(new_queue);
        return NULL;
    }

    listDestroy(new_queue->elements_list);
    new_queue->elements_list = new_list;

    new_queue->iterator = NULL;
    queue->iterator = NULL;
    return new_queue;
}

int pqGetSize(PriorityQueue queue) {
    if (queue == NULL) {
        return LIST_NULL_NO_SIZE;
    }
    return listGetSize(queue->elements_list);
}

bool pqContains(PriorityQueue queue, PQElement element) {
    if (queue == NULL || element == NULL) {
        return false;
    }
    return getFirstOccurenceByElement(queue, element) != NULL;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority) {
    if (queue == NULL || element == NULL || priority == NULL) {
        return PQ_NULL_ARGUMENT;
    }

    CombinedElement combined_element = createCombinedElement(queue, element, priority);
    if (combined_element == NULL) {
        return PQ_OUT_OF_MEMORY;
    }

    LinkedNode previous_biggest_node = getPreviousBiggestNode(queue, priority);
    LinkedNode inserted_node = NULL;
    if (previous_biggest_node == NULL) {
        inserted_node = listInsertStart(queue->elements_list, combined_element);
    } else {
        inserted_node = listInsertAfter(queue->elements_list, previous_biggest_node, combined_element);
    }

    if (inserted_node == NULL) {
        destroyCombinedElement(queue, combined_element);
        return PQ_OUT_OF_MEMORY;
    }

    queue->iterator = NULL;
    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority) {
    if (queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL) {
        return PQ_NULL_ARGUMENT;
    }

    LinkedNode target_node = getFirstOccurenceByElementAndPriority(queue, element, old_priority);
    if (target_node == NULL) {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }


    PriorityQueueResult insert_result = pqInsert(queue, element, new_priority);
    if (insert_result == PQ_OUT_OF_MEMORY) {
        return PQ_OUT_OF_MEMORY;
    }

    pqRemoveByNode(queue, target_node);
    queue->iterator = NULL;
    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue) {
    if (queue == NULL) {
        return PQ_NULL_ARGUMENT;
    }

    pqRemoveByNode(queue, listGetFirst(queue->elements_list));

    return PQ_SUCCESS;
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element) {
    if (queue == NULL || element == NULL) {
        return PQ_NULL_ARGUMENT;
    }

    LinkedNode target_node = getFirstOccurenceByElement(queue, element);
    if (target_node == NULL) {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }

    pqRemoveByNode(queue, target_node);
    return PQ_SUCCESS;
}

PQElement pqGetFirst(PriorityQueue queue) {
    if (queue == NULL) {
        return NULL;
    }

    LinkedNode first_node = listGetFirst(queue->elements_list);
    queue->iterator = first_node;
    if (first_node == NULL) {
        return NULL;
    }
    return ((CombinedElement) listGetData(first_node))->pq_element;
}

PQElement pqGetNext(PriorityQueue queue) {
    if (queue == NULL) {
        return NULL;
    }

    if (queue->iterator == NULL) {
        return NULL;
    }

    queue->iterator = listGetNext(queue->iterator);
    if (queue->iterator == NULL) {
        return NULL;
    }

    return ((CombinedElement) listGetData(queue->iterator))->pq_element;
}

PriorityQueueResult pqClear(PriorityQueue queue) {
    if (queue == NULL) {
        return PQ_NULL_ARGUMENT;
    }


    while (pqGetSize(queue) > 0) {
        pqRemove(queue);
    }

    return PQ_SUCCESS;
}




