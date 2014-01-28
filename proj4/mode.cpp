#include"mode.h"

vector<int> computeMST(const adjMat& adjMat1)
{
	if(adjMat1.adjMatrix.empty())
	{
		vector<int> kv;
		vector<int> dv;
		vector<int> pv;
		int vertexNum=adjMat1.baseNum;
		dv.assign(vertexNum, INT_MAX);
		dv[0]=0;
		kv.assign(vertexNum, 0);
		kv[0]=1;
		pv.assign(vertexNum, 0);
		for(int i=1;i<vertexNum;i++)
		{
			dv[i]=computeDist(adjMat1.mapLocation[0], adjMat1.mapLocation[i]);
		}
		for(int j=1;j<vertexNum;j++) // vertexNum-1 times then the Prim's algorithm is finished
		{
			int smallestIndex=0;
			int smallestDist=INT_MAX;
			for(int i=0;i<vertexNum;i++)
			{
				if(kv[i]==0 && dv[i]<smallestDist)
				{
					smallestIndex=i;
					smallestDist=dv[i];
				}		
			}
			kv[smallestIndex]=1;	
			for(int i=0;i<vertexNum;i++)
			{
				if(kv[i]==0 && dv[i]>computeDist(adjMat1.mapLocation[smallestIndex], adjMat1.mapLocation[i]))		
				{
					dv[i]=computeDist(adjMat1.mapLocation[smallestIndex], adjMat1.mapLocation[i]);
					pv[i]=smallestIndex;
				}
			}
		}
		return pv;
	}
	else
	{
		vector<int> kv;
		vector<int> dv;
		vector<int> pv;
		int vertexNum=adjMat1.baseNum;
		dv.assign(vertexNum, INT_MAX);
		dv[0]=0;
		kv.assign(vertexNum, 0);
		kv[0]=1;
		pv.assign(vertexNum, 0);
		for(int i=1;i<vertexNum;i++)
		{
			dv[i]=adjMat1.adjMatrix[0][i];
		}
		for(int j=1;j<vertexNum;j++) // vertexNum-1 times then the Prim's algorithm is finished
		{
			int smallestIndex=0;
			int smallestDist=INT_MAX;
			for(int i=0;i<vertexNum;i++)
			{
				if(kv[i]==0 && dv[i]<smallestDist)
				{
					smallestIndex=i;
					smallestDist=dv[i];
				}		
			}
			kv[smallestIndex]=1;	
			for(int i=0;i<vertexNum;i++)
			{
				if(kv[i]==0 && dv[i]>adjMat1.adjMatrix[smallestIndex][i])
				{
					dv[i]=adjMat1.adjMatrix[smallestIndex][i];
					pv[i]=smallestIndex;
				}
			}
		}
		return pv;	
	}
}

void printMST(const adjMat& adjMat1, const vector<int>& path)
{
	int totalDist=0; // the total distance
	for(int i=1; i<adjMat1.baseNum; i++)
	{
		totalDist+=computeDist(adjMat1.mapLocation[i], adjMat1.mapLocation[path[i]]);
	}
	cout<<totalDist;
	for(int i=1; i<adjMat1.baseNum; i++)
	{
		cout<<endl;
		if(path[i]<i)
			cout<<path[i]<<" "<<i;
		else
			cout<<i<<" "<<path[i];
	}
}

vector<int> pathFromMST(const vector<int>& path) // for each element in the vector "path", the index is its ancester
{
	int pathSize=path.size();
	vector<int> MSTTraverse;
	MSTTraverse.push_back(0);
	int tmp;
	vector<vector<int> > childList(pathSize); // the each index, the element vector is its children
	for(int i=1; i<pathSize; i++)
	{
		childList[path[i]].push_back(i);
	}
	int currentBase=0;
	for(int i=1; i<pathSize; i++)
	{
		while(childList[currentBase].empty()) // track back to the vertex that has child and visit this child
		{
			if(currentBase==0)
				break;
			currentBase=path[currentBase]; // track back to its parent until the parent of the currentBase has at least one unvisited child
		}
		MSTTraverse.push_back(childList[currentBase].back());
		tmp=childList[currentBase].back();
		childList[currentBase].pop_back();
		currentBase=tmp;
	}
	return MSTTraverse;
}

