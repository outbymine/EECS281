#ifndef MINEFIELD_H
#define MINEFIELD_H
#include<getopt.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<fstream>
#include<random>
#include<algorithm>
#include<deque>
#include"binary_heap.h"
#include"poorman_heap.h"
#include"sorted_heap.h"
#include"pairing_heap.h"
#include"eecs281heap.h"
using namespace std; //the names not allowed in std are not used in this project

//command line arguments
const struct option long_options[]=
{
	{"help",0,0,'h'},
	{"verbose",1,0,'v'},
	{"container",1,0,'c'},
	{0,0,0,0},
};

struct information //store the imformation, including the x, y axis and the dynamite cost to blast away
{
	int x;
	int y;
	int dynaQuant; // the quantity of dynamite needed to blast away the tile, if the value is zero,
                   // then that is a slot to go through 
	bool flag; // whether this slot is already in the heap or can step in, which means blasted or initially zero.
};

struct inforComp //compare whether the first element is smaller than the second one
{
	bool operator()(const information* I1, const information* I2) const
	{
		if(I1->dynaQuant<I2->dynaQuant)
			return true;
		if(I1->dynaQuant==I2->dynaQuant && I1->x<I2->x)
			return true;
		if(I1->dynaQuant==I2->dynaQuant && I1->x==I2->x && I1->y<I2->y)
			return true;
		return false;
	}
};

struct inforComp1
{
	bool operator()(const information* I1, const information* I2) const
	{
		if(I1->dynaQuant>I2->dynaQuant)
			return true;
		if(I1->dynaQuant==I2->dynaQuant && I1->x>I2->x)
			return true;
		if(I1->dynaQuant==I2->dynaQuant && I1->x==I2->x && I1->y>I2->y)
			return true;
		return false;
	}
};

class mineField
{
public:
	mineField();
	mineField(int argc, char **argv);
	~mineField();
	void findEscape();
	void output();
private:
	string inMode; //the input mode
	string heapType; //the heap type
	vector<information> routeMap; //the elements in this vector is the quantity of dynamite need to blast away rubber
	vector<information*> blasted; // the information of the rubbers blasted
	int blastedNum; // the total number of blasted rubbers
	int dynamite; //the amount of the total dynamite of the miner
	int size; //the size of the mineField
	information start; //the starting location in the mineField
	int vNum; //the number of the verbose output
	bool whetherEsc; //whether the escape was successful 
};





#endif
