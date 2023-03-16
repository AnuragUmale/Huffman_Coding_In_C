#include "node.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency){
  Node *n = (Node *)malloc(sizeof(Node)); 
  n->frequency = frequency;               
  n->symbol = symbol;                     
  n->left = NULL;                         
  n->right = NULL;                        
  return n;                               
}

void node_delete(Node **n){
  free(*n);  
  *n = NULL; 
}

Node *node_join(Node *left, Node *right){
  Node *n = (Node *)malloc(sizeof(Node));
  if (n){
    n->right = right;
    n->left = left;
    n->symbol = '$';
    n->frequency = left->frequency + right->frequency;
  }
  return n;
}

void node_print(Node *n){
  if (n){
    printf("%c ", n->symbol); 
    node_print(n->left);      
    node_print(n->right);     
  }
  return;
}

bool node_cmp(Node *n, Node *m){
  return n->frequency > m->frequency; 
}

void node_print_sym(Node *n){
  if (iscntrl(n->symbol) && isprint(n->symbol)){
    printf("%c ", n->symbol); 
  }
  else{
    printf("0x%02" PRIx8 "\n", n->symbol); 
  }
}
