all: Driver.o
	g++ -Wall -std=c++11 -o output.out Driver.o

Driver.o: Driver.cpp AmazonTruck.h Address.h Dimension.h Liquid.h Package.h
	g++ -Wall -std=c++11 -c Driver.cpp

# -f removes any error messages if the file is not present
clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#

run:
	./output.out