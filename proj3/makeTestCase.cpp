// This cpp file is used the generate test case, only the PR mode input file is meaningful and no command-line-argument should be provided

#include "makeTestCase.h"
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

	string inputType;
	getline(cin,inputType);
	if(inputType=="TL") // the TL input mode
	{
		equityInforK tmpK;
		equityInforV tmpV;
		const string delimiter=" ";
		string line;
		long long timeStamp;
		long long difference;
		long long idNum=0;
		bool isBuy;
		string::size_type lasPos, pos;
		string key; //this will be used as the key for the name of the equity
		// read the first input order from cin
		getline(cin,line);
		lasPos=line.find_first_not_of(delimiter, 0);
		pos=line.find_first_of(delimiter, lasPos);
		timeStamp=stoll(line.substr(lasPos, pos-lasPos)); 
		currentTime=timeStamp; // set the current timeStamp to the timeStamp of the first order
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter, lasPos);
		tmpV.clientName=line.substr(lasPos, pos-lasPos);
		idNum++;
		tmpK.ID=idNum;// the id number used to decide the order after the tie on the price match
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter, lasPos);
		if(line.substr(lasPos, pos-lasPos)=="BUY")
			isBuy=true;
		else
			isBuy=false;
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter, lasPos);
		key=line.substr(lasPos, pos-lasPos);
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter, lasPos);
		if(line[lasPos]!='$')
		{
			cerr<<"error, there should be a $ sign before the price"<<endl;
			exit(1);
		}
		tmpK.price=stoll(line.substr(lasPos+1, pos-lasPos-1));
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter, lasPos);
		if(line[lasPos]!='#')
		{
			cerr<<"error, there should be a $ sign before the price"<<endl;
			exit(1);
		}
		tmpV.quantity=stoll(line.substr(lasPos+1,pos-lasPos-1));
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter, lasPos);
		tmpV.duration=stoll(line.substr(lasPos));
		// insert the euqity name in a sorted container of set used to record all equity names ever appeared in order
		equityNames.insert(key);
		// insert the equity detail information into the buy order books or sell order books, also update necessary informations about ttt
		if(isBuy==true && tmpV.duration!=0)
		{
			buyOrders[key].insert(make_pair(tmpK,tmpV));
		}
		//if the client offers to sell, then set the best buy time to this and store the value of this possible price
		if(isBuy==false && tmpV.duration!=0)
		{
			sellOrders[key].insert(make_pair(tmpK,tmpV));
			if(ottt.find(key)!=ottt.end())
			{
				equityttt[key].bestBuyT=currentTime;
				equityttt[key].lowestBuyP=tmpK.price;
				equityttt[key].lowestBuyT=currentTime;
				equityttt[key].bestSellT=-1; // This means that no possible output in ttt equity yet, or no sell after buy is possible
			}
		}
		// insert the new appeared client name into the corresponding sorted set
		clientNames.insert(tmpV.clientName);
		// declare the relevant iterators corresponding to different data structures
		set<string>::iterator itrEquityNames;
		set<string>::iterator itrClientNames;
		unordered_map<string, tttInfor>::iterator itrEquityttt;
		unordered_map<string, MVInfor>::iterator itrEquityMV;
		unordered_map<string, clientStat>::iterator itrClients;
		unordered_map<string, map<equityInforK, equityInforV, equityBuyComp> >::iterator itrBuyOrders;
		// iterator for second element, which is a map inside this unordered_map
		map<equityInforK, equityInforV, equityBuyComp>::iterator itrEquityInforB;
		unordered_map<string, map<equityInforK, equityInforV, equitySellComp> >::iterator itrSellOrders; 
		// iterator for second element, which is a map inside this unordered_map
		map<equityInforK, equityInforV, equitySellComp>::iterator itrEquityInforS;
		multiset<long long>::iterator itrTradePrice;
		while(getline(cin,line)) // if there is more input lines
		{
			lasPos=line.find_first_not_of(delimiter, 0);
			pos=line.find_first_of(delimiter, lasPos);
			timeStamp=stoll(line.substr(lasPos, pos-lasPos)); // convert string to long long types
			if(currentTime!=timeStamp)
			{
				// Median output
				if(oMedian==true) 
				{
					//traverse the corrsponding set for stored equity names
					for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++) 
					{
						if(equityMV.find(*itrEquityNames)!=equityMV.end()) // check whether trade happened for this kind of equity
						{
							// check whether the size is even or odd
							if(equityMV[*itrEquityNames].tradePrice.size()%2==0) // if even
							{
								long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2-1;
								itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
								for(long long i=0;i<middleMove;i++)
									itrTradePrice++;
								long long medianPrice=(*itrTradePrice+(*(++itrTradePrice)))/2;
								cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
							}
							else // if odd
							{
								long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2;
								itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
								for(long long i=0;i<middleMove;i++)
									itrTradePrice++;
								long long medianPrice=*itrTradePrice;
								cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
							}
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
							midpointPrice=(buyOrders[*itrEquityNames].begin()->first.price+sellOrders[*itrEquityNames].begin()->first.price)/2;
							cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<midpointPrice<<endl;
						}			
						else
							cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is undefined"<<endl;
					}
				}
				difference=timeStamp-currentTime;
				currentTime=timeStamp;
				// change the duration of every order before this time point, if any orders have expired, remove them from the data structure
				// traverse and adjust the elements in the buy order books 
				for(itrBuyOrders=buyOrders.begin();itrBuyOrders!=buyOrders.end();itrBuyOrders++) // for every key, namely each equity kind in buy order books
				{
					if(!itrBuyOrders->second.empty()) // check if this kind of equity is still in the buy order book 
					{
						for(itrEquityInforB=itrBuyOrders->second.begin();itrEquityInforB!=itrBuyOrders->second.end();itrEquityInforB++)
						{
							if(itrEquityInforB->second.duration!=-1) // if this order does not persist forever, take it out, change the duration, if it should expire, 
															  // then it's done, if it does not expire after the adjustment of the duration, insert it back.
							{
								itrEquityInforB->second.duration-=difference; 
								if(itrEquityInforB->second.duration<=0) // whether the order need to be moved from the order book
									itrBuyOrders->second.erase(itrEquityInforB);
							}
						}
					}
				}
				// traverse and adjust the elements in the sell order books 
				for(itrSellOrders=sellOrders.begin();itrSellOrders!=sellOrders.end();itrSellOrders++) // for every key, namely each equity kind in buy order books
				{
					if(!itrSellOrders->second.empty()) // check if this kind of equity is still in the buy order book 
					{
						for(itrEquityInforS=itrSellOrders->second.begin();itrEquityInforS!=itrSellOrders->second.end();itrEquityInforS++)
						{
							if(itrEquityInforS->second.duration!=-1) // if this order does not persist forever, take it out, change the duration, if it should expire, 
															  // then it's done, if it does not expire after the adjustment of the duration, insert it back.
							{
								itrEquityInforS->second.duration-=difference; 
								if(itrEquityInforS->second.duration<=0) // whether the order need to be moved from the order book
									itrSellOrders->second.erase(itrEquityInforS);
							}
						}
					}
				}
			}
			lasPos=line.find_first_not_of(delimiter, pos);
			pos=line.find_first_of(delimiter, lasPos);
			tmpV.clientName=line.substr(lasPos,pos-lasPos);
			idNum++;
			tmpK.ID=idNum;// the id number used to decide the order after the tie on the price match
			lasPos=line.find_first_not_of(delimiter, pos);
			pos=line.find_first_of(delimiter, lasPos);
			if(line.substr(lasPos,pos-lasPos)=="BUY")
				isBuy=true;
			else
				isBuy=false;
			lasPos=line.find_first_not_of(delimiter, pos);
			pos=line.find_first_of(delimiter, lasPos);
			key=line.substr(lasPos,pos-lasPos);
			lasPos=line.find_first_not_of(delimiter, pos);
			pos=line.find_first_of(delimiter, lasPos);
			if(line[lasPos]!='$')
			{
			cerr<<"error, there should be a $ sign before the price"<<endl;
			exit(1);
			}
			tmpK.price=stoll(line.substr(lasPos+1,pos-lasPos-1));
			lasPos=line.find_first_not_of(delimiter, pos);
			pos=line.find_first_of(delimiter, lasPos);
			if(line[lasPos]!='#')
			{
				cerr<<"error, there should be a $ sign before the price"<<endl;
				exit(1);
			}
			tmpV.quantity=stoll(line.substr(lasPos+1,pos-lasPos-1));
			lasPos=line.find_first_not_of(delimiter, pos);
			pos=line.find_first_of(delimiter, lasPos);
			tmpV.duration=stoll(line.substr(lasPos));
			// insert the euqity name in a sorted container of set used to record all equity names ever appeared in order, clientName similarly
			equityNames.insert(key);
			clientNames.insert(tmpV.clientName);
			if(isBuy==true) // if the newly added order is a buy order
			{
				//update informations about ttt, since this is only a sell oppotunity, the corresponding element in equityttt data structure won't be created if not already created
				if(equityttt.find(key)!=equityttt.end()) // check two conditions: whether the equity is ttt and whether a buying is possible before this time, they can be combined into this check
				{
					if(equityttt[key].bestSellT==-1)// no VALID sell oppotunity before this time point
					{
						equityttt[key].largestDiff=tmpK.price-equityttt[key].lowestBuyP;
						equityttt[key].bestSellT=currentTime;
					}
					else if(equityttt[key].largestDiff<tmpK.price-equityttt[key].lowestBuyP)
					{
						equityttt[key].bestSellT=currentTime;
						equityttt[key].largestDiff=tmpK.price-equityttt[key].lowestBuyP;
						equityttt[key].bestBuyT=equityttt[key].lowestBuyT;
					}
				}
				// seeking trade oppotunities
				if(sellOrders.find(key)!=sellOrders.end()) // if there is at least one offer of such equity EVER appeard for sell in the sell order book
				{
					while(!sellOrders[key].empty() && tmpK.price>=sellOrders[key].begin()->first.price)
					{
						long long tradeAmount=min(tmpV.quantity,sellOrders[key].begin()->second.quantity); // compute the trade amount in this successful trade
						if(oVerbose==true)
							cout<<tmpV.clientName<<" purchased "<<tradeAmount<<" shares of "<<key<<" from "<<sellOrders[key].begin()->second.clientName<<" for $"<<sellOrders[key].begin()->first.price<<"/share"<<endl;
						// adjust to class members after a trade is made
						if(equityMV.find(key)==equityMV.end())// if this equity is curretly not a key in equityMV
						{
							equityMV[key].NumSharesTraded=0;
							equityMV[key].totalValue=0;
						}
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*sellOrders[key].begin()->first.price;
						equityMV[key].tradePrice.insert(sellOrders[key].begin()->first.price);
						if(client.find(tmpV.clientName)==client.end())
						{
							client[tmpV.clientName].bought=0;
							client[tmpV.clientName].sold=0;
							client[tmpV.clientName].netTransfer=0;
						}
						if(client.find(sellOrders[key].begin()->second.clientName)==client.end())
						{
							client[sellOrders[key].begin()->second.clientName].bought=0;
							client[sellOrders[key].begin()->second.clientName].sold=0;
							client[sellOrders[key].begin()->second.clientName].netTransfer=0;
						}							
						client[tmpV.clientName].bought+=tradeAmount;
						client[sellOrders[key].begin()->second.clientName].sold+=tradeAmount;
						client[tmpV.clientName].netTransfer-=tradeAmount*sellOrders[key].begin()->first.price;
						client[sellOrders[key].begin()->second.clientName].netTransfer+=tradeAmount*sellOrders[key].begin()->first.price;
						if(tradeAmount==tmpV.quantity)
						{
							tmpV.quantity=0;
							sellOrders[key].begin()->second.quantity-=tradeAmount;
							if(sellOrders[key].begin()->second.quantity==0)
								sellOrders[key].erase(sellOrders[key].begin());	
							break;
						}
						else
						{
							tmpV.quantity-=tradeAmount;
							//pop the top sell order of this equity from the data structure
							sellOrders[key].erase(sellOrders[key].begin());
						}
					}
				}
				if(tmpV.quantity!=0 && tmpV.duration!=0)
				buyOrders[key].insert(make_pair(tmpK,tmpV));	
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
						equityttt[key].lowestBuyP=tmpK.price;
					}
					else if(equityttt[key].lowestBuyP>tmpK.price) // if there was already a buy oppotunity and the newly price of the oppotunity in currentTime is lower
					{
						equityttt[key].lowestBuyT=currentTime;
						equityttt[key].lowestBuyP=tmpK.price;
						if(equityttt[key].bestSellT==-1) // if there is no sell oppotunity after the previous buy oppotunity yet 
							equityttt[key].bestBuyT=currentTime;
					}
				}
				// seeking trade oppotunities
				if(buyOrders.find(key)!=buyOrders.end()) // if there is at least one offer of such equity EVER appeard for sell in the sell order book
				{
					while(!buyOrders[key].empty() && tmpK.price<=buyOrders[key].begin()->first.price)
					{
						long long tradeAmount=min(tmpV.quantity,buyOrders[key].begin()->second.quantity); // compute the trade amount in this successful trade
						if(oVerbose==true)
							cout<<buyOrders[key].begin()->second.clientName<<" purchased "<<tradeAmount<<" shares of "<<key<<" from "<<tmpV.clientName<<" for $"<<buyOrders[key].begin()->first.price<<"/share"<<endl;
						// adjust to class members after a trade is made
						if(equityMV.find(key)==equityMV.end())// if this equity is curretly not a key in equityMV
						{
							equityMV[key].NumSharesTraded=0;
							equityMV[key].totalValue=0;
						}
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*buyOrders[key].begin()->first.price;
						equityMV[key].tradePrice.insert(buyOrders[key].begin()->first.price);
						if(client.find(tmpV.clientName)==client.end())
						{
							client[tmpV.clientName].bought=0;
							client[tmpV.clientName].sold=0;
							client[tmpV.clientName].netTransfer=0;
						}
						if(client.find(buyOrders[key].begin()->second.clientName)==client.end())
						{
							client[buyOrders[key].begin()->second.clientName].bought=0;
							client[buyOrders[key].begin()->second.clientName].sold=0;
							client[buyOrders[key].begin()->second.clientName].netTransfer=0;
						}							
						client[tmpV.clientName].sold+=tradeAmount;
						client[buyOrders[key].begin()->second.clientName].bought+=tradeAmount;
						client[tmpV.clientName].netTransfer+=tradeAmount*buyOrders[key].begin()->first.price;
						client[buyOrders[key].begin()->second.clientName].netTransfer-=tradeAmount*buyOrders[key].begin()->first.price;
						if(tradeAmount==tmpV.quantity)
						{
							tmpV.quantity=0;
							buyOrders[key].begin()->second.quantity-=tradeAmount;
							if(buyOrders[key].begin()->second.quantity==0)
								buyOrders[key].erase(buyOrders[key].begin());
							break;
						}
						else
						{
							tmpV.quantity-=tradeAmount;
							//pop the top sell order of this equity from the data structure
							buyOrders[key].erase(buyOrders[key].begin());
						}
					}
				}
				if(tmpV.quantity!=0 && tmpV.duration!=0)
				sellOrders[key].insert(make_pair(tmpK,tmpV));
			}
		}
		// the getline loop finished, treat the end of the day like the timeStamp has moved again
		// Median output
		if(oMedian==true) 
		{
			//traverse the corrsponding set for stored equity names
			for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++) 
			{
				if(equityMV.find(*itrEquityNames)!=equityMV.end()) // check whether trade happened for this kind of equity
				{
					// check whether the size is even or odd
					if(equityMV[*itrEquityNames].tradePrice.size()%2==0) // if even
					{
						long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2-1;
						itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
						for(long long i=0;i<middleMove;i++)
							itrTradePrice++;
						long long medianPrice=(*itrTradePrice+(*(++itrTradePrice)))/2;
						cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
					}
					else // if odd
					{
						long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2;
						itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
						for(long long i=0;i<middleMove;i++)
							itrTradePrice++;
						long long medianPrice=(*itrTradePrice)/2;
						cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
					}
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
					midpointPrice=(buyOrders[*itrEquityNames].begin()->first.price+sellOrders[*itrEquityNames].begin()->first.price)/2;
					cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<midpointPrice<<endl;
				}			
				else
					cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is undefined"<<endl;
			}
		}
		// pint all END OF DAY	output
		cout<<"---End of Day---"<<endl;
		long long TAMF=0; // total amount of money transfered
		long long NCP=0; // number of completed trades
		long long totalNST=0; // total number of shares traded
		for(itrEquityMV=equityMV.begin();itrEquityMV!=equityMV.end();itrEquityMV++)
		{
			TAMF+=itrEquityMV->second.totalValue;
			NCP+=itrEquityMV->second.tradePrice.size();
			totalNST+=itrEquityMV->second.NumSharesTraded;
		}
		cout<<"Commission Earnings: $"<<(TAMF/100)*2<<endl;
		cout<<"Total Amount of Money Transfered: $"<<TAMF<<endl;
		cout<<"Number of Completed Trades: "<<NCP<<endl;
		cout<<"Number of Shares Traded: "<<totalNST;
		//print out the command line arguments: transfers information and VWAP information
		if(oTransfers==true)
		{
			for(itrClientNames=clientNames.begin();itrClientNames!=clientNames.end();itrClientNames++)
			{
				cout<<endl;
				if(client.find(*itrClientNames)==client.end()) // if this client did not make any successful trade
					cout<<*itrClientNames<<" bought 0 and sold 0 for a net transfer of $0";
				else
					cout<<*itrClientNames<<" bought "<<client[*itrClientNames].bought<<" and sold "<<client[*itrClientNames].sold<<" for a net transfer of $"<<client[*itrClientNames].netTransfer;
			}
		}
		if(oVWAP==true)
		{
			for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++)
			{
				cout<<endl;
				if(equityMV.find(*itrEquityNames)==equityMV.end())
					cout<<*itrEquityNames<<"'s volumn weighted average price: $-1";
				else
					cout<<*itrEquityNames<<"'s volumn weighted average price: $"<<equityMV[*itrEquityNames].totalValue/equityMV[*itrEquityNames].NumSharesTraded;
			}
		}
		// print out the ttt information
		if(otttv.size()!=0)
		{
			vector<string>::iterator itrOtttv;
			for(itrOtttv=otttv.begin();itrOtttv!=otttv.end();itrOtttv++)
			{
				cout<<endl;
				if(equityttt.find(*itrOtttv)==equityttt.end())
					cout<<"Time travelers would buy "<<*itrOtttv<<" at time: -1 and sell it at time: -1";
				else if(equityttt[*itrOtttv].bestSellT==-1)
					cout<<"Time travelers would buy "<<*itrOtttv<<" at time: -1 and sell it at time: -1";
				else
					cout<<"Time travelers would buy "<<*itrOtttv<<" at time: "<<equityttt[*itrOtttv].bestBuyT<<" and sell it at time: "<<equityttt[*itrOtttv].bestSellT;
			}
		}
	}
	else // the PR input mode
	{
		string line;
		const string delimiter=" ";
		string::size_type lasPos, pos;
		getline(cin,line);
		lasPos=line.find_first_of(delimiter,0);
		pos=line.find_first_not_of(delimiter,lasPos);
		int random_seed=atoi(line.substr(pos).c_str());
		getline(cin,line);
		lasPos=line.find_first_of(delimiter,0);
		pos=line.find_first_not_of(delimiter,lasPos);
		long long number_of_orders=stoll(line.substr(pos));
		getline(cin,line);
		lasPos=line.find_first_of(delimiter,0);
		pos=line.find_first_not_of(delimiter,lasPos);
		char last_client=line.substr(pos)[0];
		getline(cin,line);
		lasPos=line.find_first_of(delimiter,0);
		pos=line.find_first_not_of(delimiter,lasPos);
		char last_equity=line.substr(pos)[0];
		getline(cin,line);
		lasPos=line.find_first_of(delimiter,0);
		pos=line.find_first_not_of(delimiter,lasPos);
		double arrival_rate=::atof(line.substr(pos).c_str());
		std::mt19937 gen(random_seed);
		std::uniform_int_distribution<char> clients('a', last_client);
		std::uniform_int_distribution<char> equities('A', last_equity);
		std::exponential_distribution<> arrivals(arrival_rate);
		std::bernoulli_distribution buy_or_sell;
		std::uniform_int_distribution<> price(2, 11);
		std::uniform_int_distribution<> quantity(1, 30);
		std::uniform_int_distribution<> duration(-1, floor(number_of_orders/arrival_rate));
		// generate the first order
		equityInforK tmpK;
		equityInforV tmpV;
		long long timeStamp;
		long long difference;
		long long idNum=0;
		bool isBuy;
		string key; // used as the name for name of the equity
		long long generator_timeStamp=0;
		timeStamp=generator_timeStamp;
		generator_timeStamp=generator_timeStamp+floor(arrivals(gen)+0.5);
		currentTime=0;
		tmpV.clientName=string("C_")+clients(gen);
		idNum++;
		tmpK.ID=idNum;
		isBuy=(buy_or_sell(gen) ? true : false);
		string BUY_OR_SELL;
		if(isBuy==true)
			BUY_OR_SELL="BUY";
		else
			BUY_OR_SELL="SELL";
		key=string("E_")+equities(gen);
		tmpK.price=5*price(gen);
		tmpV.quantity=quantity(gen);
		tmpV.duration=duration(gen);
				// insert the euqity name in a sorted container of set used to record all equity names ever appeared in order
		equityNames.insert(key);
		// insert the equity detail information into the buy order books or sell order books, also update necessary informations about ttt
		if(isBuy==true && tmpV.duration!=0)
		{
			buyOrders[key].insert(make_pair(tmpK,tmpV));
		}
		//if the client offers to sell, then set the best buy time to this and store the value of this possible price
		if(isBuy==false && tmpV.duration!=0)
		{
			sellOrders[key].insert(make_pair(tmpK,tmpV));
			if(ottt.find(key)!=ottt.end()) // if the ttt exist for this equity
			{
				equityttt[key].bestBuyT=currentTime;
				equityttt[key].lowestBuyP=tmpK.price;
				equityttt[key].lowestBuyT=currentTime;
				equityttt[key].bestSellT=-1; // This means that no possible output in ttt equity yet, or no sell after buy is possible
			}
		}
		// insert the new appeared client name into the corresponding sorted set
		clientNames.insert(tmpV.clientName);
		// declare the relevant iterators corresponding to different data structures
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// output cout
		cout<<timeStamp<<" "<<tmpV.clientName<<" "<<BUY_OR_SELL<<" "<<key<<" $"<<tmpK.price<<" #"<<tmpV.quantity<<" "<<tmpV.duration<<endl;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		currentTime=timeStamp;
		set<string>::iterator itrEquityNames;
		set<string>::iterator itrClientNames;
		unordered_map<string, tttInfor>::iterator itrEquityttt;
		unordered_map<string, MVInfor>::iterator itrEquityMV;
		unordered_map<string, clientStat>::iterator itrClients;
		unordered_map<string, map<equityInforK, equityInforV, equityBuyComp> >::iterator itrBuyOrders;
		// iterator for second element, which is a map inside this unordered_map
		map<equityInforK, equityInforV, equityBuyComp>::iterator itrEquityInforB;
		unordered_map<string, map<equityInforK, equityInforV, equitySellComp> >::iterator itrSellOrders; 
		// iterator for second element, which is a map inside this unordered_map
		map<equityInforK, equityInforV, equitySellComp>::iterator itrEquityInforS;
		multiset<long long>::iterator itrTradePrice;
		for(long long i=1;i<number_of_orders;i++)
		{
			timeStamp=generator_timeStamp;
			generator_timeStamp=generator_timeStamp+floor(arrivals(gen)+0.5);
			if(currentTime!=timeStamp)
			{
				// Median output
				if(oMedian==true) 
				{
					//traverse the corrsponding set for stored equity names
					for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++) 
					{
						if(equityMV.find(*itrEquityNames)!=equityMV.end()) // check whether trade happened for this kind of equity
						{
							// check whether the size is even or odd
							if(equityMV[*itrEquityNames].tradePrice.size()%2==0) // if even
							{
								long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2-1;
								itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
								for(long long i=0;i<middleMove;i++)
									itrTradePrice++;
								long long medianPrice=(*itrTradePrice+(*(++itrTradePrice)))/2;
								cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
							}
							else // if odd
							{
								long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2;
								itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
								for(long long i=0;i<middleMove;i++)
									itrTradePrice++;
								long long medianPrice=*itrTradePrice;
								cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
							}
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
							midpointPrice=(buyOrders[*itrEquityNames].begin()->first.price+sellOrders[*itrEquityNames].begin()->first.price)/2;
							cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<midpointPrice<<endl;
						}			
						else
							cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is undefined"<<endl;
					}
				}
				difference=timeStamp-currentTime;
				currentTime=timeStamp;
				// change the duration of every order before this time point, if any orders have expired, remove them from the data structure
				// traverse and adjust the elements in the buy order books 
				for(itrBuyOrders=buyOrders.begin();itrBuyOrders!=buyOrders.end();itrBuyOrders++) // for every key, namely each equity kind in buy order books
				{
					if(!itrBuyOrders->second.empty()) // check if this kind of equity is still in the buy order book 
					{
						for(itrEquityInforB=itrBuyOrders->second.begin();itrEquityInforB!=itrBuyOrders->second.end();itrEquityInforB++)
						{
							if(itrEquityInforB->second.duration!=-1) // if this order does not persist forever, take it out, change the duration, if it should expire, 
																  // then it's done, if it does not expire after the adjustment of the duration, insert it back.
							{
								itrEquityInforB->second.duration-=difference; 
								if(itrEquityInforB->second.duration<=0) // whether the order need to be moved from the order book
									itrBuyOrders->second.erase(itrEquityInforB);
							}
						}
					}
				}
				// traverse and adjust the elements in the sell order books 
				for(itrSellOrders=sellOrders.begin();itrSellOrders!=sellOrders.end();itrSellOrders++) // for every key, namely each equity kind in buy order books
				{
					if(!itrSellOrders->second.empty()) // check if this kind of equity is still in the buy order book 
					{
						for(itrEquityInforS=itrSellOrders->second.begin();itrEquityInforS!=itrSellOrders->second.end();itrEquityInforS++)
						{
							if(itrEquityInforS->second.duration!=-1) // if this order does not persist forever, take it out, change the duration, if it should expire, 
																  // then it's done, if it does not expire after the adjustment of the duration, insert it back.
							{
								itrEquityInforS->second.duration-=difference; 
								if(itrEquityInforS->second.duration<=0) // whether the order need to be moved from the order book
									itrSellOrders->second.erase(itrEquityInforS);
							}
						}
					}
				}
			}
			tmpV.clientName=string("C_")+clients(gen);
			idNum++;
			tmpK.ID=idNum;
			isBuy=(buy_or_sell(gen) ? true:false);
			key=string("E_")+equities(gen);
			tmpK.price=5*price(gen);
			tmpV.quantity=quantity(gen);
			tmpV.duration=duration(gen);
			equityNames.insert(key);
			clientNames.insert(tmpV.clientName);
			if(isBuy==true)
				BUY_OR_SELL="BUY";
			else
				BUY_OR_SELL="SELL";
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// output cout
			cout<<timeStamp<<" "<<tmpV.clientName<<" "<<BUY_OR_SELL<<" "<<key<<" $"<<tmpK.price<<" #"<<tmpV.quantity<<" "<<tmpV.duration<<endl;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if(isBuy==true) // if the newly added order is a buy order
			{
				//update informations about ttt, since this is only a sell oppotunity, the corresponding element in equityttt data structure won't be created if not already created
				if(equityttt.find(key)!=equityttt.end()) // check two conditions: whether the equity is ttt and whether a buying is possible before this time, they can be combined into this check
				{
					if(equityttt[key].bestSellT==-1)// no VALID sell oppotunity before this time point
					{
						equityttt[key].largestDiff=tmpK.price-equityttt[key].lowestBuyP;
						equityttt[key].bestSellT=currentTime;
					}
					else if(equityttt[key].largestDiff<tmpK.price-equityttt[key].lowestBuyP)
					{
						equityttt[key].bestSellT=currentTime;
						equityttt[key].largestDiff=tmpK.price-equityttt[key].lowestBuyP;
						equityttt[key].bestBuyT=equityttt[key].lowestBuyT;
					}
				}
				// seeking trade oppotunities
				if(sellOrders.find(key)!=sellOrders.end()) // if there is at least one offer of such equity EVER appeard for sell in the sell order book
				{
					while(!sellOrders[key].empty() && tmpK.price>=sellOrders[key].begin()->first.price)
					{
						long long tradeAmount=min(tmpV.quantity,sellOrders[key].begin()->second.quantity); // compute the trade amount in this successful trade
						if(oVerbose==true)
							cout<<tmpV.clientName<<" purchased "<<tradeAmount<<" shares of "<<key<<" from "<<sellOrders[key].begin()->second.clientName<<" for $"<<sellOrders[key].begin()->first.price<<"/share"<<endl;
						// adjust to class members after a trade is made
						if(equityMV.find(key)==equityMV.end())// if this equity is curretly not a key in equityMV
						{
							equityMV[key].NumSharesTraded=0;
							equityMV[key].totalValue=0;
						}
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*sellOrders[key].begin()->first.price;
						equityMV[key].tradePrice.insert(sellOrders[key].begin()->first.price);
						if(client.find(tmpV.clientName)==client.end())
						{
							client[tmpV.clientName].bought=0;
							client[tmpV.clientName].sold=0;
							client[tmpV.clientName].netTransfer=0;
						}
						if(client.find(sellOrders[key].begin()->second.clientName)==client.end())
						{
							client[sellOrders[key].begin()->second.clientName].bought=0;
							client[sellOrders[key].begin()->second.clientName].sold=0;
							client[sellOrders[key].begin()->second.clientName].netTransfer=0;
						}							
						client[tmpV.clientName].bought+=tradeAmount;
						client[sellOrders[key].begin()->second.clientName].sold+=tradeAmount;
						client[tmpV.clientName].netTransfer-=tradeAmount*sellOrders[key].begin()->first.price;
						client[sellOrders[key].begin()->second.clientName].netTransfer+=tradeAmount*sellOrders[key].begin()->first.price;
						if(tradeAmount==tmpV.quantity)
						{
							tmpV.quantity=0;
							sellOrders[key].begin()->second.quantity-=tradeAmount;
							if(sellOrders[key].begin()->second.quantity==0)
								sellOrders[key].erase(sellOrders[key].begin());	
							break;
						}
						else
						{
							tmpV.quantity-=tradeAmount;
							//pop the top sell order of this equity from the data structure
							sellOrders[key].erase(sellOrders[key].begin());
						}
					}
				}
				if(tmpV.quantity!=0 && tmpV.duration!=0)
					buyOrders[key].insert(make_pair(tmpK,tmpV));
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
						equityttt[key].lowestBuyP=tmpK.price;
					}
					else if(equityttt[key].lowestBuyP>tmpK.price) // if there was already a buy oppotunity and the newly price of the oppotunity in currentTime is lower
					{
						equityttt[key].lowestBuyT=currentTime;
						equityttt[key].lowestBuyP=tmpK.price;
						if(equityttt[key].bestSellT==-1) // if there is no sell oppotunity after the previous buy oppotunity yet 
							equityttt[key].bestBuyT=currentTime;
					}
				}
				// seeking trade oppotunities
				if(buyOrders.find(key)!=buyOrders.end()) // if there is at least one offer of such equity EVER appeard for sell in the sell order book
				{
					while(!buyOrders[key].empty() && tmpK.price<=buyOrders[key].begin()->first.price)
					{
						long long tradeAmount=min(tmpV.quantity,buyOrders[key].begin()->second.quantity); // compute the trade amount in this successful trade
						if(oVerbose==true)
							cout<<buyOrders[key].begin()->second.clientName<<" purchased "<<tradeAmount<<" shares of "<<key<<" from "<<tmpV.clientName<<" for $"<<buyOrders[key].begin()->first.price<<"/share"<<endl;
						// adjust to class members after a trade is made
						if(equityMV.find(key)==equityMV.end())// if this equity is curretly not a key in equityMV
						{
							equityMV[key].NumSharesTraded=0;
							equityMV[key].totalValue=0;
						}
						equityMV[key].NumSharesTraded+=tradeAmount;
						equityMV[key].totalValue+=tradeAmount*buyOrders[key].begin()->first.price;
						equityMV[key].tradePrice.insert(buyOrders[key].begin()->first.price);
						if(client.find(tmpV.clientName)==client.end())
						{
							client[tmpV.clientName].bought=0;
							client[tmpV.clientName].sold=0;
							client[tmpV.clientName].netTransfer=0;
						}
						if(client.find(buyOrders[key].begin()->second.clientName)==client.end())
						{
							client[buyOrders[key].begin()->second.clientName].bought=0;
							client[buyOrders[key].begin()->second.clientName].sold=0;
							client[buyOrders[key].begin()->second.clientName].netTransfer=0;
						}							
						client[tmpV.clientName].sold+=tradeAmount;
						client[buyOrders[key].begin()->second.clientName].bought+=tradeAmount;
						client[tmpV.clientName].netTransfer+=tradeAmount*buyOrders[key].begin()->first.price;
						client[buyOrders[key].begin()->second.clientName].netTransfer-=tradeAmount*buyOrders[key].begin()->first.price;
						if(tradeAmount==tmpV.quantity)
						{
							tmpV.quantity=0;
							buyOrders[key].begin()->second.quantity-=tradeAmount;
							if(buyOrders[key].begin()->second.quantity==0)
								buyOrders[key].erase(buyOrders[key].begin());
							break;
						}
						else
						{
							tmpV.quantity-=tradeAmount;
							//pop the top sell order of this equity from the data structure
							buyOrders[key].erase(buyOrders[key].begin());
						}
					}
				}
				if(tmpV.quantity!=0 && tmpV.duration!=0)
				sellOrders[key].insert(make_pair(tmpK,tmpV));
			}
		}
		// the getline loop finished, treat the end of the day like the timeStamp has moved again
		// Median output
		if(oMedian==true) 
		{
			//traverse the corrsponding set for stored equity names
			for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++) 
			{
				if(equityMV.find(*itrEquityNames)!=equityMV.end()) // check whether trade happened for this kind of equity
				{
					// check whether the size is even or odd
					if(equityMV[*itrEquityNames].tradePrice.size()%2==0) // if even
					{
						long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2-1;
						itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
						for(long long i=0;i<middleMove;i++)
							itrTradePrice++;
						long long medianPrice=(*itrTradePrice+(*(++itrTradePrice)))/2;
						cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
					}
					else // if odd
					{
						long long middleMove=equityMV[*itrEquityNames].tradePrice.size()/2;
						itrTradePrice=equityMV[*itrEquityNames].tradePrice.begin();
						for(long long i=0;i<middleMove;i++)
							itrTradePrice++;
						long long medianPrice=(*itrTradePrice)/2;
						cout<<"Median match price of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<medianPrice<<endl;
					}
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
					midpointPrice=(buyOrders[*itrEquityNames].begin()->first.price+sellOrders[*itrEquityNames].begin()->first.price)/2;
					cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is $"<<midpointPrice<<endl;
				}			
				else
					cout<<"MidPoint of "<<*itrEquityNames<<" at time "<<currentTime<<" is undefined"<<endl;
			}
		}
				// pint all END OF DAY	output
		cout<<"---End of Day---"<<endl;
		long long TAMF=0; // total amount of money transfered
		long long NCP=0; // number of completed trades
		long long totalNST=0; // total number of shares traded
		for(itrEquityMV=equityMV.begin();itrEquityMV!=equityMV.end();itrEquityMV++)
		{
			TAMF+=itrEquityMV->second.totalValue;
			NCP+=itrEquityMV->second.tradePrice.size();
			totalNST+=itrEquityMV->second.NumSharesTraded;
		}
		cout<<"Commission Earnings: $"<<(TAMF/100)*2<<endl;
		cout<<"Total Amount of Money Transfered: $"<<TAMF<<endl;
		cout<<"Number of Completed Trades: "<<NCP<<endl;
		cout<<"Number of Shares Traded: "<<totalNST;
		//print out the command line arguments: transfers information and VWAP information
		if(oTransfers==true)
		{
			for(itrClientNames=clientNames.begin();itrClientNames!=clientNames.end();itrClientNames++)
			{
				cout<<endl;
				if(client.find(*itrClientNames)==client.end()) // if this client did not make any successful trade
					cout<<*itrClientNames<<" bought 0 and sold 0 for a net transfer of $0";
				else
					cout<<*itrClientNames<<" bought "<<client[*itrClientNames].bought<<" and sold "<<client[*itrClientNames].sold<<" for a net transfer of $"<<client[*itrClientNames].netTransfer;
			}
		}
		if(oVWAP==true)
		{
			for(itrEquityNames=equityNames.begin();itrEquityNames!=equityNames.end();itrEquityNames++)
			{
				cout<<endl;
				if(equityMV.find(*itrEquityNames)==equityMV.end())
					cout<<*itrEquityNames<<"'s volumn weighted average price: $-1";
				else
					cout<<*itrEquityNames<<"'s volumn weighted average price: $"<<equityMV[*itrEquityNames].totalValue/equityMV[*itrEquityNames].NumSharesTraded;
			}
		}
		// print out the ttt information
		if(otttv.size()!=0)
		{
			vector<string>::iterator itrOtttv;
			for(itrOtttv=otttv.begin();itrOtttv!=otttv.end();itrOtttv++)
			{
				cout<<endl;
				if(equityttt.find(*itrOtttv)==equityttt.end())
					cout<<"Time travelers would buy "<<*itrOtttv<<" at time: -1 and sell it at time: -1";
				else if(equityttt[*itrOtttv].bestSellT==-1)
					cout<<"Time travelers would buy "<<*itrOtttv<<" at time: -1 and sell it at time: -1";
				else
					cout<<"Time travelers would buy "<<*itrOtttv<<" at time: "<<equityttt[*itrOtttv].bestBuyT<<" and sell it at time: "<<equityttt[*itrOtttv].bestSellT;
			}
		}
	}
}
