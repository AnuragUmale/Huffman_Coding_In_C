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
#include <stdbool.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hi:o:v"

void usage(void){
  fprintf(stderr, "SYNOPSIS\n");
  fprintf(stderr, "  A Huffman decoder.\n");
  fprintf(stderr, "  Decompresses a file using the Huffman coding algorithm.\n\n");
  fprintf(stderr, "USAGE\n");
  fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n\n");
  fprintf(stderr, "OPTIONS\n");
  fprintf(stderr, "  -h             Program usage and help.\n");
  fprintf(stderr, "  -v             Print compression statistics.\n");
  fprintf(stderr, "  -i infile      Input file to decompress.\n");
  fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
}

int main(int argc, char **argv){
  int opt = 0;
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  bool print_stats_flag = false;
  struct stat perms;
  Header h;
  Node *root;
  Node *node;
  uint8_t bit;
  uint32_t count = 0;
  while ((opt = getopt(argc, argv, OPTIONS)) != -1){
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
  read_bytes(infile, (uint8_t *)&h, sizeof(Header));
  if (h.magic != MAGIC){
    fprintf(stderr, "Invalid magic number\n");
    return (1);
  }
  fstat(infile, &perms);
  fchmod(outfile, h.permissions);
  uint8_t dump_code[h.tree_size];
  read_bytes(infile, dump_code, h.tree_size);
  root = rebuild_tree(h.tree_size, dump_code);
  node = root;
  while (count < h.file_size && read_bit(infile, &bit)){
    if (node->right == NULL && node->left == NULL) {
      write_bytes(outfile, &node->symbol, sizeof(uint8_t));
      node = root;
      count++;
    }
    if (bit == 0){
      node = node->left;
    }
    else if (bit == 1){
      node = node->right;
    }
  }
  if (print_stats_flag){
    fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_read);
    fprintf(stderr, "Decompressed file size: %" PRIu64 " bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1 - ((double)bytes_read / bytes_written)));
  }
  delete_tree(&root);
  close(infile);
  close(outfile);
}
