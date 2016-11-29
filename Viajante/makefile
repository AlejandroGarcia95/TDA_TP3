CARGS= -Wall -pedantic -std=c99 -g


PRUEBAS= tsp.c
EXEC = TSPExec
GRAFO = grafo.o
TDAS = lista.o hash.o heap.o cola.o pila.o

VALGRIND= valgrind --track-origins=yes --leak-check=full
VALGRIND-V= $(VALGRIND) -v 

all: $(GRAFO) $(EXEC)
	echo "OK"

hash.o: hash.c hash.h
	$(CC) $(CARGS) hash.c -c

grafo.o: grafo.c grafo.h
	$(CC) $(CARGS) grafo.c -c

cola.o: cola.c cola.h
	$(CC) $(CARGS) cola.c -c

pila.o: pila.c pila.h
	$(CC) $(CARGS) pila.c -c

heap.o: heap.c heap.h
	$(CC) $(CARGS) heap.c -c

lista.o: lista.c lista.h
	$(CC) $(CARGS) lista.c -c

$(EXEC): $(GRAFO) $(PRUEBAS) $(TDAS) 
	gcc $(CARGS) $(GRAFO) $(TDAS) $(PRUEBAS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	$(VALGRIND) ./$(EXEC)

valgrind_V: $(EXEC)
	$(VALGRIND_V) ./$(EXEC)

rebuild: clean $(EXEC)

clean:
	rm -f *.o $(EXEC)
