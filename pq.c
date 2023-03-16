#include "pq.h"
#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct PriorityQueue {
  uint32_t capacity;
  uint32_t size;
  Node **items_in_queue;
};

static inline uint32_t priority_queue_left_child(uint32_t n){
  uint32_t return_value = 2 * n + 1; 
  return return_value;               
}

static inline uint32_t priority_queue_right_child(uint32_t n){
  uint32_t return_value = 2 * n + 2; 
  return return_value;               
}

static inline int priority_queue_parent(uint32_t n){
  return (int)(n - 1) / 2; 
}

PriorityQueue *pq_create(uint32_t capacity){
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue)); 
  pq->capacity = capacity;    
  pq->size = 0;               
  pq->items_in_queue = malloc(capacity * sizeof(Node)); 
  return pq;                           
}

void pq_delete(PriorityQueue **q){
  free((*q)->items_in_queue);  
  (*q)->items_in_queue = NULL; 
  free(*q);                    
  *q = NULL;                   
}

void priority_queue_swap(PriorityQueue *q, uint32_t index_1, uint32_t index_2){
  Node *temp_var = q->items_in_queue[index_1]; 
  q->items_in_queue[index_1] = q->items_in_queue[index_2];        
  q->items_in_queue[index_2] = temp_var; 
}

void priority_queue_up_heap(PriorityQueue *q){
  uint32_t i = q->size - 1; 
  while (priority_queue_parent(i) >= 0 && ((q->items_in_queue[priority_queue_parent(i)]->frequency) > (q->items_in_queue[i]->frequency))){
    priority_queue_swap(q, priority_queue_parent(i), i); 
    i = priority_queue_parent(i);                        
  }
}

void priority_queue_down_heap(PriorityQueue *q){
  uint32_t i = 0;
  while (priority_queue_left_child(i) < q->size) {
    uint32_t small_child_index = priority_queue_left_child(i); 
    if (priority_queue_right_child(i) < q->size &&  ((q->items_in_queue[priority_queue_right_child(i)]->frequency) < (q->items_in_queue[priority_queue_left_child(i)]->frequency))){
      small_child_index = priority_queue_right_child(i); 
    }
    if ((q->items_in_queue[i]->frequency) > (q->items_in_queue[small_child_index]->frequency)){
      priority_queue_swap(q, i, small_child_index);
    }
    else {
      break; 
    }
    i = small_child_index; 
  }
}

bool pq_empty(PriorityQueue *q){
  uint32_t size_of_queue = q->size;
  uint32_t zero = 0;
  if (size_of_queue == zero) {
    return true; 
  }
  else {
    return false; 
  }
}

bool pq_full(PriorityQueue *q){
  uint32_t capacity_of_queue = q->capacity;
  uint32_t size_of_queue = q->size;
  if (capacity_of_queue == size_of_queue){
    return true; 
  }
  else{
    return false; 
  }
}

uint32_t pq_size(PriorityQueue *q){
  return q->size; 
}

bool enqueue(PriorityQueue *q, Node *n){
  uint32_t capacity_of_queue = q->capacity;
  uint32_t size_of_queue = q->size;
  if (capacity_of_queue == size_of_queue){
    return false; 
  }
  q->items_in_queue[q->size] = n; 
  q->size++;                      
  priority_queue_up_heap(q);      
  return true;                    
}

bool dequeue(PriorityQueue *q, Node **n){
  if (q->size == 0){
    return false; 
  }
  *n = q->items_in_queue[0]; 
  q->items_in_queue[0] = q->items_in_queue[q->size - 1]; 
  q->size -= 1;                       
  priority_queue_down_heap(q);        
  return true;                        
}

void pq_print(PriorityQueue *q){
  for (uint32_t i = 0; i < q->size; i++){
    node_print(q->items_in_queue[i]); 
                                      
  }
}
