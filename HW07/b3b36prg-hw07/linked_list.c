#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>

struct  node{
    int value;
    struct  node *next;
};
static struct node *head = NULL;
static int count = 0;

_Bool push(int entry){
    struct node *new_node = malloc(sizeof(struct node));
    if(!new_node){
        return false;
    }
    new_node->value = entry;
    new_node->next = NULL;
    if (head == NULL) {
        head = new_node;
    } else {
        struct node *cur = head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new_node;
    }
    count++;
    return true;
}

int pop(void){
    if(head==NULL){
        return -1;
    }
    struct node *old_head = head;
    int value = old_head->value;
    head = old_head->next;
    free(old_head);
    count--;
    return value;
}
_Bool insert(int entry){
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL){
        return false;
    }
    new_node->value = entry;
    if (head == NULL || entry >= head->value){
        new_node->next = head;
        head = new_node;
    } else {
        struct node *prev = head;
        struct node *cur = head->next;
        while (cur != NULL && entry < cur->value){
            prev = cur;
            cur = cur->next;
        }
        new_node->next = cur;
        prev->next = new_node;
    }
    count++;
    return true;
}
_Bool erase(int entry){
    _Bool found = false;
    struct node *prev = NULL;
    struct node *cur = head;
    while (cur!=NULL){
        if(cur->value==entry){
            if(prev==NULL){
                head = cur->next;
            }else{
                prev->next = cur->next;
            }
            struct node *old_node = cur;
            cur = cur->next;
            free(old_node);
            count--;
            found = true;            
        }else{
            prev = cur;
            cur = cur->next;
        }
    }
    return found;
}
int getEntry(int idx){
    if(idx<0 || idx>=count){
        return -1;
    }
    struct node *cur = head;
    for(unsigned int i = 0; i < idx; i++){
        cur = cur->next;
    }
    return cur->value;
}

int size(void){
    return count;
}

void clear(){
    struct node *cur = head;
    while(cur!=NULL){
        struct node *old_node = cur;
        cur = cur->next;
        free(old_node);
    }
    head = NULL;
    count = 0;
}