void optFromMST(const adjMat& adjMat1, vector<int>& currentPath, int& cursorPos, int& totalDist)
{
	int baseNum=adjMat1.baseNum;
	while(cursorPos<baseNum)
	{
		for(int dist=2; dist<baseNum-1; dist++)
		{
			if(exchangeVertex(adjMat1, currentPath, baseNum, cursorPos, dist, totalDist))
			{
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
				cout<<currentPath[cursorPos]<<" "<<currentPath[(cursorPos+1)%baseNum]<<" "<<currentPath[(cursorPos+dist)%baseNum]<<" "<<currentPath[(cursorPos+dist+1)%baseNum]<<endl;
				int totalDist=0;
				for(int i=0;i<adjMat1.baseNum-1;i++)
				{
					int vertex1=currentPath[i];
					int vertex2=currentPath[i+1];
					totalDist+=adjMat1.adjMatrix[vertex1][vertex2];
				}
				totalDist+=adjMat1.adjMatrix[currentPath[0]][currentPath[adjMat1.baseNum-1]];
				cout<<totalDist<<endl;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
				optFromMST(adjMat1, currentPath, cursorPos, totalDist);
				return;
			}
		}
		cursorPos++;
	}
}


bool exchangeVertex(const adjMat& adjMat1, vector<int>& currentPath, int baseNum, int cursorPos, int dist, int& totalDist)
{
	vector<int> pathAfterAdjust=currentPath;
	swap(pathAfterAdjust[(cursorPos+1)%baseNum], pathAfterAdjust[(cursorPos+dist)%baseNum]);
	int newDist=totalDistance(adjMat1, pathAfterAdjust);	
	if(totalDist>newDist)
	{
/////////////////////////////////////////////////////////////////////////////////////////////
		//for(int i=0; i<baseNum; i++)/////////////////////////////////////////////////
			//cout<<currentPath[i]<<"";////////////////////////////////////////////
		//cout<<endl;//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

		swap(currentPath[(cursorPos+1)%baseNum], currentPath[(cursorPos+dist)%baseNum]);
		totalDist=newDist;
/////////////////////////////////////////////////////////////////////////////////////////////
		//for(int i=0; i<baseNum; i++)/////////////////////////////////////////////////
			//cout<<currentPath[i]<<"";////////////////////////////////////////////
		//cout<<endl;//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
		return true;
	}
	else 
		return false;
}

int totalDistance(const adjMat& adjMat1, const vector<int>& currentPath)
{
	int totalDist=0;
	int vertex1, vertex2;
	for(int i=0;i<adjMat1.baseNum-1;i++)
	{
		vertex1=currentPath[i];
		vertex2=currentPath[i+1];
		totalDist+=adjMat1.adjMatrix[vertex1][vertex2];
	}
	totalDist+=adjMat1.adjMatrix[currentPath[0]][currentPath[adjMat1.baseNum-1]];
	return totalDist;
	
}

void printFASTTSP(const adjMat& adjMat1, const vector<int>& pathFASTTSP)
{
	int totalDist=0;
	int vertex1, vertex2;
	for(int i=0;i<adjMat1.baseNum-1;i++)
	{
		vertex1=pathFASTTSP[i];
		vertex2=pathFASTTSP[i+1];
		totalDist+=adjMat1.adjMatrix[vertex1][vertex2];
	}
	totalDist+=adjMat1.adjMatrix[0][pathFASTTSP[adjMat1.baseNum-1]];
	cout<<totalDist<<endl;
	cout<<pathFASTTSP[0];
	for(int i=1;i<adjMat1.baseNum;i++)
	{
		cout<<" "<<pathFASTTSP[i];
	}
}

vector<unsigned long long> factorialVec(int baseNum)
{
	vector<unsigned long long> factorialRecord(baseNum);
	for(int i=0; i<baseNum; i++)
	{
		factorialRecord[i]=factorial(i);
	}
	return factorialRecord;

}

/*
vector<int> nthFactorial(const vector<unsigned long long>& factorialRecord, int baseNum, unsigned long long nth)
{
	vector<int> orderBook(baseNum);
	for(int i=0; i<baseNum; i++)
		orderBook[i]=i;
	vector<int> permutation;
	unsigned int i;
	while(true)
	{
		if(orderBook.size()==1)
		{
			permutation.push_back(orderBook[0]);
			break;
		}
		unsigned long long factorialNum=factorialRecord[orderBook.size()-1];
		for(i=1; i<orderBook.size(); i++)
		{
			if(i*factorialNum>=nth)
				break;
		}
		permutation.push_back(orderBook[i-1]);
		orderBook.erase(orderBook.begin()+i-1);
		nth-=(i-1)*factorialNum;
	}
	return permutation;
}
*/

