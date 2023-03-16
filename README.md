# Huffman Coding

Huffman coding is a lossless data compression algorithm that assigns variable-length codes to symbols in a message or data stream. It was developed by David A. Huffman in 1952 while he was a student at MIT. The algorithm works by building a binary tree of nodes, where each leaf node represents a symbol in the message and each non-leaf node represents the sum of the frequencies of its children. The tree is built using a priority queue, with the symbols sorted by frequency.

## Files in Repository

1. node.c
2. node.h
3. stack.c
4. stack.h
5. Makefile
6. pq.c
7. pq.h
8. code.c
9. code.h
10. README.md
11. define.h
12. header.h
13. huffman.c
14. huffman.h
15. io.c
16. io.h
17. encode.c
18. decode.c


## Build 

### Steps:

1. Put all the files in one directory.
2. Open the directory in terminal.
3. Run command **make**.


## Run

### For encode:

```
./encode [-h] [-i infile] [-o outfile]
```

- `-h`: Shows help and usage
- `-i infile`: Input containing graph (default: stdin)
- `-o outfile`: Output of computed path (default: stdout)
- `-v`: Enable verbose printing.

### For decode:

```
./decode [-h] [-i infile] [-o outfile] 
```

- `-h`: Shows help and usage
- `-i infile`: Input containing graph (default: stdin)
- `-o outfile`: Output of computed path (default: stdout)
- `-v`: Enable verbose printing.


# Note 

If you are a student and if are taking a look at my repository for your assignment, any piece of code that your copy from this repository is completely your responsibility. I should not be held repsonsible for any academic misconduct.