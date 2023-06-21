#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "queue.h"

#define INITIAL_CAPACITY 8
#define RESIZE_FACTOR 2
#define SHRINK_TRESHOLD 3

int get_queue_size(queue_t *queue){
   return queue->size;
}

void *pop_from_queue(queue_t *queue){
   if(queue->size==0){
      return NULL;
   }

   void *data = queue->buffer[queue->head];
   queue->head = (queue->head+1) % queue->capacity;
   queue->size--;

   if(queue->capacity>INITIAL_CAPACITY && queue->size<=queue->capacity/SHRINK_TRESHOLD){
    int newCapacity = queue->capacity/RESIZE_FACTOR;
    void **newBuffer = (void**) malloc(newCapacity*sizeof(void*));
    if(newBuffer==NULL){
        return data;
    }
    for(unsigned int i=0; i<queue->size; i++){
        newBuffer[i] = queue->buffer[(queue->head+i) % queue->capacity];
    }
    free(queue->buffer);
    queue->buffer = newBuffer;
    queue->head = 0;
    queue->tail = queue->size;
    queue->capacity = newCapacity;
   }
   return data;
}

bool push_to_queue(queue_t *queue, void *data){
    if(queue->size==queue->capacity){
        int newCapacity = queue->capacity*RESIZE_FACTOR;
        void **newBuffer = (void**) malloc(newCapacity*sizeof(void*));
        if(newBuffer==NULL){
            return false;
        }
        for(unsigned int i=0; i<queue->size; i++){
            newBuffer[i] = queue->buffer[(queue->head+i)%queue->capacity];
        }
        free(queue->buffer);
        queue->buffer=newBuffer;
        queue->head = 0;
        queue->tail = queue->size;
        queue->capacity = newCapacity;
    }
    queue->buffer[queue->tail] = data;
    queue->tail = (queue->tail+1) % queue->capacity;
    queue->size++;
    return true;
}

void delete_queue(queue_t *queue){
   free(queue->buffer);
   free(queue);
}

queue_t *create_queue(int capacity){
   queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
   queue->buffer = (void**) malloc(capacity*sizeof(void*));

   queue->head=0;
   queue->tail=0;
   queue->capacity=capacity;
   queue->size=0;

   return queue;
}

void *get_from_queue(queue_t *queue, int idx){
   if(idx<0 || idx>=queue->size){
      return NULL;
   }

   int index = (queue->head+idx) % queue->capacity;
   void *data = queue->buffer[index];
   return data;
}


