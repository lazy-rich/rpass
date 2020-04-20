rpass: main.c
	cc -Wall -Wextra main.c -o rpass -lbsd
clean:
	rm -f rpass
install: rpass
	install -m 0700 rpass ~/bin
