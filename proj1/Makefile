PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

CC=g++
OBJS=proj1.o stationSpace.o
CFLAGS=-Wall -Werror -O3
STDFLAG=-std=c++11
#CFLAGS=-g

proj1: $(OBJS)
	${CC} ${CFLAGS} $(OBJS) -o proj1 ${STDFLAG}
stationSpace.o: stationSpace.cpp stationSpace.h
	${CC} ${CFLAGS} -c stationSpace.cpp ${STDFLAG}
proj1.o: proj1.cpp stationSpace.h
	${CC} ${CFLAGS} -c proj1.cpp ${STDFLAG}
clean:
	rm -f proj1 *.o

