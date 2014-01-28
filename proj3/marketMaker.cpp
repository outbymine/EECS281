#include "marketMaker.h"
using namespace std;

MM::MM(int argc, char **argv)
{
	oVerbose=false;
	oMedian=false; 
	oMidpoint=false; 
	oTransfers=false;
	oVWAP=false;
	int c; 
	while((c=getopt_long(argc, argv, "hvmptwg:", long_options, NULL))!=-1)
	{
		switch(c)
		{
		case 'h':
			cout<<"this program is meant to immitate the market marker"<<endl;
			cout<<"v means output the verbose information"<<endl;
			cout<<"m means output the median information"<<endl;
			cout<<"p means the midpoint information"<<endl;
			cout<<"t means the transfer option"<<endl;
			cout<<"w means the VWAP information"<<endl;
			cout<<"g means the ttt options available"<<endl;
			exit(0);
		case 'v':
			oVerbose=true;
			break;
		case 'm':
			oMedian=true;
			break;
		case 'p':
			oMidpoint=true;
			break;
		case 't':
			oTransfers=true;
			break;
		case 'w':
			oVWAP=true;
			break;
		case 'g':
			otttv.push_back(string(optarg));
			ottt.insert(string(optarg));
			break;
		case '?':
			cerr<<"unknown command line arguement, error"<<endl;
			exit(1);
		default:
			abort();
		}
	}
}

bool isInt(const string& s)
{
	string::const_iterator it=s.begin();
	while(it!=s.end() && isdigit(*it))
		it++;
	return it==s.end();
}

bool isValid(const string::size_type pos, const string::size_type lasPos) 
{
	if(pos==string::npos || lasPos==string::npos)
	{
		//cerr<<"error, the input file ends prematurely";
		exit(1);
		return false;
	}
	else
		return true;
}

void MM::readLine1(const string line, const string delimiter, string & key, bool & isBuy, equityInfor& tmp, long long & idNum)
{
	commissionFee=0;
	string::size_type lasPos, pos;
	lasPos=line.find_first_not_of(delimiter, 0);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(!isInt(line.substr(lasPos, pos-lasPos)))
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.timeStamp=stoll(line.substr(lasPos, pos-lasPos)); 
	currentTime=tmp.timeStamp; // set the current timeStamp to the timeStamp of the first order
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	tmp.clientName=line.substr(lasPos, pos-lasPos);
	idNum++;
	tmp.ID=idNum;// the id number used to decide the order after the tie on the price match
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(line.substr(lasPos, pos-lasPos)=="BUY")
		isBuy=true;
	else
		isBuy=false;
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	key=line.substr(lasPos, pos-lasPos);
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(line[lasPos]!='$')
	{
		//cerr<<"error, there should be a $ sign before the price"<<endl;
		exit(1);
	}
	if(!isInt(line.substr(lasPos+1, pos-lasPos-1)))
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.price=stoll(line.substr(lasPos+1, pos-lasPos-1));
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(line[lasPos]!='#')
	{
		//cerr<<"error, there should be a $ sign before the price"<<endl;
		exit(1);
	}
	if(!isInt(line.substr(lasPos+1, pos-lasPos-1)))
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.quantity=stoll(line.substr(lasPos+1,pos-lasPos-1));
	lasPos=line.find_first_not_of(delimiter, pos);
	isValid(pos, lasPos);
	if(!isInt(line.substr(lasPos)) && line.substr(lasPos)!="-1")
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.duration=stoll(line.substr(lasPos));
	// insert the euqity name in a sorted container of set used to record all equity names ever appeared in order
	equityNames.insert(key);
	// insert the equity detail information into the buy order books or sell order books, also update necessary informations about ttt
	if(isBuy==true && tmp.duration!=0)
	{
		buyOrders[key].push(tmp);
	}
	//if the client offers to sell, then set the best buy time to this and store the value of this possible price
	if(isBuy==false && tmp.duration!=0)
	{
		sellOrders[key].push(tmp);
		if(ottt.find(key)!=ottt.end())
		{
			equityttt[key].bestBuyT=currentTime;
			equityttt[key].lowestBuyP=tmp.price;
			equityttt[key].lowestBuyT=currentTime;
			equityttt[key].bestSellT=-1; // This means that no possible output in ttt equity yet, or no sell after buy is possible
		}
	}
	// insert the new appeared client name into the corresponding sorted set
	clientNames.insert(tmp.clientName);
}

