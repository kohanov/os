all : makeCat
cat.o : cat.c
	gcc -c cat.c -o cat.o
makeCat : cat.o
	gcc -s cat.o -o cat
clean :
	rm -f cat cat.o