vector<int> nthFactorial(const vector<unsigned long long>& factorialRecord, int baseNum, unsigned long long nth)
{
	vector<int> orderBook(baseNum);
	for(int i=0; i<baseNum; i++)
		orderBook[i]=i;
	vector<int> permutation;
	nthFactorial(nth, factorialRecord, orderBook, permutation);
	return permutation;
}

void nthFactorial(unsigned long long& reminder, const vector<unsigned long long>& factorialRecord, vector<int>& orderBook, vector<int>& permutation)
{
	if(orderBook.size()==1)
	{
		permutation.push_back(orderBook[0]);
		return;
	}
	unsigned long long factorialNum=factorialRecord[orderBook.size()-1];
	unsigned int i;
	for(i=1; i<orderBook.size(); i++)
	{
		if(i*factorialNum>=reminder)
			break;
	}
	permutation.push_back(orderBook[i-1]);
	orderBook.erase(orderBook.begin()+i-1);
	reminder-=(i-1)*factorialNum;
	nthFactorial(reminder, factorialRecord, orderBook, permutation);
}

vector<int> nextCheckpoint(vector<int>& data, int checkPos)
{
	if(*max_element(data.begin()+checkPos+1, data.end())<data[checkPos])
	{
		while(data[checkPos]<data[checkPos-1])
			checkPos--;
		checkPos--;
	}
	if(checkPos==0)
		return data;
	int currentNext=INT_MAX;
	int pos=0;
	for(unsigned int i=checkPos+1; i<data.size(); i++)
	{
		if(data[i]<currentNext && data[i]>data[checkPos])
		{
			currentNext=data[i];
			pos=i;
		}
	}
	swap(data[pos], data[checkPos]);
	sort(data.begin()+checkPos+1, data.end());
	return data;
}

vector<vector<int> > sortedDistanceMatrix(const adjMat& adjMat1)
{
	vector<vector<int> > smallest2(adjMat1.baseNum);
	vector<vector<int> > tmpMat=adjMat1.adjMatrix;
	for(int i=0; i<adjMat1.baseNum; i++)
	{
		tmpMat[i][i]=INT_MAX;
		sort(tmpMat[i].begin(), tmpMat[i].end());
		tmpMat[i][1]=tmpMat[i][0]+tmpMat[i][1];
	}
	for(int i=0; i<adjMat1.baseNum; i++)
	{
		smallest2[i].resize(2);
		smallest2[i][0]=tmpMat[i][0];
		smallest2[i][1]=tmpMat[i][1];
	}
	return smallest2;
}

int computeLowerBound(const vector<vector<int> >& SDM, const vector<vector<int> >& weightMat, int baseNum, const vector<int>& data, int i, int& circleLength)
{
	int j=i+1;
	int lowerBound=0;
	lowerBound+=SDM[0][0];
	lowerBound+=SDM[data[i+1]][0];
	circleLength+=weightMat[data[j-1]][data[j]];
	lowerBound+=2*circleLength;	
	for(j=i+2; j<baseNum; j++)
	{
		lowerBound+=SDM[data[j]][1];
	}
	return lowerBound/2;
}

int computeLowerBoundPQ(const vector<vector<int> >& SDM, const vector<vector<int> >& weightMat, int baseNum, const vector<int>& data, int i)
{
	int circleLength=0;
	int lowerBound=0;
	int j;
	for(j=0; j<i+1; j++)
		circleLength+=weightMat[data[j]][data[j+1]];
	lowerBound+=SDM[0][0];
	lowerBound+=SDM[data[i+1]][0];
	lowerBound+=2*circleLength;	
	for(j=i+2; j<baseNum; j++)
	{
		lowerBound+=SDM[data[j]][1];
	}
	return lowerBound/2;
}

