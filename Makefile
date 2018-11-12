all: interpolationspolynom


interpoloationspolynom: interpolationspolynom.c
	gcc -Wall -O1 --std=c99 -lm -o interpoloationspolynom interpolationspolynom.c

clean:
	rm interpolationspolynom
