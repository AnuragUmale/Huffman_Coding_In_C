#include "code.h"
#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

Code code_init(void){
  Code c;                                      
  c.top = 0;                                   
  for (uint32_t i = 0; i < MAX_CODE_SIZE; i++) {
    c.bits[i] = 0; 
  }
  return c; 
}

uint32_t code_size(Code *c){
  return c->top; 
}

bool code_empty(Code *c){
  if (c->top == 0){
    return true; 
  }
  else{
    return false; 
  }
}

bool code_full(Code *c){
  if (c->top == MAX_CODE_SIZE){
    return true; 
  }
  else{
    return false; 
  }
}

bool code_set_bit(Code *c, uint32_t i){
  if (i > c->top){
    return false; 
  }
  uint32_t position_of_byte, position_of_bit; 
  position_of_byte = i / 8;                   
  position_of_bit = i % 8;                    
  c->bits[position_of_byte] |= (1 << position_of_bit); 
  return true;                
}

bool code_clr_bit(Code *c, uint32_t i){
  if (i > c->top) {
    return false; 
  }
  uint32_t position_of_byte, position_of_bit; 
  position_of_byte = i / 8;                   
  position_of_bit = i % 8;                    
  c->bits[position_of_byte] &= ~(1 << position_of_bit); 
                               
  return true;                 
}

bool code_get_bit(Code *c, uint32_t i){
  return (c->bits[i / 8] >> i % 8) & 0x1; 
              
}

bool code_push_bit(Code *c, uint8_t bit){
  if (c->top != MAX_CODE_SIZE) {
    if (bit) {
      code_set_bit(c, c->top); 
    }
    else{
      code_clr_bit(c, c->top); 
    }
    c->top++;    
    return true; 
  }
  else{
    return false; 
  }
}

bool code_pop_bit(Code *c, uint8_t *bit){
  if (code_empty(c)) {
    return false; 
  }
  else {
    *bit = code_get_bit(c, c->top--); 
    return true;      
  }
}

void code_print(Code *c){
  for (uint32_t x = 0; x < c->top; x++){
    if (code_get_bit(c, x)){
      printf("1");
    }
    else  {
      printf("0");
    }
  }
  printf("\n"); 
}
