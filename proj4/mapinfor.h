#ifndef MAPINFOR_H
#define MAPINFOR_H
#include<vector>
#include<cmath>
#include<string>
#include<cstdlib>
#include<iostream>
#include<getopt.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

//command line arguments
const struct option long_options[]=
{
	{"help",0,0,'h'},
	{"mode",1,0,'m'},
};

//the struct of each location information
struct location
{
	int x;
	int y;
	int height;
};

//the adjacency_matrix representation for the computation of MST
class adjMat
{
public:
	adjMat(int argc, char **argv); // read the input base map

	vector<location> mapLocation; // the location information for each base
	vector<vector<int> > adjMatrix; // the adjacent matrix
	int baseNum;
	string mode; // project output mode
};

int computeDist(const location& l1, const location& l2); // compute the Manhatten distance

#endif
