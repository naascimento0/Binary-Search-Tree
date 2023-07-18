all:
	gcc -o exec *.c source/*.c source/Stack/*.c source/Queue/*.c -Wall
run:
	./exec <in/input >in/output
	diff in/output in/expected

valgrind:
	valgrind --leak-check=full ./exec <in/input >in/output
diff:
	diff in/output in/expected
clean:
	rm -r exec
