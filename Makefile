rpass: rand.o main.c
	cc -Wall -Wextra main.c rand.o -o rpass
rand.o:
	cc -Wall -Wextra -c rand.c -o rand.o
clean:
	rm -f rpass rand.o
install: rpass
	install -m 0700 rpass ~/bin
