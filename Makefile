test:
	gcc -o test test.c opcode.c -g
clean:
	rm -f test *.o
