PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64
market:
	g++ -Wall -Werror marketMaker.cpp market.cpp -O3 -std=c++11 -o market
clean:
	rm -f market *.o
