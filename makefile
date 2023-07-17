all:
	gcc -o exec *.c source/*.c source/Stack/*.c source/Queue/*.c
run:
	./exec <in/input >in/output
	diff in/output in/expected
clean:
	rm -r exec
