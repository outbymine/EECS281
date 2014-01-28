#include"stationSpace.h"
int main(int argc, char **argv)
{

	stationSpace proj1(argc,argv);//build the route map detail of the stationSpace
	location H=proj1.findEscape();//find the way out
	proj1.output(H);//print the output
	return 0;
}
