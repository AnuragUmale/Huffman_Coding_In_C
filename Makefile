CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -gdwarf-4


all: encode decode

encode: encode.o node.o stack.o code.o pq.o io.o huffman.o

decode: decode.o node.o stack.o code.o pq.o io.o huffman.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode *.o

spotless:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[c]