void readLine(const string line, const string delimiter, string & key, bool & isBuy, equityInfor& tmp, long long & idNum, const MM & MM1)
{
	string::size_type lasPos, pos;
	lasPos=line.find_first_not_of(delimiter, 0);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(!isInt(line.substr(lasPos, pos-lasPos)))
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.timeStamp=stoll(line.substr(lasPos, pos-lasPos));
	if(tmp.timeStamp<MM1.currentTime)
	{
		//cerr<<"error, orders should arrive in timeStamp order";
		exit(1);
	}
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	tmp.clientName=line.substr(lasPos, pos-lasPos);
	idNum++;
	tmp.ID=idNum;// the id number used to decide the order after the tie on the price match
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(line.substr(lasPos, pos-lasPos)=="BUY")
		isBuy=true;
	else
		isBuy=false;
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	key=line.substr(lasPos, pos-lasPos);
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(line[lasPos]!='$')
	{
		//cerr<<"error, there should be a $ sign before the price"<<endl;
		exit(1);
	}
	if(!isInt(line.substr(lasPos+1, pos-lasPos-1)))
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.price=stoll(line.substr(lasPos+1, pos-lasPos-1));
	lasPos=line.find_first_not_of(delimiter, pos);
	pos=line.find_first_of(delimiter, lasPos);
	isValid(pos, lasPos);
	if(line[lasPos]!='#')
	{
		//cerr<<"error, there should be a $ sign before the price"<<endl;
		exit(1);
	}
	if(!isInt(line.substr(lasPos+1, pos-lasPos-1)))
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.quantity=stoll(line.substr(lasPos+1,pos-lasPos-1));
	lasPos=line.find_first_not_of(delimiter, pos);
	isValid(pos, lasPos);
	if(!isInt(line.substr(lasPos)) && line.substr(lasPos)!="-1")
	{
		//cerr<<"error, not reading in integer-type value in integer_typed variable";
		exit(1);
	}
	tmp.duration=stoll(line.substr(lasPos));
}

void MM::processTime(const equityInfor& tmp)
{
	if(currentTime!=tmp.timeStamp)
	{
		set<string>::iterator itrEquityNames;
		unordered_map<string, priority_queue<equityInfor, vector<equityInfor>, equityBuyComp> >::iterator itrBuyOrders;
		unordered_map<string, priority_queue<equityInfor, vector<equityInfor>, equitySellComp> >::iterator itrSellOrders; 
		multiset<long long>::iterator itrTradePrice;
		// Median output
		if(oMedian==true) 
		{
			//traverse the corrsponding set for stored equity names
			for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++)
			{
				if(equityMV.find(*itrEquityNames)!=equityMV.end()) // check whether trade happened for this kind of equity
				{
					//getting the median element is O(1) time in this case
					//cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<equityMV[*itrEquityNames].median<<endl;
					printf("Median match price of %s at time %lld is $%lld\n", (*itrEquityNames).c_str(), currentTime, equityMV[*itrEquityNames].median);
				}
			}
		}
		//midpoint output
		if(oMidpoint==true)
		{
			long long midpointPrice;
			//traverse the corrsponding set for stored equity names
			for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++) 
			{
				if(!buyOrders[*itrEquityNames].empty() && !sellOrders[*itrEquityNames].empty())
				{
					midpointPrice=(buyOrders[*itrEquityNames].top().price+sellOrders[*itrEquityNames].top().price)/2;
					//cout<<"Midpoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<midpointPrice<<endl;
					printf("Midpoint of %s at time %lld is $%llu\n", (*itrEquityNames).c_str(), currentTime, midpointPrice);
				}			
				else
				{
					//cout<<"Midpoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is undefined"<<endl;
					printf("Midpoint of %s at time %lld is undefined\n", (*itrEquityNames).c_str(), currentTime);
				}
			}
		}
		currentTime=tmp.timeStamp;
	}
}

