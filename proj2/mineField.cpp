#include"mineField.h"
using namespace std;

void help()//print out the help information
{
	cout<<"This project is called breaking out of the mine, and it is meant to find that whether it is";
	cout<<"possible to blast away a plie of rubbers using dynamites to find the way out"<<endl;
	cout<<"There are totally 4 tyes of heaps, namely, sorted_heap, poorman_heap, pairing_heap, binary_heap";
	cout<<"they can be specified in command line arguments"<<endl;
	cout<<"if verbose or v is specified in the command line arguments, more information is printed out,";
	cout<<"please refer to the project manual for the details of output under this condition"<<endl;
}

mineField::mineField(){}

mineField::~mineField(){}

void mineField::output()
{
	if(whetherEsc==false)
	{
		cout<<"You couldn't escape!";
		if(!blasted.empty())
		{
			cout<<" You used your last dynamite sticks on tile "<<blasted[blastedNum-1]->x<<","<<blasted[blastedNum-1]->y<<".";
		}
	}
	else if(whetherEsc==true && blastedNum>0)
	{
		cout<<"You escaped through tile "<<blasted[blastedNum-1]->x<<","<<blasted[blastedNum-1]->y<<"!";
		cout<<" You have "<<dynamite<<" sticks of dynamite left.";
	}
	else
	{
		cout<<"You escaped through tile "<<start.x<<","<<start.y<<"!";
		cout<<" You have "<<dynamite<<" sticks of dynamite left.";
	}
	if(vNum!=0) 
	{
		if(!blasted.empty())
		{
			vNum=min(blastedNum, vNum);
			cout<<endl;
			cout<<"First tiles blown up:"<<endl;
			for(int i=0;i<vNum;i++)
				cout<<i+1<<" ("<<blasted[i]->x<<","<<blasted[i]->y<<"): "<<blasted[i]->dynaQuant<<endl;
			cout<<"Last tiles blown up:"<<endl;
			for(int i=0;i<vNum;i++)
				cout<<blastedNum-i<<" ("<<blasted[blastedNum-i-1]->x<<","<<blasted[blastedNum-i-1]->y<<"): "<<blasted[blastedNum-i-1]->dynaQuant<<endl;
			cout<<"Hardest tiles to blow up:"<<endl;
			binary_heap<information*, inforComp1> hardest;
			for (int i=0; i<blastedNum;i++)
				hardest.push(blasted[i]);
			for(int i=0;i<vNum;i++)
			{
				cout<<i+1<<" ("<<hardest.top()->x<<","<<hardest.top()->y<<"): "<<hardest.top()->dynaQuant<<endl;
				hardest.pop();
			}
			cout<<"Easiest tiles to blow up:"<<endl;
			binary_heap<information*, inforComp> easiest;
			for(int i=0; i<blastedNum;i++)
				easiest.push(blasted[i]);
			for(int i=0;i<vNum;i++)
			{
				cout<<i+1<<" ("<<easiest.top()->x<<","<<easiest.top()->y<<"): "<<easiest.top()->dynaQuant<<endl;
				easiest.pop();
			}
		}
		else if(whetherEsc==true)
		{
			cout<<endl;
			cout<<"First tiles blown up:"<<endl;
			cout<<"Last tiles blown up:"<<endl;
			cout<<"Hardest tiles to blow up:"<<endl;
			cout<<"Easiest tiles to blow up:";
		}
	}
}

