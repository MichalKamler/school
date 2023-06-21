#include "queue.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue_node{
    void *data;
    struct queue_node *next;
} QueueNode;

typedef struct{
    QueueNode *head;
    QueueNode *tail;
    int size;
    int (*compare)(const void *, const void *);
    void (*clear)(void *);
} Queue;

void* create(){
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if(queue == NULL){
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    queue->compare = NULL;
    queue->clear = free; // default clear function is free()
    return queue;
}

void clear(void *queue_ptr){
    Queue *queue = (Queue*)queue_ptr;
    while(queue->head != NULL){
        QueueNode *node = queue->head;
        queue->head = node->next;
        queue->clear(node->data);
        free(node);
    }
    queue->tail = NULL;
    queue->size = 0;
}

_Bool push(void *queue_ptr, void *entry){
    Queue *queue = (Queue*)queue_ptr;
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    if(node == NULL){
        return false;
    }
    node->data = entry;
    node->next = NULL;
    if(queue->tail != NULL){
        queue->tail->next = node;
    }else{
        queue->head = node;
    }
    queue->tail = node;
    queue->size++;
    return true;
}

void* pop(void *queue_ptr){
    Queue *queue = (Queue*)queue_ptr;
    if(queue->head == NULL){
        return NULL;
    }
    QueueNode *node = queue->head;
    queue->head = node->next;
    if(queue->head == NULL){
        queue->tail = NULL;
    }
    queue->size--;
    void *entry = node->data;
    free(node);
    return entry;
}

_Bool insert(void *queue, void *entry){
    if(queue == NULL || entry == NULL){
        return false;
    }    
    QueueNode *newNode = malloc(sizeof(QueueNode));
    if(newNode == NULL){
        return false;
    }
    newNode->data = entry;
    newNode->next = NULL;    
    Queue *q = (Queue *)queue;
    if (q->head == NULL) {
        q->head = q->tail = newNode;
        q->size++;
        return true;
    }    
    int(*compare)(const void *, const void *) = q->compare;
    if(compare == NULL){
        return false;
    }
    QueueNode *prev = NULL, *curr = q->head;
    while(curr != NULL && compare(entry, curr->data) >= 0){
        prev = curr;
        curr = curr->next;
    }    
    if(prev == NULL){
        newNode->next = q->head;
        q->head = newNode;
    }else{
        newNode->next = prev->next;
        prev->next = newNode;
    }    
    if(newNode->next == NULL){
        q->tail = newNode;
    }    
    q->size++;
    return true;
}

_Bool erase(void *queue, void *entry) {
    if(queue == NULL || entry == NULL){
        return false;
    }
    _Bool found = false;
    int queue_size = size(queue);
    for(int i = 0; i < queue_size; i++){
        void *item = getEntry(queue, i);
        if(item == NULL){
            continue;
        }
        if(item == entry || *(int*)item == *(int*)entry){
            // Found the entry, remove it
            void *removed_item = pop(queue);
            if(removed_item != NULL){
                clear(removed_item);
                free(removed_item);
                found = true;
            }
        }else{
            // Item doesn't match, push it back into the queue
            push(queue, item);
        }
    }
    return found;
}
void* getEntry(const void *queue, int idx){
    if(queue == NULL || idx < 0 || idx >= size(queue)){
        return NULL;
    }
    int curr_idx = 0;
    void *item = NULL;
    void *temp_queue = create();
    while((item = pop(queue)) != NULL){
        if (curr_idx == idx) {
            break;
        }
        push(temp_queue, item);
        curr_idx++;
    }
    // Restore the original queue
    while((item = pop(temp_queue)) != NULL){
        push(queue, item);
    }
    // Free the temporary queue
    clear(temp_queue);
    free(temp_queue);
    return item;
}

int size(const void *queue){
    if(queue == NULL){
        return 0;
    }
    Queue *q = (Queue *) queue;
    return q->size;
}

void setCompare(void *queue, int (*compare)(const void *, const void *)){
    if(queue == NULL || compare == NULL){
        return;
    }
    Queue *q = (Queue *) queue;
    q->compare = compare;
}

void setClear(void *queue, void (*clear)(void *)){
    if(queue == NULL){
        return;
    }
    Queue *q = (Queue *) queue;
    q->clear = (clear != NULL) ? clear : free;
}
