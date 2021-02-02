all: driver.o AugmentedBinarySearchTree.o Proj2Aux.o
	g++ -std=c++11 -Wall driver.o AugmentedBinarySearchTree.o Proj2Aux.o -o Proj2.out

AugmentedBinarySearchTree.o: AugmentedBinarySearchTree.cpp AugmentedBinarySearchTree.h BinarySearchTree.h Proj2Aux.h
	g++ -std=c++11 -Wall AugmentedBinarySearchTree.cpp -c

BinarySearchTree.o: BinarySearchTree.cpp BinarySearchTree.h
	g++ -std=c++11 -Wall BinarySearchTree.cpp -c

Proj2Aux.o: Proj2Aux.cpp Proj2Aux.h
	g++ -std=c++11 -Wall Proj2Aux.cpp -c

driver.o: driver.cpp
	g++ -std=c++11 -Wall driver.cpp -c

val:
	valgrind ./output.out

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
# make run FILE=input.txt COMMANDS=commands.txt
run:
	./Proj2.out $(FILE) $(COMMANDS) 2> errors.txt