mineField::mineField(int argc, char **argv)
{
	int c;
	while((c=getopt_long(argc, argv, "hv:c:", long_options, NULL))!=-1)
	{
		switch(c)
		{
			case 'h':
				help();
				exit(0);
			case 'v':
				vNum=atoi(optarg);
				break;
			case 'c':
				heapType=string(optarg);
				break;
			case '?':
				cerr<<"unknow command line argument, error"<<endl;
				exit(1);
			default: 
				abort();
		}
	}
	whetherEsc=false;
	ifstream inFile;
	inFile.open(argv[optind]);
	getline(inFile, inMode);
	blastedNum=0;
	if(inMode=="M")//if the input mode is M
	{
		const string delimiter=" ";
		string line;
		getline(inFile, line);
		string::size_type lasPos=line.find_first_of(delimiter, 0);
		string::size_type pos=line.find_first_not_of(delimiter, lasPos);
		dynamite=atoi((line.substr(pos)).c_str());
		getline(inFile, line);
		lasPos=line.find_first_of(delimiter, 0);
		pos=line.find_first_not_of(delimiter, lasPos);
		size=atoi((line.substr(pos)).c_str());
		information inforTmp;
		int row=0;
		while(getline(inFile,line))
		{
			lasPos=line.find_first_not_of(delimiter, 0);
			pos=line.find_first_of(delimiter, lasPos);
			int col=0;
			while(string::npos != pos || string::npos !=lasPos)
			{
				if((line.substr(lasPos, pos-lasPos))=="SL")
				{
					inforTmp.dynaQuant=0;
					inforTmp.x=col;
					inforTmp.y=size-row-1;
					inforTmp.flag=false;
					start=inforTmp;
					routeMap.push_back(inforTmp); 
					col++;
				}
				else
				{
					inforTmp.dynaQuant=atoi((line.substr(lasPos, pos-lasPos)).c_str());
					inforTmp.x=col;
					inforTmp.y=size-row-1;
					inforTmp.flag=false;
					routeMap.push_back(inforTmp); 
					col++;
				}
				lasPos=line.find_first_not_of(delimiter, pos);
				pos=line.find_first_of(delimiter,lasPos);
			}
			row++;
		}
	}
	else //the PR input mode
	{
		const string delimiter=" ";
		string line;
		getline(inFile, line);
		string::size_type lasPos=line.find_first_of(delimiter, 0);
		string::size_type pos=line.find_first_not_of(delimiter, lasPos);
		dynamite=atoi((line.substr(pos)).c_str());
		getline(inFile, line);
		lasPos=line.find_first_of(delimiter, 0);
		pos=line.find_first_not_of(delimiter, lasPos);
		information fill;
		fill.x=0;
		fill.y=0;
		fill.flag=false;
		fill.dynaQuant=0;
		size=atoi((line.substr(pos)).c_str());
		routeMap.assign(size*size, fill);
		getline(inFile, line);
		lasPos=line.find_first_of(delimiter, 0);
		pos=line.find_first_not_of(delimiter, lasPos);
		int Random_seed=atoi((line.substr(pos)).c_str());
		getline(inFile, line);
		lasPos=line.find_first_of(delimiter, 0);
		pos=line.find_first_not_of(delimiter, lasPos);
		int max_rubble=atoi((line.substr(pos)).c_str());
		getline(inFile, line);
		lasPos=line.find_first_of(delimiter, 0);
		pos=line.find_first_not_of(delimiter, lasPos);
		int start_x=atoi((line.substr(pos)).c_str());
		getline(inFile, line);
		lasPos=line.find_first_of(delimiter, 0);
		pos=line.find_first_not_of(delimiter, lasPos);
		int start_y=atoi((line.substr(pos)).c_str());
		std::mt19937 gen(Random_seed);
		std::uniform_int_distribution<> dis(0, max_rubble);
		information inforTmp;
		for(int j=0;j<size;j++)
		{
			for(int i=0;i<size;i++)
			{
				inforTmp.dynaQuant=dis(gen);
				inforTmp.x=i;
				inforTmp.y=j;
				inforTmp.flag=false;
				routeMap[(size-j-1)*(size)+i]=inforTmp;//fill the routeMap 
			}
		}
		routeMap[(size-start_y-1)*size+start_x].dynaQuant=0; // set the dynamite needed to be zero in the starting location
		start.x=start_x;
		start.y=start_y;
		start.dynaQuant=0;
		start.flag=false;
	}
}

