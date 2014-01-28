#ifndef STATIONSPACE_H
#define STATIONSPACE_H
#include<getopt.h>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<vector>
#include<string>
using namespace std;

//command line arguments
const struct option long_options[]=
{
	{"help",0,0,'h'},
	{"stack",0,0,'s'},
	{"queue",0,0,'q'},
	{"output",1,0,'o'},
	{0,0,0,0},
};

//the location details of each level
struct location
{
	int row;
	int col;
	int levelNum;
	char character;
};

//the station struct
struct station
{
	string inMode;
	string outMode;
	char routeScheme;
	int level;
	int dim;
	vector<location> routeMap;
	//this recoreds the details of the stationSpace, including the position and flag of each element when calling the constructor. 
	vector<location> escape;
	//In the function "findEscape", this vector is used to keep track of the passed positions and from which direction the element is reached.
};

//the stationspace class
class stationSpace
{
public:
	stationSpace();
	stationSpace(int argc, char **argv);
	~stationSpace();
	location findEscape();
	void output(location &H);//output to the output file
	friend void help();//print out the help function
	friend bool isLegalChar(char character);//check whether the input character is legal
	int directBack(const location &L_route);
private:
	station  SS;
	location start;
};
#endif
