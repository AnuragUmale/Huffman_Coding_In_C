#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"
#include <fcntl.h>
#include <inttypes.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hi:o:v"


void usage(void){
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "  A Huffman encoder.\n");
  fprintf(stderr, "  Compresses a file using the Huffman coding algorithm.\n\n");
  fprintf(stderr, "USAGE\n");
  fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n\n");
  fprintf(stderr, "OPTIONS\n");
  fprintf(stderr, "  -h             Program usage and help.\n");
  fprintf(stderr, "  -v             Print compression statistics.\n");
  fprintf(stderr, "  -i infile      Input file to compress.\n");
  fprintf(stderr, "  -o outfile     Output of compressed data.\n");
}

int main(int argc, char **argv){
  int opt = 0;                   
  int infile = STDIN_FILENO;     
  int outfile = STDOUT_FILENO;   
  bool print_stats_flag = false; 
  Header h;                      
  Node *root;                    
  uint8_t buf[BLOCK];            
  uint64_t histogram[ALPHABET];  
  Code table[ALPHABET] = {0};    
  uint32_t uinque_symbols = 0;   
  struct stat permissions;       
  int br;                        
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt){
    case 'h':
      usage();    
      return (0); 
    case 'i':
      infile = open(optarg, O_RDONLY); 
      if (infile == -1){
        printf("Error opening file\n"); 
        return (1);                     
      }
      break; 
    case 'o':
      outfile = open(optarg, O_WRONLY | O_CREAT); 
      if (outfile == -1){
        printf("Error opening file\n"); 
        return (1);                     
      }
      break; 
    case 'v':
      print_stats_flag = true; 
      break;                   
    default:
      usage();    
      return (1); 
    }
  }

  for (int i = 0; i < ALPHABET; i++) {
    histogram[i] = 0; 
  }
  histogram[0] = 1; 
  histogram[1] = 1; 
  while ((br = read_bytes(infile, buf, BLOCK)) > 0){
    for (int i = 0; i < br; i++) {
      histogram[buf[i]]++; 
    }
  }
  root = build_tree(histogram); 
  build_codes(root, table);     
  for (int i = 0; i < ALPHABET; i++){
    if (histogram[i] > 0) {
      uinque_symbols += 1; 
    }
  }
  fstat(infile, &permissions);          
  fchmod(outfile, permissions.st_mode); 
  h.magic = MAGIC;                     
  h.permissions = permissions.st_mode; 
  h.tree_size = (3 * uinque_symbols) - 1;                
  h.file_size = permissions.st_size;                
  write_bytes(outfile, (uint8_t *)&h, sizeof(Header)); 
  dump_tree(outfile, root);                            
  lseek(infile, 0, SEEK_SET); 
  bytes_read = 0; 
  while ((br = read_bytes(infile, buf, BLOCK)) > 0) {
    for (int i = 0; i < br; i++) {
      write_code(outfile, &table[buf[i]]); 
    }
  }
  flush_codes(outfile); 
  if (print_stats_flag) {
    fprintf(stderr, "Uncompressed file size: %lu bytes\n", bytes_read);
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1 - ((double)bytes_written / bytes_read)));
  }
  delete_tree(&root); 
  close(outfile);     
  close(infile);      
  return 0;           
}
