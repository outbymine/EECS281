#include"marketMaker.h"
using namespace std;

int main(int argc, char **argv)
{
	std::ios_base::sync_with_stdio(false);
	MM MM1(argc, argv);
	equityInfor tmp;
	string key;
	long long idNum=0;
	string line;
	bool isBuy=false;
	string delimiter=" 	";
	getline(cin, line);
	if(line=="TL")
	{
		getline(cin, line);
		MM1.readLine1(line, delimiter, key, isBuy, tmp, idNum);
		while(getline(cin, line))
		{
			readLine(line, delimiter, key, isBuy, tmp, idNum, MM1);
			MM1.processTime(tmp);
			MM1.matchAndFill(key, isBuy, tmp);
		}
		tmp.timeStamp++; // treat the end of day like the timeStamp has moved again
		MM1.processTime(tmp);
		MM1.finalOutput();
	}
	else
	{
		long long generator_timeStamp=0;
		getline(cin, line);
		string::size_type lasPos, pos;
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
		tmp.timeStamp=generator_timeStamp;
		generator_timeStamp=generator_timeStamp+floor(arrivals(gen)+0.5);
		tmp.clientName=string("C_")+clients(gen);
		idNum++;
		tmp.ID=idNum;
		isBuy=(buy_or_sell(gen) ? true : false);
		key=string("E_")+equities(gen);
		tmp.price=5*price(gen);
		tmp.quantity=quantity(gen);
		tmp.duration=duration(gen);
		// insert the euqity name in a sorted container of set used to record all equity names ever appeared in order
		MM1.equityNames.insert(key);
		// insert the equity detail information into the buy order books or sell order books, also update necessary informations about ttt
		if(isBuy==true && tmp.duration!=0)
		{
			MM1.buyOrders[key].push(tmp);
		}
		//if the client offers to sell, then set the best buy time to this and store the value of this possible price
		if(isBuy==false && tmp.duration!=0)
		{
			MM1.sellOrders[key].push(tmp);
			if(MM1.ottt.find(key)!=MM1.ottt.end()) // if the ttt exist for this equity
			{
				MM1.equityttt[key].bestBuyT=MM1.currentTime;
				MM1.equityttt[key].lowestBuyP=tmp.price;
				MM1.equityttt[key].lowestBuyT=MM1.currentTime;
				MM1.equityttt[key].bestSellT=-1; // This means that no possible output in ttt equity yet, or no sell after buy is possible
			}
		}
		// insert the new appeared client name into the corresponding sorted set
		MM1.clientNames.insert(tmp.clientName);
		MM1.currentTime=tmp.timeStamp;
		for(long long i=1; i<number_of_orders; i++)
		{
			tmp.timeStamp=generator_timeStamp;
			generator_timeStamp=generator_timeStamp+floor(arrivals(gen)+0.5);
			tmp.clientName=string("C_")+clients(gen);
			idNum++;
			tmp.ID=idNum;
			isBuy=(buy_or_sell(gen) ? true : false);
			key=string("E_")+equities(gen);
			tmp.price=5*price(gen);
			tmp.quantity=quantity(gen);
			tmp.duration=duration(gen);
			MM1.processTime(tmp);
			MM1.matchAndFill(key, isBuy, tmp);						
		}
		tmp.timeStamp++; // treat the end of day like the timeStamp has moved again
		MM1.processTime(tmp);
		MM1.finalOutput();
	}
	return 0;
}
