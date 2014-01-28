#ifndef MARKETMAKER_H
#define MARKETMAKER_H
#include<iostream>
#include<string>
#include<functional>
#include<vector>
#include<unordered_set>
#include<set>
#include<utility>
#include<unordered_map>
#include<map>
#include<queue>
#include<random>
#include<algorithm>
#include<cctype>
#include<getopt.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

//command line arguments
const struct option long_options[]=
{
	{"help",0,0,'h'},
	{"verbose",0,0,'v'},
	{"median",0,0,'m'},
	{"midpoint",0,0,'p'},
	{"transfers",0,0,'t'},
	{"VWAP",0,0,'w'},
	{"ttt",1,0,'g'},
};

//the statistics about each client, prepared for the -t and -g
struct clientStat
{
	long long bought; // for the -t/transfers
	long long sold; //..
	long long netTransfer; //..
};

// the necessary variable in the computing of the ttt is stored in this struct
struct tttInfor
{
	long long lowestBuyT; // until current point, the time to get lowest price to buy the equity
	long long lowestBuyP; // until current point, the lowest price to buy the equity
	long long bestBuyT; // until current point, if the largest profit is made, the best buy time, note that this may not equal to the loewestBuyT
	long long bestSellT; // note that this is the same with the highest sell since sell is executed after buy action;
	long long largestDiff; // the largest possible price difference that ttt can get	
};

// store the information for the computation of median and VWAP,namely, the relevant trading record
// also store the information necessary related to ttt
struct MVInfor
{
	priority_queue<long long, vector<long long>, std::less<long long> > lowPriceHeap; // This is a priority_queue that stores the larger-half value of the price, the top element is the smallest value among these 
	priority_queue<long long, vector<long long>, std::greater<long long> > highPriceHeap; // This is a priority_queue that stores the smaller-half value of the price, the top element is the largest value among these 
	long long median; // the median, updated when a trade is completed each time
	long long totalValue; // the total traded value
	long long NumSharesTraded; // the number of shares traded 
};

struct equityInfor // store the key of information about each order of the same specific equity
{
	long long timeStamp;
	long long ID; // the specific ID of an equity, specifiesc the order sequence
	long long price; // the price the client offers
	string clientName; 
	long long quantity; // the quantity the client offers to buy/sell
	long long duration; // the duaration of the trade periods
};

struct equityBuyComp // the functor to compare the items in priority_queue for buy orders
{
	bool operator()(const equityInfor& e1, const equityInfor& e2)
	{
		if(e1.price<e2.price)
			return true;
		if(e1.price==e2.price && e1.ID>e2.ID)
			return true;
		return false;
	}
};

struct equitySellComp // the functor to compare the items in priority_queue for sell orders
{
	bool operator()(const equityInfor& e1, const equityInfor& e2)
	{
		if(e1.price>e2.price)
			return true;
		if(e1.price==e2.price && e1.ID>e2.ID)
			return true;
		return false;
	}
};

class MM // Market Marker class
{
public:
	MM(int argc, char **argv); // read in the command line arguments
	void readLine1(const string line, const string delimiter, string & key, bool & isBuy, equityInfor& tmp, long long & idNum); // read in the first line
	void processTime(const equityInfor& tmp); // deal with step 2 in the market logic, but do not change the duration or pop any element in the data structure
	void matchAndFill(const string key, const bool isBuy, equityInfor& tmp); // match the successful trade and put the remaining current order to the order books
	void finalOutput();
	set<string> clientNames; // the names of all the clients that have ever placed order.
	set<string> equityNames; // the names of all equities that have ever appeared in orders 
	unordered_map<string, tttInfor> equityttt; // the ttt information, NOTE THAT ONLY WHEN A SELL ORDER COMES, THIS DATA STRUCTURE IS FILLED WITH THE EQUITY NAME AND ITS TTT
	unordered_map<string, MVInfor> equityMV; // the key is the unsorted equity names completed at least one TRADE (completed order) in during the entire process
	unordered_map<string, clientStat> client; // the key is the unsorted clients that ever appeared in the order, not necessarily traded
	unordered_map<string, priority_queue<equityInfor, vector<equityInfor>, equityBuyComp> > buyOrders; // buy order book
	unordered_map<string, priority_queue<equityInfor, vector<equityInfor>, equitySellComp> > sellOrders; // sell order book
	bool oVerbose; // whether output verbose
	bool oMedian; //whether output median
	bool oMidpoint; // whether output midpoint
	bool oTransfers; // whether output the tansfers option
	bool oVWAP; // whether output the VWAP
	vector<string> otttv; // ttt options, sequence the same as that appeard in command line arguments
	unordered_set<string> ottt; // the hash table used to contain the ttt options for equities, facilitate the search
	long long currentTime; // record the current timeStamp;
	long long commissionFee; // the total commission fee
};

void readLine(const string line, const string delimiter, string & key, bool & isBuy, equityInfor& tmp, long long & idNum, const MM & MM1); // read in each line

bool isInt(const string& s); // check whether the string is composed only of digit values

bool isValid(const string::size_type pos, const string::size_type lasPos); // check that the input file does not end prematurely

#endif 
