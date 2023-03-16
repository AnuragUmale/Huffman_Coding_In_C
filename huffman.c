#include "huffman.h"
#include "code.h"
#include "defines.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Code c = {0, {0}}; 

Node *build_tree(uint64_t hist[static ALPHABET]){
  Node *n;                                 
  Node *left;                              
  Node *right;                             
  Node *parent;                            
  PriorityQueue *pq = pq_create(ALPHABET); 
  for (uint32_t i = 0; i < ALPHABET; i++){
    if (hist[i] != 0){
      n = node_create(i, hist[i]); 
      enqueue(pq, n);              
    }
  }
  while (pq_size(pq) > 1){
    dequeue(pq, &left);              
    dequeue(pq, &right);             
    parent = node_join(left, right);                       
    enqueue(pq, parent);             
  }
  dequeue(pq, &parent); 
  pq_delete(&pq);       
  return parent;        
}

void build_codes(Node *root, Code table[static ALPHABET]){
  uint8_t bit_dump;
  if (root != NULL){
    if (!root->left && !root->right){
      table[root->symbol] = c; 
    }
    else{
      code_push_bit(&c, 0); 
      build_codes(root->left, table);                         
      code_pop_bit(&c, &bit_dump); 
      code_push_bit(&c, 1);        
      build_codes(root->right,  table);          
      code_pop_bit(&c, &bit_dump); 
    }
  }
}

void dump_tree(int outfile, Node *root){
  uint8_t L = 'L', I = 'I'; 
                            
  if (root){
    dump_tree(outfile, root->left);                        
    dump_tree(outfile, root->right); 
    if (!root->left && !root->right) {
      write_bytes(outfile, &L, 1); 
      write_bytes(outfile, &root->symbol, 1);             
    }
    else{
      write_bytes(outfile, &I, 1); 
    }
  }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]){
  Stack *stack = stack_create(ALPHABET); 
  Node *left = NULL;                     
  Node *right = NULL;                    
  Node *parent = NULL;                   
  for (int i = 0; i < nbytes; i++){
    if (tree[i] == 'L') {
      Node *n = node_create(tree[i + 1], 0); 
      stack_push(stack, n);     
      i++;                      
    }
    else if (tree[i] == 'I'){
      stack_pop(stack, &right);        
      stack_pop(stack, &left);         
      parent = node_join(left, right);                            
      stack_push(stack, parent);       
    }
  }
  stack_pop(stack, &parent); 
  stack_delete(&stack);      
  return parent;             
}

void delete_tree(Node **root){
  node_delete(&(*root)->left); 
  node_delete(&(*root)->right); 
  node_delete(root);
}
