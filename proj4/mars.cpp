#include"mode.h"
#include"mapinfor.h"
using namespace std;

int main(int argc, char **argv)
{
	adjMat adjMat1(argc, argv);
	if(adjMat1.mode=="MST")
	{
		vector<int> MSTPath=computeMST(adjMat1);
		printMST(adjMat1, MSTPath);
		cout<<endl;
	}
	else if(adjMat1.mode=="FASTTSP")
	{
		vector<int> MSTPath=computeMST(adjMat1);
		vector<int> pathFASTTSP=pathFromMST(MSTPath);
		//int totalDist=totalDistance(adjMat1, pathFASTTSP);/////////////////////////		
		//int cursorPos=0;///////////////////////////////////////////////////////////
		//optFromMST(adjMat1, pathFASTTSP, cursorPos, totalDist);////////////////////
		//cout<<totalDistance(adjMat1, pathFASTTSP);/////////////////////////////////
		printFASTTSP(adjMat1, pathFASTTSP);
		cout<<endl;
	}
	else if(adjMat1.mode=="OPTTSP")
	{
		//cout<<adjMat1.adjMatrix[adjMat1.baseNum-1][adjMat1.baseNum-1]<<endl;//////////////
		vector<int> MSTPath=computeMST(adjMat1);
		vector<int> pathFASTTSP=pathFromMST(MSTPath);
		int totalDist=totalDistance(adjMat1, pathFASTTSP);		
		int cursorPos=0;
		optFromMST(adjMat1, pathFASTTSP, cursorPos, totalDist);
		int upperBound=totalDistance(adjMat1, pathFASTTSP);
		//cout<<adjMat1.adjMatrix[adjMat1.baseNum-1][adjMat1.baseNum-1]<<endl;//////////////
		//cout<<upperBound<<endl;//////////////////////////////////////////////////////
		//upperBound=94;///////////////////////////////////////////////////////////////
		vector<int> pathOPTTSP=findOPTTSP(adjMat1, upperBound, pathFASTTSP);
		//cout<<adjMat1.adjMatrix[adjMat1.baseNum-1][adjMat1.baseNum-1]<<endl;//////////////
		printOPTTSP(upperBound, pathOPTTSP);
		cout<<endl;
	}
	else
	{
		cerr<<"invalid command line argument"<<endl;
		exit(1);
	}
	return 0;
}
