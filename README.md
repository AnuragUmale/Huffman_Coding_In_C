# Huffman Coding

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


## Build and run encode.c and decode.c with help of  node.c, stack.c, pq.c, code.c, io.c, and huffman.c 

### Steps:
1. Put all the files in one directory.
2. Open the directory in terminal.
3. Run command **make**.

# For encode:
```
./encode [-h] [-i infile] [-o outfile]
```

- `-h`: Shows help and usage
- `-i infile`: Input containing graph (default: stdin)
- `-o outfile`: Output of computed path (default: stdout)
- `-v`: Enable verbose printing.

# For decode:
```
./decode [-h] [-i infile] [-o outfile] 
```

- `-h`: Shows help and usage
- `-i infile`: Input containing graph (default: stdin)
- `-o outfile`: Output of computed path (default: stdout)
- `-v`: Enable verbose printing.