void MM::matchAndFill(const string key, const bool isBuy, equityInfor& tmp)
{
	equityNames.insert(key);
	clientNames.insert(tmp.clientName);
	long long buyCommission;
	long long sellCommission;
	// declare the relevant iterators corresponding to different data structures
	set<string>::iterator itrEquityNames;
	set<string>::iterator itrClientNames;
	unordered_map<string, tttInfor>::iterator itrEquityttt;
	unordered_map<string, MVInfor>::iterator itrEquityMV;
	unordered_map<string, clientStat>::iterator itrClients;
	unordered_map<string, priority_queue<equityInfor, vector<equityInfor>, equityBuyComp> >::iterator itrBuyOrders;
	unordered_map<string, priority_queue<equityInfor, vector<equityInfor>, equitySellComp> >::iterator itrSellOrders; 
	multiset<long long>::iterator itrTradePrice;
	if(isBuy==true) // if the newly added order is a buy order
	{
		//update informations about ttt, since this is only a sell oppotunity, the corresponding element in equityttt data structure won't be created if not already created
		if(equityttt.find(key)!=equityttt.end()) // check two conditions: whether the equity is ttt and whether a buying is possible before this time, they can be combined into this check
		{
			if(equityttt[key].bestSellT==-1)// no VALID sell oppotunity before this time point
			{
				equityttt[key].largestDiff=tmp.price-equityttt[key].lowestBuyP;
				equityttt[key].bestSellT=currentTime;
			}
			else if(equityttt[key].largestDiff<tmp.price-equityttt[key].lowestBuyP)
			{
				equityttt[key].bestSellT=currentTime;
				equityttt[key].largestDiff=tmp.price-equityttt[key].lowestBuyP;
				equityttt[key].bestBuyT=equityttt[key].lowestBuyT;
			}
		}
		// seeking trade oppotunities
		if(sellOrders.find(key)!=sellOrders.end()) // if there is at least one offer of such equity EVER appeard for sell in the sell order book
		{
			// pop the expired orders in the front of the priority_queue for the order match and the computation of midpoint in step 2, note that the we need to keep popping the first element until no the
			// first one is an alive one 
			while(!sellOrders[key].empty())
			{
				while(!sellOrders[key].empty())
				{
					if((currentTime-sellOrders[key].top().timeStamp)>=sellOrders[key].top().duration && sellOrders[key].top().duration!=-1)
						sellOrders[key].pop();
					else
						break;
				}
				if(!sellOrders[key].empty() && tmp.price>=sellOrders[key].top().price && tmp.quantity!=0) // if this condition is satisfied, then the trade can be executed
				{
					long long tradeAmount=min(tmp.quantity, sellOrders[key].top().quantity); // compute the trade amount in this successful trade
					if(oVerbose==true)
					{
						//cout<<tmp.clientName<<" purchased "<<tradeAmount<<" shares of "<<key<<" from "<<sellOrders[key].top().clientName<<" for $"<<sellOrders[key].top().price<<"/share"<<endl;
						printf("%s purchased %lld shares of %s from %s for $%lld/share\n", tmp.clientName.c_str(), tradeAmount, key.c_str(), sellOrders[key].top().clientName.c_str(), sellOrders[key].top().price);
					}
					buyCommission=tradeAmount*sellOrders[key].top().price/100;
					sellCommission=buyCommission;
					commissionFee=commissionFee+buyCommission+sellCommission;
					// adjust to class members after a trade is made
					if(equityMV.find(key)==equityMV.end())// if this equity is curretly not a key in equityMV, also this is the base case in computing the median
					{
						equityMV[key].NumSharesTraded=0;
						equityMV[key].totalValue=0;
						equityMV[key].highPriceHeap.push(sellOrders[key].top().price);
						equityMV[key].median=sellOrders[key].top().price;
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*sellOrders[key].top().price;
					}
					else
					{
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*sellOrders[key].top().price;
						if(sellOrders[key].top().price<=equityMV[key].median)
						{
							if(equityMV[key].lowPriceHeap.size()<=equityMV[key].highPriceHeap.size())
								equityMV[key].lowPriceHeap.push(sellOrders[key].top().price);
							else
							{
								equityMV[key].highPriceHeap.push(equityMV[key].lowPriceHeap.top());
								equityMV[key].lowPriceHeap.pop();
								equityMV[key].lowPriceHeap.push(sellOrders[key].top().price);
							}
						}
						else
						{
							if(equityMV[key].highPriceHeap.size()<=equityMV[key].lowPriceHeap.size())
								equityMV[key].highPriceHeap.push(sellOrders[key].top().price);
							else
							{
								equityMV[key].lowPriceHeap.push(equityMV[key].highPriceHeap.top());
								equityMV[key].highPriceHeap.pop();
								equityMV[key].highPriceHeap.push(sellOrders[key].top().price);
							}
							
						}
						if(equityMV[key].lowPriceHeap.size()==equityMV[key].highPriceHeap.size())
							equityMV[key].median=(equityMV[key].lowPriceHeap.top()+equityMV[key].highPriceHeap.top())/2;
						else if(equityMV[key].lowPriceHeap.size()>equityMV[key].highPriceHeap.size())
							equityMV[key].median=equityMV[key].lowPriceHeap.top();
						else
							equityMV[key].median=equityMV[key].highPriceHeap.top();							
					}
					if(client.find(tmp.clientName)==client.end())
					{
						client[tmp.clientName].bought=0;
						client[tmp.clientName].sold=0;
						client[tmp.clientName].netTransfer=0;
					}
					if(client.find(sellOrders[key].top().clientName)==client.end())
					{
						client[sellOrders[key].top().clientName].bought=0;
						client[sellOrders[key].top().clientName].sold=0;
						client[sellOrders[key].top().clientName].netTransfer=0;
					}							
					client[tmp.clientName].bought+=tradeAmount;
					client[sellOrders[key].top().clientName].sold+=tradeAmount;
					client[tmp.clientName].netTransfer-=tradeAmount*sellOrders[key].top().price;
					client[sellOrders[key].top().clientName].netTransfer+=tradeAmount*sellOrders[key].top().price;
					if(tradeAmount==tmp.quantity)
					{
						tmp.quantity=0;
						equityInfor tmpTop=sellOrders[key].top();
						sellOrders[key].pop();
						tmpTop.quantity-=tradeAmount;
						if(tmpTop.quantity!=0)
						{
							sellOrders[key].push(tmpTop);
							break;
						}
					}
					else
					{
						tmp.quantity-=tradeAmount;
						//pop the top sell order of this equity from the data structure
						sellOrders[key].pop();
					}
				}
				else
					break;
			}
		}
		if(tmp.quantity!=0 && tmp.duration!=0)
		buyOrders[key].push(tmp);
		// deal with the buy order book, keep poping those expired orders in the front until the top element in the priority_queue is an active one
		if(buyOrders.find(key)!=buyOrders.end())
		{
			while(!buyOrders[key].empty() && (currentTime-buyOrders[key].top().timeStamp)>=buyOrders[key].top().duration && buyOrders[key].top().duration!=-1)
				buyOrders[key].pop();
		}
	}
	else // if the order is a sell order
	{
		//update informations about ttt, since this is a buy oppotunity, the corresponding element in equityttt data structure will be created if not already created
		if(ottt.find(key)!=ottt.end()) // check whether this equity is a ttt
		{
			if(equityttt.find(key)==equityttt.end()) // if there is not yet a buy oppotunity for the ttt of this equity
			{
				equityttt[key].bestBuyT=currentTime;
				equityttt[key].lowestBuyT=currentTime;
				equityttt[key].bestSellT=-1;
				equityttt[key].lowestBuyP=tmp.price;
			}
			else if(equityttt[key].lowestBuyP>tmp.price) // if there was already a buy oppotunity and the newly price of the oppotunity in currentTime is lower
			{
				equityttt[key].lowestBuyT=currentTime;
				equityttt[key].lowestBuyP=tmp.price;
				if(equityttt[key].bestSellT==-1) // if there is no sell oppotunity after the previous buy oppotunity yet 
					equityttt[key].bestBuyT=currentTime;
			}
		}
		// seeking trade oppotunities
		if(buyOrders.find(key)!=buyOrders.end()) // if there is at least one offer of such equity EVER appeard for sell in the sell order book
		{
			// pop the expired orders in the front of the priority_queue for the order match and the computation of midpoint in step 2, note that the we need to keep popping the first element until no the
			// first one is an alive one 
			while(!buyOrders[key].empty())
			{
				while(!buyOrders[key].empty())
				{
					if((currentTime-buyOrders[key].top().timeStamp)>=buyOrders[key].top().duration && buyOrders[key].top().duration!=-1)
						buyOrders[key].pop();
					else
						break;
				}
				if(!buyOrders[key].empty() && buyOrders[key].top().price>=tmp.price && tmp.quantity!=0) // if this condition is satisfied, then the trade is executed
				{
					long long tradeAmount=min(tmp.quantity, buyOrders[key].top().quantity); // compute the trade amount in this successful trade
					if(oVerbose==true)
					{
						//cout<<buyOrders[key].top().clientName<<" purchased "<<tradeAmount<<" shares of "<<key<<" from "<<tmp.clientName<<" for $"<<buyOrders[key].top().price<<"/share"<<endl;
						printf("%s purchased %lld shares of %s from %s for $%lld/share\n", buyOrders[key].top().clientName.c_str(), tradeAmount, key.c_str(), tmp.clientName.c_str(), buyOrders[key].top().price);
					}
					buyCommission=tradeAmount*buyOrders[key].top().price/100;
					sellCommission=buyCommission;
					commissionFee=commissionFee+buyCommission+sellCommission;
					// adjust to class members after a trade is made
					if(equityMV.find(key)==equityMV.end())// if this equity is curretly not a key in equityMV, also this is the base case in computing the median
					{
						equityMV[key].NumSharesTraded=0;
						equityMV[key].totalValue=0;
						equityMV[key].highPriceHeap.push(buyOrders[key].top().price);
						equityMV[key].median=buyOrders[key].top().price;
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*buyOrders[key].top().price;
					}
					else
					{
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*buyOrders[key].top().price;
						if(buyOrders[key].top().price<=equityMV[key].median)
						{
							if(equityMV[key].lowPriceHeap.size()<=equityMV[key].highPriceHeap.size())
								equityMV[key].lowPriceHeap.push(buyOrders[key].top().price);
							else
							{
								equityMV[key].highPriceHeap.push(equityMV[key].lowPriceHeap.top());
								equityMV[key].lowPriceHeap.pop();
								equityMV[key].lowPriceHeap.push(buyOrders[key].top().price);
							}
						}
						else
						{
							if(equityMV[key].highPriceHeap.size()<=equityMV[key].lowPriceHeap.size())
								equityMV[key].highPriceHeap.push(buyOrders[key].top().price);
							else
							{
								equityMV[key].lowPriceHeap.push(equityMV[key].highPriceHeap.top());
								equityMV[key].highPriceHeap.pop();
								equityMV[key].highPriceHeap.push(buyOrders[key].top().price);
							}
							
						}
						if(equityMV[key].lowPriceHeap.size()==equityMV[key].highPriceHeap.size())
							equityMV[key].median=(equityMV[key].lowPriceHeap.top()+equityMV[key].highPriceHeap.top())/2;
						else if(equityMV[key].lowPriceHeap.size()>equityMV[key].highPriceHeap.size())
							equityMV[key].median=equityMV[key].lowPriceHeap.top();
						else
							equityMV[key].median=equityMV[key].highPriceHeap.top();							
					}									if(client.find(tmp.clientName)==client.end())
					{
						client[tmp.clientName].bought=0;
						client[tmp.clientName].sold=0;
						client[tmp.clientName].netTransfer=0;
					}
					if(client.find(buyOrders[key].top().clientName)==client.end())
					{
						client[buyOrders[key].top().clientName].bought=0;
						client[buyOrders[key].top().clientName].sold=0;
						client[buyOrders[key].top().clientName].netTransfer=0;
					}							
					client[tmp.clientName].sold+=tradeAmount;
					client[buyOrders[key].top().clientName].bought+=tradeAmount;
					client[tmp.clientName].netTransfer+=tradeAmount*buyOrders[key].top().price;
					client[buyOrders[key].top().clientName].netTransfer-=tradeAmount*buyOrders[key].top().price;
					if(tradeAmount==tmp.quantity)
					{
						tmp.quantity=0;
						equityInfor tmpTop=buyOrders[key].top();
						buyOrders[key].pop();
						tmpTop.quantity-=tradeAmount;
						if(tmpTop.quantity!=0)
						{
							buyOrders[key].push(tmpTop);
							break;
						}
					}
					else
					{
						tmp.quantity-=tradeAmount;
						//pop the top sell order of this equity from the data structure
						buyOrders[key].pop();
					}
				}
				else
					break;
			}
		}
		if(tmp.quantity!=0 && tmp.duration!=0)
		sellOrders[key].push(tmp);	
		// deal with the sell order book, keep poping those expired orders in the front until the top element in the priority_queue is an active one
		if(sellOrders.find(key)!=sellOrders.end())
		{
			while(!sellOrders[key].empty() && (currentTime-sellOrders[key].top().timeStamp)>=sellOrders[key].top().duration && sellOrders[key].top().duration!=-1)
				sellOrders[key].pop();
		}
	}
} 

