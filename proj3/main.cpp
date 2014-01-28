#include<random>
#include<iostream>
using namespace std;

int main()
{
	mt19937 gen(10000);
	uniform_int_distribution<int> aaa(0, 10);
	for(int i=0; i<10; i++)
	{
		cout<<aaa(gen)<<endl;
	}
	return 0;
}
