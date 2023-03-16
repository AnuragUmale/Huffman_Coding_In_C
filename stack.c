#include "stack.h"
#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

Stack *stack_create(uint32_t capacity){
  Stack *s = (Stack *)malloc(sizeof(Stack));         
  s->top = 0;                                 
  s->capacity = capacity;                     
  s->items = malloc(capacity * sizeof(Node)); 
  return s;                                   
}

void stack_delete(Stack **s){
  free((*s)->items);  
  (*s)->items = NULL; 
  free(*s);           
  (*s) = NULL;        
}

bool stack_empty(Stack *s) { return s->top == 0; }

bool stack_full(Stack *s) { return s->top == s->capacity; }

uint32_t stack_size(Stack *s){
  uint32_t size; 
  size = s->top; 
  return size;   
}

bool stack_push(Stack *s, Node *n){
  if (stack_full(s)) {
    return false; 
  }
  s->items[s->top] = n; 
  s->top += 1;          
  return true;          
}

bool stack_pop(Stack *s, Node **n){
  if (stack_empty(s)) 
  {
    return false; 
  }
  s->top -= 1;           
  *n = s->items[s->top]; 
  s->items[s->top] = 0;  
  return true;           
}

void stack_print(Stack *s){
  for (uint32_t i = 0; i < s->capacity; i++) {
    node_print(s->items[i]); 
  }
}
