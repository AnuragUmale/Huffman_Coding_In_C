#include "code.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t buffer[BLOCK];
static uint32_t bit_index = BLOCK * 8;
static uint32_t index = 0;
static uint32_t end = 0;


int read_bytes(int infile, uint8_t *buf, int nbytes){
  int total = 0;
  int bytes = -2; 
  while ((bytes > 0 || bytes == -2) && total < nbytes) {
    bytes = read(infile, buf + total, nbytes - total); 
    total += bytes;               
  }
  bytes_read += total; 
  return total;        
}

int write_bytes(int outfile, uint8_t *buf, int nbytes){
  int total = 0;  
  int bytes = -2; 
  while ((bytes > 0 || bytes == -2) && total < nbytes) {
    bytes = write(outfile, buf + total, nbytes - total); 
    total += bytes;                
  }
  bytes_written += total; 
  return total;
}

bool read_bit(int infile, uint8_t *bit){
  if (bit_index == BLOCK * 8){
    end = (read_bytes(infile, buffer, BLOCK) * 8) + 1;       
    bit_index = 0; 
  }
  uint32_t index_of_byte = bit_index / 8; 
  uint32_t index_of_bit = bit_index % 8;  
  *bit = (buffer[index_of_byte] >> index_of_bit) & 1;             
  bit_index++;          
  if (bit_index == end){
    return false; 
  }
  else {
    return true; 
  }
}

void write_code(int outfile, Code *c){
  uint32_t index_of_byte, index_of_bit;                     
  for (uint32_t i = 0; i < c->top; i++){
    index_of_byte = index / 8; 
    index_of_bit = index % 8;  
    if (code_get_bit(c, i) == 1) 
    {
      buffer[index_of_byte] |=  (1 << index_of_bit); 
    }
    else{
      buffer[index_of_byte] &= ~(1 << index_of_bit); 
    }
    index++;                
    if (index == BLOCK * 8){
      write_bytes(outfile, buffer, BLOCK); 
      index = 0;                           
    }
  }
}

void flush_codes(int outfile){
  uint32_t block_times_eight = BLOCK * 8; 
  if (index < block_times_eight){
    while (index % 8 != 0) {
      uint32_t byte_pos = index / 8;       
      uint32_t bit_pos = index % 8;        
      buffer[byte_pos] &= ~(1 << bit_pos); 
      index++;                             
    }
    write_bytes(outfile, buffer, index / 8); 
  }
}
