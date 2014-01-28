#include"mineField.h"
using namespace std;

int main(int argc, char **argv)
{
	mineField obj(argc, argv);
	obj.findEscape();
	obj.output();
	return 0;
}
