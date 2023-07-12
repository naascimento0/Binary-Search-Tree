all:
	gcc -o exec *.c source/*.c
run:
	./exec <in/input >in/output
	diff in/output in/expected
clean:
	rm -r exec
