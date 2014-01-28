PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64
mars:
	g++ -Wall -Werror mapinfor.cpp mode.cpp mars.cpp -O3 -std=c++11 -o mars
clean:
	rm -f mars *.o
