#include"mapinfor.h"

adjMat::adjMat(int argc, char **argv)
{
	int c;
	while((c=getopt_long(argc, argv, "hm:", long_options, NULL))!=-1)
	{
		switch(c)
		{
		case 'h':
			cout<<"when the mode is MST, we should output the minimum spanning tree"<<endl;
			cout<<"when the mode is OPTTSP, we should output the optimal rover exploration plan"<<endl;
			cout<<"when the mode is FASTTSP, we should output the optimal rover exploration plan we can get in a finite bound of time"<<endl;
			exit(0);
		case 'm':
			mode=string(optarg);
			break;
		}
	}
	string line;
	getline(cin,line);
	baseNum=atoi(line.c_str());
	string::size_type lasPos, pos;
	string delimiter=" 	";
	location tmp;
	for(int i=0; i<baseNum; i++)
	{
		getline(cin,line);
		lasPos=line.find_first_not_of(delimiter, 0);
		pos=line.find_first_of(delimiter,lasPos);
		tmp.x=atoi(line.substr(lasPos, pos-lasPos).c_str());
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter,lasPos);
		tmp.y=atoi(line.substr(lasPos, pos-lasPos).c_str());
		lasPos=line.find_first_not_of(delimiter, pos);
		pos=line.find_first_of(delimiter,lasPos);
		tmp.height=atoi(line.substr(lasPos, pos-lasPos).c_str());
		mapLocation.push_back(tmp);
	}
	if(mode!="MST")
	{
		for(int i=0;i<baseNum;i++)
		{
			vector<int> tmp;
			for(int j=0;j<baseNum;j++)
			{
				tmp.push_back(computeDist(mapLocation[i],mapLocation[j]));
			}
			adjMatrix.push_back(tmp);
		}
		mapLocation.clear(); // release the unnecessary information
	}
}

int computeDist(const location& l1, const location& l2)
{
	return (abs(l1.x-l2.x)+abs(l1.y-l2.y)+abs(l1.height-l2.height));
}
