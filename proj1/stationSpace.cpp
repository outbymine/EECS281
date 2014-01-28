#include<getopt.h>
#include<cstdlib>
#include<string>
#include<vector>
#include<string>
#include<iostream>
#include<deque>
#include<cstring>
#include<sstream>
#include"stationSpace.h"
using namespace std;

void help()//print out the helping information and then exit the program
{
	cout<<"This program is designed to help to find the way back"<<endl;
	cout<<"should indicate the route file name and output escape file name at the end of command-line arguments"<<endl;
	cout<<"-h or --help: to show the help message and the exit the program"<<endl;
	cout<<"-s or --stack: use the stack based routing scheme"<<endl;
	cout<<"-q or --queue: use the queue based routing schema"<<endl;
	cout<<"-o or --output: show the format of the output file by the command-line arguments, these arguments not optional"<<endl;
}

bool isLegalChar(char character)
{
	switch(character)
	{
		case '.':
		case '#':
		case 'S':
		case 'H':
		case 'E':
			return true;
			break;
		default: 
			return false;
			break;
	}
}

stationSpace::stationSpace()
{
}

stationSpace::stationSpace(int argc, char **argv)
{
	int c;
	while((c=getopt_long(argc,argv,"hsqo:",long_options,NULL))!=-1)
	{
		switch(c)	
		{
		case 'h':
			help();
			exit(0);
		case 's':
			if(SS.routeScheme=='s' || SS.routeScheme=='q')
			{
				cerr<<"should not specify the routing scheme twice";
				exit(1);
			}
			else
			{
				SS.routeScheme='s';
				break;
			}
		case 'q':
			if(SS.routeScheme=='s' || SS.routeScheme=='q')
			{
				cerr<<"should not specify the routing scheme twice"<<endl;
				exit(1);
			}
			else
			{
				SS.routeScheme='q';
				break;
			}
		case 'o':
			if (optarg[0] == 'M')
				SS.outMode="M";
			else
				SS.outMode="L";
			break;
		case '?':
			if(isprint(optopt))
			fprintf(stderr,"Unknown option '-%c'.\n",optopt);
			else
			fprintf(stderr,"Unknown option character '\\x%x'.\n", optopt);
			exit(1);
		default:
			abort();
		}
	}
	if(SS.routeScheme!='s' && SS.routeScheme!='q')
	{
		cerr<<"should specify a route scheme";
		exit(1);
	}
	cin>>SS.inMode>>SS.dim>>SS.level;
	string line;
	getline(cin,line);
	const location fillElem={0,0,0,'.'};//
	SS.routeMap.assign(SS.level*SS.dim*SS.dim,fillElem);
	SS.escape.assign(SS.level*SS.dim*SS.dim,fillElem);
	if(SS.inMode=="M")
	{
		int totalRowNum=SS.level*SS.dim;
		while(getline(cin,line))
		{
			if(line[0]!='/')
			{
				totalRowNum--;
				for(int i=0;i<SS.dim;i++)
				{
					if(isLegalChar(line[i]))
					{
						int col=i;
						int levelNum=totalRowNum/SS.dim;
						int row=totalRowNum%SS.dim;//notice that I count the row number from the bottom to top
						SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].row=row;
						SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].col=col;
						SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].levelNum=levelNum;
						SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].character=line[i];
						if(line[i]=='S')
						{
							start.row=row;
							start.col=col;
							start.levelNum=levelNum;
							start.character=line[i];
						}
					}
					else
					{
						cerr<<"illegal character input";
						exit(1);
					}
				}			
			}
		}
	}
	if(SS.inMode=="L")
	{//initialize the routeMap
		for(int i=0;i<SS.level;i++)//levelNum
		{
			for(int j=0;j<SS.dim;j++)//rowNum
			{
				for(int k=0;k<SS.dim;k++)//colNum
				{
					SS.routeMap[i*SS.dim*SS.dim+j*SS.dim+k].row=j;
					SS.routeMap[i*SS.dim*SS.dim+j*SS.dim+k].col=k;
					SS.routeMap[i*SS.dim*SS.dim+j*SS.dim+k].levelNum=i;
					SS.routeMap[i*SS.dim*SS.dim+j*SS.dim+k].character='.';
				}
			}
		}
		const string delimiter=",()";
		string numString;//extracted portion from the string, ready to convert to numbers
		int num;//the value of row/col/levelNum
		string::size_type lasPos, pos;
		while(getline(cin,line))
		{
			if(line[0]!='/')
			{
				//skip delimiter, namely "(" at the beginning of the line
				lasPos=line.find_first_not_of(delimiter,0);
				//find the first non-deliminiter
				pos=line.find_first_of(delimiter,lasPos);
				numString=line.substr(lasPos,pos-lasPos);
				num=atoi(numString.c_str());
				int row=num;
				row=SS.dim-row-1;
				if(row<0 || row>SS.dim-1)
					{
						cerr<<"the index is out of scope";
						exit(1);
					}
				lasPos=line.find_first_not_of(delimiter,pos);
				pos=line.find_first_of(delimiter,lasPos);
				numString=line.substr(lasPos,pos-lasPos);
				num=atoi(numString.c_str());
				int col=num;
				if(col<0 || col>SS.dim-1)
					{
						cerr<<"the index is out of scope";
						exit(1);
					}
				lasPos=line.find_first_not_of(delimiter,pos);
				pos=line.find_first_of(delimiter,lasPos);
				numString=line.substr(lasPos,pos-lasPos);
				num=atoi(numString.c_str());
				int levelNum=num;
				if(levelNum<0 || levelNum>SS.level-1)
					{
						cerr<<"the index is out of scope";
						exit(1);
					}
				
				char character=line[pos+1];
				if(!isLegalChar(character))
				{
					cerr<<"input character is illegal";
					exit(1);
				}
				SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].row=row;
				SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].col=col;
				SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].levelNum=levelNum;
				SS.routeMap[levelNum*SS.dim*SS.dim+row*SS.dim+col].character=character;
				if(character=='S')
				{
					start.row=row;
					start.col=col;
					start.levelNum=levelNum;
					start.character=character;
				}
			}
		}
	}
}

