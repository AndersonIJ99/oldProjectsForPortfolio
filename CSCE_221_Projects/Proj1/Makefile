all: Driver.o Llama.o
	g++ -Wall Driver.o Llama.o -o output.out

Llama.o: Llama.cpp Llama.h LlamaNode.h
	g++ -Wall Llama.cpp -c

Driver.o: Driver.cpp
	g++ -Wall Driver.cpp -c

val:
	valgrind ./output.out

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#

run:
	./output.out