void MM::finalOutput()
{
	unordered_map<string, MVInfor>::iterator itrEquityMV;
	set<string>::iterator itrClientNames;
	set<string>::iterator itrEquityNames;
	//cout<<"---End of Day---"<<endl;
	printf("---End of Day---\n");
	long long TAMF=0; // total amount of money transfered
	long long NCP=0; // number of completed trades
	long long totalNST=0; // total number of shares traded
	for(itrEquityMV=equityMV.begin();itrEquityMV!=equityMV.end();itrEquityMV++)
	{
		TAMF+=itrEquityMV->second.totalValue;
		NCP+=(itrEquityMV->second.highPriceHeap.size()+itrEquityMV->second.lowPriceHeap.size());
		totalNST+=itrEquityMV->second.NumSharesTraded;
	}
	//cout<<"Commission Earnings: $"<<commissionFee<<endl;
	printf("Commission Earnings: $%lld\n", commissionFee);
	//cout<<"Total Amount of Money Transferred: $"<<TAMF<<endl;
	printf("Total Amount of Money Transferred: $%lld\n", TAMF);
	//cout<<"Number of Completed Trades: "<<NCP<<endl;
	printf("Number of Completed Trades: %lld\n", NCP);
	//cout<<"Number of Shares Traded: "<<totalNST;
	printf("Number of Shares Traded: %lld", totalNST);
	//print out the command line arguments: transfers information and VWAP information
	if(oTransfers==true)
	{
		for(itrClientNames=clientNames.begin();itrClientNames!=clientNames.end();itrClientNames++)
		{
			//cout<<endl;
			printf("\n");
			if(client.find(*itrClientNames)==client.end()) // if this client did not make any successful trade
			{
				//cout<<*itrClientNames<<" bought 0 and sold 0 for a net transfer of $0";
				printf("%s bought 0 and sold 0 for a net transfer of $0", (*itrClientNames).c_str());
			}
			else
			{
				//cout<<*itrClientNames<<" bought "<<client[*itrClientNames].bought<<" and sold "<<client[*itrClientNames].sold<<" for a net transfer of $"<<client[*itrClientNames].netTransfer;
				printf("%s bought %lld and sold %lld for a net transfer of $%lld", (*itrClientNames).c_str(), client[*itrClientNames].bought, client[*itrClientNames].sold, client[*itrClientNames].netTransfer);
			}			
		}
	}
	if(oVWAP==true)
	{
		for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++)
		{
			//cout<<endl;
			printf("\n");
			if(equityMV.find(*itrEquityNames)==equityMV.end())
			{
				//cout<<*itrEquityNames<<"'s volume-weighted average price: $-1";
				printf("%s's volume-weighted average price: $-1", (*itrEquityNames).c_str());
			}
			else
			{
				//cout<<*itrEquityNames<<"'s volume-weighted average price: $"<<equityMV[*itrEquityNames].totalValue/equityMV[*itrEquityNames].NumSharesTraded;
				printf("%s's volume-weighted average price: $%lld", (*itrEquityNames).c_str(), equityMV[*itrEquityNames].totalValue/equityMV[*itrEquityNames].NumSharesTraded);
			}
		}
	}
	// print out the ttt information
	if(otttv.size()!=0)
	{
		vector<string>::iterator itrOtttv;
		for(itrOtttv=otttv.begin();itrOtttv!=otttv.end();itrOtttv++)
		{
			//cout<<endl;
			printf("\n");
			if(equityttt.find(*itrOtttv)==equityttt.end())
			{
				//cout<<"Time travelers would buy "<<*itrOtttv<<" at time: -1 and sell it at time: -1";
				printf("Time travelers would buy %s at time: -1 and sell it at time: -1", (*itrOtttv).c_str());
			}
			else if(equityttt[*itrOtttv].bestSellT==-1)
			{
				//cout<<"Time travelers would buy "<<*itrOtttv<<" at time: -1 and sell it at time: -1";
				printf("Time travelers would buy %s at time: -1 and sell it at time: -1", (*itrOtttv).c_str());
			}
			else
			{
				//cout<<"Time travelers would buy "<<*itrOtttv<<" at time: "<<equityttt[*itrOtttv].bestBuyT<<" and sell it at time: "<<equityttt[*itrOtttv].bestSellT;
				printf("Time travelers would buy %s at time: %lld and sell it at time: %lld", (*itrOtttv).c_str(), equityttt[*itrOtttv].bestBuyT, equityttt[*itrOtttv].bestSellT);
			}
		}
	}
}