vector<int> findOPTTSP(const adjMat& adjMat1, int& upperBound, vector<int>& OPTTSPPath)
{
	vector<vector<int> > SDM=sortedDistanceMatrix(adjMat1);
	int baseNum=adjMat1.baseNum;
	vector<unsigned long long> factorialRecord=factorialVec(baseNum);
	vector<int> data(baseNum);
	iota(data.begin(),data.end(),0);
	vector<vector<int> > weightMat=adjMat1.adjMatrix;
	int divideNum=3;
	if(baseNum<10)
		divideNum=1;
	unsigned long long subDist=factorialRecord[baseNum-divideNum-1];
	priority_queue<subset, vector<subset>, subsetComp> pq;
	int pqSize=factorialRecord[baseNum-1]/factorialRecord[baseNum-divideNum-1];
	subset tmp;
	int currentBest;
	unsigned long long permTimePQ=1;
	for(int j=0; j<pqSize; j++)
	{
		tmp.lowerBound=computeLowerBoundPQ(SDM, weightMat, baseNum, data, divideNum);
		tmp.nthPerm=permTimePQ;
		pq.push(tmp);
		permTimePQ+=subDist;	
		data=nthFactorial(factorialRecord, baseNum, permTimePQ);
	}
	iota(data.begin(), data.end(), 0);
	while(!pq.empty())
	{
		data=nthFactorial(factorialRecord, baseNum, pq.top().nthPerm);
		currentBest=pq.top().lowerBound;
		if(currentBest<=upperBound)
			findOPTTSP(factorialRecord, SDM, weightMat, baseNum, upperBound, OPTTSPPath, data, subDist, divideNum);
		pq.pop();
	}
	return OPTTSPPath;
}

void findOPTTSP(const vector<unsigned long long> factorialRecord, const vector<vector<int> >& SDM, const vector<vector<int> > weightMat, int baseNum, int& upperBound, vector<int>& OPTTSPPath, vector<int>& data, unsigned long long permNum, int divideNum)
{
	
	unsigned long long permTime=0;
	int i;
	int lowerBound=0;
	unsigned long long pruneNum=0; // the number of possible paths that can be pruned after dealing with one certain path
	int circleLength=0;
	int backPath;
	for(i=0; i<divideNum; i++)
		circleLength+=weightMat[data[i]][data[i+1]];
	for(i=divideNum; i<baseNum-1; i++)
	{
		lowerBound=computeLowerBound(SDM, weightMat, baseNum, data, i, circleLength);
		backPath=circleLength+weightMat[data[i+1]][data[baseNum-1]]+weightMat[data[baseNum-1]][0];
		if(backPath>=upperBound || lowerBound>=upperBound)
			break;
	}
	if(i==baseNum-1)
	{
		circleLength+=weightMat[data[0]][data[baseNum-1]];
		if(circleLength<upperBound)
		{
			upperBound=circleLength;
			OPTTSPPath=data;
		}
	}
	else if(i<baseNum-3)
	{
		pruneNum=factorialRecord[baseNum-i-2]-1;
	}
	permTime++;
	unsigned long long boundNum=factorialRecord[2];
	while(permTime<permNum)
	{
		if(pruneNum>boundNum)
		{
			permTime+=pruneNum;
			nextCheckpoint(data, i+1);
			pruneNum=0;
		}
		else if(pruneNum>0)
		{
			permTime+=pruneNum;
			for(unsigned int j=0; j<pruneNum; j++)
				next_permutation(data.begin()+divideNum, data.end());
			pruneNum=0;
		}
		if(next_permutation(data.begin()+divideNum, data.end()))
		{
			circleLength=0;
			for(i=0; i<divideNum; i++)
				circleLength+=weightMat[data[i]][data[i+1]];
			for(i=divideNum; i<baseNum-1; i++)
			{
				lowerBound=computeLowerBound(SDM, weightMat, baseNum, data, i, circleLength);
				backPath=circleLength+weightMat[data[i+1]][data[baseNum-1]]+weightMat[data[baseNum-1]][0];
				if(backPath>=upperBound || lowerBound>=upperBound)
					break;
			}
			if(i==baseNum-1)
			{
				circleLength+=weightMat[data[0]][data[baseNum-1]];
				if(circleLength<upperBound)
				{
					upperBound=circleLength;
					OPTTSPPath=data;
				}
			}
			else if(i<baseNum-3)
			{
				pruneNum=factorialRecord[baseNum-i-2]-1;
			}
			permTime++;
		}
	}
}

unsigned long long factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

void printOPTTSP(int upperBound, const vector<int>& OPTTSPPath)
{
	cout<<upperBound<<endl;
	cout<<OPTTSPPath[0];
	for(unsigned int i=1; i<OPTTSPPath.size(); i++)
	{
		cout<<" "<<OPTTSPPath[i];
	}
}
