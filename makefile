all:
	gcc -o exec *.c
run:
	./exec
clean:
	rm -r exec
