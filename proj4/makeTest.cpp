#include<random>
#include<iostream>
#include<climits>
using namespace std;

int main()
{
	mt19937 gen(1890224);
	uniform_int_distribution<> value(-10, 10);
	int time=24;
	cout<<time;
	for(int i=0; i<time; i++)
	{
		cout<<endl;
		cout<<value(gen)<<" "<<value(gen)<<" "<<0;
	}	
	return 0;
}