void mineField::findEscape()
{
	// determine the heap type to store the possible rubbers to blast away
	if(heapType=="BINARY")
	{
		binary_heap<information*, inforComp> rubberheap;
		deque<information*> accessible; // the accessible slots;
		int row;
		int col;
		accessible.push_back(&start);
		while(1)
		{
			while(!accessible.empty())
			{
				row=accessible.front()->x; // notice here x,y and col,row correspondence is reversed!
				col=accessible.front()->y;
				if(row==0 || row==size-1 || col==0 || col==size-1)
				{
					whetherEsc=true;
					break;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant>0 && routeMap[(size-col-1)*size+row+1].flag==false) //right rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant==0 && routeMap[(size-col-1)*size+row+1].flag==false) //right slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant>0 && routeMap[(size-col-1)*size+row-1].flag==false) //left rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant==0 && routeMap[(size-col-1)*size+row-1].flag==false) //left slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant>0 && routeMap[(size-col-2)*size+row].flag==false) //upper rubber search
				{
					rubberheap.push(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant==0 && routeMap[(size-col-2)*size+row].flag==false) //upper slot search
				{
					accessible.push_back(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant>0 && routeMap[(size-col)*size+row].flag==false) //lower rubber search
				{
					rubberheap.push(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant==0 && routeMap[(size-col)*size+row].flag==false) //lower slot search
				{
					accessible.push_back(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				accessible.pop_front();
			}
			if(whetherEsc==true) // successfully escape out 
				break;
			if(dynamite-rubberheap.top()->dynaQuant>=0)
			{
				col=rubberheap.top()->x;
				row=rubberheap.top()->y;
				dynamite-=rubberheap.top()->dynaQuant;
				blasted.push_back(&routeMap[(size-row-1)*size+col]);
				blastedNum++;
				accessible.push_back(&routeMap[(size-row-1)*size+col]);
				rubberheap.pop();
			}
			else // not enouth sticks of dynamite to blast away the rubber
				break;
		}
	}
if(heapType=="SORTED")
	{
		sorted_heap<information*, inforComp> rubberheap;
		deque<information*> accessible; // the accessible slots;
		int row;
		int col;
		accessible.push_back(&start);
		while(1)
		{
			while(!accessible.empty())
			{
				row=accessible.front()->x;
				col=accessible.front()->y;
				if(row==0 || row==size-1 || col==0 || col==size-1)
				{
					whetherEsc=true;
					break;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant>0 && routeMap[(size-col-1)*size+row+1].flag==false) //right rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant==0 && routeMap[(size-col-1)*size+row+1].flag==false) //right slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant>0 && routeMap[(size-col-1)*size+row-1].flag==false) //left rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant==0 && routeMap[(size-col-1)*size+row-1].flag==false) //left slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant>0 && routeMap[(size-col-2)*size+row].flag==false) //upper rubber search
				{
					rubberheap.push(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant==0 && routeMap[(size-col-2)*size+row].flag==false) //upper slot search
				{
					accessible.push_back(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant>0 && routeMap[(size-col)*size+row].flag==false) //lower rubber search
				{
					rubberheap.push(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant==0 && routeMap[(size-col)*size+row].flag==false) //lower slot search
				{
					accessible.push_back(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				accessible.pop_front();
			}
			if(whetherEsc==true) // successfully escape out 
				break;
			if(dynamite-rubberheap.top()->dynaQuant>=0)
			{
				col=rubberheap.top()->x;
				row=rubberheap.top()->y;
				dynamite-=rubberheap.top()->dynaQuant;
				blasted.push_back(&routeMap[(size-row-1)*size+col]);
				blastedNum++;
				accessible.push_back(&routeMap[(size-row-1)*size+col]);
				rubberheap.pop();
			}
			else // not enouth sticks of dynamite to blast away the rubber
				break;
		}
	}
if(heapType=="POOR_MAN")
	{
		poorman_heap<information*, inforComp> rubberheap;
		deque<information*> accessible; // the accessible slots;
		int row;
		int col;
		accessible.push_back(&start);
		while(1)
		{
			while(!accessible.empty())
			{
				row=accessible.front()->x;
				col=accessible.front()->y;
				if(row==0 || row==size-1 || col==0 || col==size-1)
				{
					whetherEsc=true;
					break;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant>0 && routeMap[(size-col-1)*size+row+1].flag==false) //right rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant==0 && routeMap[(size-col-1)*size+row+1].flag==false) //right slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant>0 && routeMap[(size-col-1)*size+row-1].flag==false) //left rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant==0 && routeMap[(size-col-1)*size+row-1].flag==false) //left slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant>0 && routeMap[(size-col-2)*size+row].flag==false) //upper rubber search
				{
					rubberheap.push(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant==0 && routeMap[(size-col-2)*size+row].flag==false) //upper slot search
				{
					accessible.push_back(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant>0 && routeMap[(size-col)*size+row].flag==false) //lower rubber search
				{
					rubberheap.push(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant==0 && routeMap[(size-col)*size+row].flag==false) //lower slot search
				{
					accessible.push_back(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				accessible.pop_front();
			}
			if(whetherEsc==true) // successfully escape out 
				break;
			if(dynamite-rubberheap.top()->dynaQuant>=0)
			{
				col=rubberheap.top()->x;
				row=rubberheap.top()->y;
				dynamite-=rubberheap.top()->dynaQuant;
				blasted.push_back(&routeMap[(size-row-1)*size+col]);
				blastedNum++;
				accessible.push_back(&routeMap[(size-row-1)*size+col]);
				rubberheap.pop();
			}
			else // not enouth sticks of dynamite to blast away the rubber
				break;
		}
	}
if(heapType=="PAIRING")
	{
		pairing_heap<information*, inforComp> rubberheap;
		deque<information*> accessible; // the accessible slots;
		int row;
		int col;
		accessible.push_back(&start);
		while(1)
		{
			while(!accessible.empty())
			{
				row=accessible.front()->x;
				col=accessible.front()->y;
				if(row==0 || row==size-1 || col==0 || col==size-1)
				{
					whetherEsc=true;
					break;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant>0 && routeMap[(size-col-1)*size+row+1].flag==false) //right rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row+1].dynaQuant==0 && routeMap[(size-col-1)*size+row+1].flag==false) //right slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row+1]);
					routeMap[(size-col-1)*size+row+1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant>0 && routeMap[(size-col-1)*size+row-1].flag==false) //left rubber search
				{
					rubberheap.push(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-1)*size+row-1].dynaQuant==0 && routeMap[(size-col-1)*size+row-1].flag==false) //left slot search
				{
					accessible.push_back(&routeMap[(size-col-1)*size+row-1]);
					routeMap[(size-col-1)*size+row-1].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant>0 && routeMap[(size-col-2)*size+row].flag==false) //upper rubber search
				{
					rubberheap.push(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col-2)*size+row].dynaQuant==0 && routeMap[(size-col-2)*size+row].flag==false) //upper slot search
				{
					accessible.push_back(&routeMap[(size-col-2)*size+row]);
					routeMap[(size-col-2)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant>0 && routeMap[(size-col)*size+row].flag==false) //lower rubber search
				{
					rubberheap.push(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				if(routeMap[(size-col)*size+row].dynaQuant==0 && routeMap[(size-col)*size+row].flag==false) //lower slot search
				{
					accessible.push_back(&routeMap[(size-col)*size+row]);
					routeMap[(size-col)*size+row].flag=true;
				}
				accessible.pop_front();
			}
			if(whetherEsc==true) // successfully escape out 
				break;
			if(dynamite-rubberheap.top()->dynaQuant>=0)
			{
				col=rubberheap.top()->x;
				row=rubberheap.top()->y;
				dynamite-=rubberheap.top()->dynaQuant;
				blasted.push_back(&routeMap[(size-row-1)*size+col]);
				blastedNum++;
				accessible.push_back(&routeMap[(size-row-1)*size+col]);
				rubberheap.pop();
			}
			else // not enouth sticks of dynamite to blast away the rubber
				break;
		}
	}
}
