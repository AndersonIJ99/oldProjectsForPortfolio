all: driver.o Node.o Util.o
	g++ -std=c++11 -Wall driver.o Node.o Util.o -o Proj3.out

# HashedSplays.o: HashedSplays.h
# 	g++ -std=c++11 -Wall HashedSplays.h -c

Node.o: Node.cpp
	g++ -std=c++11 -Wall Node.cpp -c

Util.o: Util.cpp
	g++ -std=c++11 -Wall Util.cpp -c

driver.o: driver.cpp
	g++ -std=c++11 -Wall driver.cpp -c

val:
	valgrind ./Proj3.out

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
# make run FILE=input.txt
run:
	./Proj3.out $(FILE)