all: Driver.o Heap.o PinHit.o MaxHeap.o MinHeap.o
	g++ -std=c++11 -Wall Driver.o Heap.o MinHeap.o MaxHeap.o PinHit.o -o Proj4.out

Heap.o: Heap.cpp Heap.h
	g++ -std=c++11 -Wall Heap.cpp -c

MaxHeap.o: MaxHeap.cpp MaxHeap.h
	g++ -std=c++11 -Wall MaxHeap.cpp -c

MinHeap.o: MinHeap.cpp MinHeap.h
	g++ -std=c++11 -Wall MinHeap.cpp -c

PinHit.o: PinHit.cpp PinHit.h
	g++ -std=c++11 -Wall PinHit.cpp -c

Driver.o: Driver.cpp
	g++ -std=c++11 -Wall Driver.cpp -c

val:
	valgrind ./Proj4.out

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
# make run FILE=input.txt TYPE=--min
run:
	./Proj4.out $(FILE) $(TYPE)