stationSpace::~stationSpace()
{
}

location stationSpace::findEscape()//the return value is the location of the hanger, and in the SS.routeMap,  Note that if
	                           //there is no route exist, the character of the location correspond to the return position is not a hanger.
{
	deque<location> findRoute;
	findRoute.push_front(start);
	location popElem;
	location pushElem;
	int newPos;
	newPos=start.levelNum*SS.dim*SS.dim+start.row*SS.dim+start.col;//the starting location
	SS.escape[newPos].row=start.row;
	SS.escape[newPos].col=start.col;
	SS.escape[newPos].levelNum=start.levelNum;
	SS.escape[newPos].character='S';
	while(!findRoute.empty())
	{
		if(SS.routeScheme=='s')//if the routeScheme is stack
		{
			popElem=findRoute.front();
			findRoute.pop_front();
		}
		else//if the routeScheme is a queue
		{
			popElem=findRoute.back();
			findRoute.pop_back();
		}
		if(1)
		{
			if(popElem.row<SS.dim-1)//check whether there is nouth element
			{
				newPos=popElem.levelNum*SS.dim*SS.dim+(popElem.row+1)*SS.dim+popElem.col;//the new potential position 
				if(SS.routeMap[newPos].character!='#' && SS.escape[newPos].character=='.')//check whether the new position is a wall and whether it's a passed element.
				{
					pushElem.row=popElem.row+1;
					pushElem.col=popElem.col;
					pushElem.levelNum=popElem.levelNum;
					pushElem.character=SS.routeMap[newPos].character;
					findRoute.push_front(pushElem);
					SS.escape[newPos].row=popElem.row+1;
					SS.escape[newPos].col=popElem.col;
					SS.escape[newPos].levelNum=popElem.levelNum;
					SS.escape[newPos].character='n';
					if(SS.routeMap[newPos].character=='H')
						return SS.routeMap[newPos]; //the hanger is found, so exit and break out of the while loop;
				}
			}
			if(popElem.col<SS.dim-1)//check whether there is east element
			{
				newPos=popElem.levelNum*SS.dim*SS.dim+popElem.row*SS.dim+popElem.col+1;//the new potential position  
				if(SS.routeMap[newPos].character!='#' && SS.escape[newPos].character=='.')//check whether the new position is a wall and whether it's a passed element.
				{
					pushElem.row=popElem.row;
					pushElem.col=popElem.col+1;
					pushElem.levelNum=popElem.levelNum;
					pushElem.character=SS.routeMap[newPos].character;
					findRoute.push_front(pushElem);
					SS.escape[newPos].row=popElem.row;
					SS.escape[newPos].col=popElem.col+1;
					SS.escape[newPos].levelNum=popElem.levelNum;
					SS.escape[newPos].character='e';
					if(SS.routeMap[newPos].character=='H')
						return SS.routeMap[newPos];  //the hanger is found, so exit and break out of the while loop;
				}
			}
			if(popElem.row!=0)//check whether there is south element
			{
				newPos=popElem.levelNum*SS.dim*SS.dim+(popElem.row-1)*SS.dim+popElem.col;//the new potential position  
				if(SS.routeMap[newPos].character!='#' && SS.escape[newPos].character=='.')//check whether the new position is a wall and whether it's a passed element.
				{
					pushElem.row=popElem.row-1;
					pushElem.col=popElem.col;
					pushElem.levelNum=popElem.levelNum;
					pushElem.character=SS.routeMap[newPos].character;
					findRoute.push_front(pushElem);
					SS.escape[newPos].row=popElem.row-1;
					SS.escape[newPos].col=popElem.col;
					SS.escape[newPos].levelNum=popElem.levelNum;
					SS.escape[newPos].character='s';
					if(SS.routeMap[newPos].character=='H')
						return SS.routeMap[newPos]; //the hanger is found, so exit and break out of the while loop;
				}
			}
			if(popElem.col!=0)//check whether there is west element
			{
				newPos=popElem.levelNum*SS.dim*SS.dim+popElem.row*SS.dim+popElem.col-1;//the new potential position  
				if(SS.routeMap[newPos].character!='#' && SS.escape[newPos].character=='.')//check whether the new position is a wall and whether it's a passed element.
				{
					pushElem.row=popElem.row;
					pushElem.col=popElem.col-1;
					pushElem.levelNum=popElem.levelNum;
					pushElem.character=SS.routeMap[newPos].character;
					findRoute.push_front(pushElem);
					SS.escape[newPos].row=popElem.row;
					SS.escape[newPos].col=popElem.col-1;
					SS.escape[newPos].levelNum=popElem.levelNum;
					SS.escape[newPos].character='w';
					if(SS.routeMap[newPos].character=='H')
						return SS.routeMap[newPos];  //the hanger is found, so exit and break out of the while loop;
				}
			}
		}
		if(popElem.character=='E')//if encounters an elevator
		{
			int currentLevelNum=popElem.levelNum;
			int levelElevator=0;
			stringstream eleLevel;
			char charc;//when encounters an elevator, the escape route records from which level it comes, rather than a 'E';
			while(levelElevator<SS.level)//enqueue/push all elevator tiles from 0 to the top if the same location is an elevator.
			{
				if(levelElevator!=currentLevelNum && SS.escape[(levelElevator)*SS.dim*SS.dim+popElem.row*SS.dim+popElem.col].character=='.' 
				&& SS.routeMap[(levelElevator)*SS.dim*SS.dim+popElem.row*SS.dim+popElem.col].character=='E')
				//note that should not enqueue/push the starting elevator position twice
				{
					newPos=(levelElevator)*SS.dim*SS.dim+popElem.row*SS.dim+popElem.col;
					pushElem.row=popElem.row;
					pushElem.col=popElem.col;
					pushElem.levelNum=levelElevator;
					pushElem.character=SS.routeMap[newPos].character;
					findRoute.push_front(pushElem);
					SS.escape[newPos].row=pushElem.row;
					SS.escape[newPos].col=pushElem.col;
					SS.escape[newPos].levelNum=levelElevator;
					eleLevel.clear();
					eleLevel.str("");
					eleLevel<<levelElevator;
					eleLevel>>charc;
					SS.escape[newPos].character=charc;
				}
				levelElevator++;
			}
		}
	}
	return SS.routeMap[newPos];
}

