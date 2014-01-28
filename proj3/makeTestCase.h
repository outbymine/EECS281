#ifndef MAKETESTCASE_H
#define MAKETESTCASE_H
#include<iostream>
#include<string>
#include<functional>
#include<vector>
#include<unordered_set>
#include<set>
#include<utility>
#include<unordered_map>
#include<map>
#include<random>
#include<algorithm>
#include<cmath>
#include<getopt.h>
#include<stdio.h>
#include<stdlib.h>
#include<cstdlib>
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
	multiset<long long> tradePrice; // store the price for each completed trading
	long long totalValue; // the total traded value
	long long NumSharesTraded; // the number of shares traded 
};

struct equityInforK // store the key of information about each order of the same specific equity
{
	long long ID; // the specific ID of an equity, specifiesc the order sequence
	long long price; // the price the client offers
};

struct equityInforV // store the value part in the map of information about each order of the same specific equity
{
	string clientName; 
	long long quantity; // the quantity the client offers to buy/sell
	long long duration; // the duaration of the trade periods
};

struct equityBuyComp // the functor to compare the items in priority_queue for buy orders
{
	bool operator()(const equityInforK& e1, const equityInforK& e2)
	{
		if(e1.price>e2.price)
			return true;
		if(e1.price==e2.price && e1.ID<e2.ID)
			return true;
		return false;
	}
};

struct equitySellComp // the functor to compare the items in priority_queue for sell orders
{
	bool operator()(const equityInforK& e1, const equityInforK& e2)
	{
		if(e1.price<e2.price)
			return true;
		if(e1.price==e2.price && e1.ID<e2.ID)
			return true;
		return false;
	}
};

class MM // Market Marker class
{
public:
	MM(int argc, char **argv); // all procedures are combined in this single function;
private:
	set<string> clientNames; // the names of all the clients that have ever placed order.
	set<string> equityNames; // the names of all equities that have ever appeared in orders 
	unordered_map<string, tttInfor> equityttt; // the ttt information, NOTE THAT ONLY WHEN A SELL ORDER COMES, THIS DATA STRUCTURE IS FILLED WITH THE EQUITY NAME AND ITS TTT
	unordered_map<string, MVInfor> equityMV; // the key is the unsorted equity names completed at least one TRADE (completed order) in during the entire process
	unordered_map<string, clientStat> client; // the key is the unsorted clients that ever appeared in the order, not necessarily traded
	unordered_map<string, map<equityInforK, equityInforV, equityBuyComp> > buyOrders; // buy order book
	unordered_map<string, map<equityInforK, equityInforV, equitySellComp> > sellOrders; // sell order book
	bool oVerbose; // whether output verbose
	bool oMedian; //whether output median
	bool oMidpoint; // whether output midpoint
	bool oTransfers; // whether output the tansfers option
	bool oVWAP; // whether output the VWAP
	vector<string> otttv; // ttt options, sequence the same as that appeard in command line arguments
	unordered_set<string> ottt; // the hash table used to contain the ttt options for equities, facilitate the search
	long long currentTime; // record the current timeStamp;
};
#endif 