int stationSpace::directBack(const location &L_route)
{
	int position=L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
	int prePos;//through which position the current position is got
	location L_escape=SS.escape[position];
	int currentLevel=L_route.levelNum;
	switch(L_escape.character)
	{
	case 'n':
		return L_route.levelNum*SS.dim*SS.dim+(L_route.row-1)*SS.dim+L_route.col;
	case 'e':
		return L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col-1;
	case 's':
		return L_route.levelNum*SS.dim*SS.dim+(L_route.row+1)*SS.dim+L_route.col;
	case 'w':
		return L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col+1;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		while(--currentLevel>=0)
		{
			prePos=currentLevel*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
			if(SS.routeMap[prePos].character=='E')
			{
				if(SS.escape[prePos].character=='n' || SS.escape[prePos].character=='e' || 
				SS.escape[prePos].character=='s' || SS.escape[prePos].character=='w')
				{
					return prePos;
				}
			}
		}
		currentLevel=L_route.levelNum;
		while(++currentLevel<SS.level)
		{
			prePos=currentLevel*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
			if((SS.routeMap[prePos].character=='E') && (SS.escape[prePos].character=='n' || SS.escape[prePos].character=='e' || 
			SS.escape[prePos].character=='s' || SS.escape[prePos].character=='w'))
			{
				return prePos;
			}
		}
	default:
		cerr<<"illegal character, error";
		exit(1);
	}
}
void stationSpace::output(location &H)
{
	cout<<SS.dim<<endl;
	cout<<SS.level<<endl;
        int position;
	int positionOld;
	if(SS.outMode.empty() || SS.outMode=="M")//the output mode is map
	{
		if(H.character=='H')//if escape is possible and the route is found, change the routeMap then output to file
			                //if escape is not possible, output the routeMap to the file directly
		{
			location L_route=H;
			position=L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
			while(SS.escape[position].character!='S')
			{
				L_route=SS.routeMap[directBack(L_route)];
				positionOld=position;
				position=L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
				SS.routeMap[position].character=SS.escape[positionOld].character;
			}
		}
		for(int i=SS.level-1;i>=0;i--)//levelNum
		{
			cout<<"//level "<<i<<endl;
			for(int j=SS.dim-1;j>=0;j--)//rowNum
			{
				for(int k=0;k<SS.dim;k++)//colNum
				{
					cout<<SS.routeMap[i*SS.dim*SS.dim+j*SS.dim+k].character;
				}
				cout<<endl;
			}
		}
	}
	else//if the output mode is a list
	{
		cout<<"//path taken"<<endl;
		if(H.character=='H')//if escape is possible and the route is found, change the routeMap then output to file
			                //if escape is not possible, out put nothing after "path taken"
		{
			location L_route=H;
			position=L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
			stringstream ss;
			string convert;
			vector<string> order;//store the route from the hanger to the starting poit in this vector
			while(SS.escape[position].character!='S')
			{
				L_route=SS.routeMap[directBack(L_route)];
				positionOld=position;
				position=L_route.levelNum*SS.dim*SS.dim+L_route.row*SS.dim+L_route.col;
				SS.routeMap[position].character=SS.escape[positionOld].character;
				ss.clear();
				ss.str("");
				ss<<"("<<SS.dim-SS.routeMap[position].row-1<<","<<SS.routeMap[position].col
					<<","<<SS.routeMap[position].levelNum<<","<<SS.routeMap[position].character
					<<")";
				convert=ss.str();
				order.push_back(convert);
			}
			for(int i=order.size()-1;i>=0;i--)
			{
				cout<<order[i]<<endl;	
			}
		}
	}
}



