#include<iostream>
#include<iomanip>
#include<map>
#include<vector>
#include<stdlib.h>
#include <time.h>
using namespace std;

#define INF 10000000

/****************function**************/
void printCheckerboard();
void Strategy(int);
void rivalStrategy();
string judgeEnd();
void judge_printResult();

typedef struct checkSpace checkSpace;
typedef struct position position;
int MaxValue(string**,string,int,int,int,checkSpace,int);
int MinValue(string**,string,int,int,int,checkSpace,int);
int Utility(string**,checkSpace,int);
vector<struct position> getAvailableMoves(string**,checkSpace);
string** getNextState(string**, position, string);
checkSpace updateSearch(position,checkSpace);

/****************gobal variable**************/
string **checkerboard = new string*[19];
map<string,int> alphabetChangeN;	// alphabet is changed to number
string alphabet[19]={"A","B","C","D","E","F","G","H"
						,"I","J","K","L","M","N","O","P"
						,"Q","R","S"};
string me="B",rival="W";
bool END = false; 

				
struct position
{
	int i,j;
	
	position(){};
	position(int x,int y)
		:i(x),j(y)
	{}
	
};

struct checkSpace
{
	int xLow, xHeight;
	int yLow, yHeight;
	checkSpace(){}
	checkSpace(int i, int j, int k, int r)
		:yLow(i),yHeight(j),xLow(k),xHeight(r)
	{} 
};
checkSpace searchArea;

//record the used chess
vector<position> ourTeam;
vector<position> RivalTeam;

int main()
{
	int i, j;
	bool FIRST_STEP = true;
	srand(time(NULL));
	/****************judge:Black_White**************/
	cout<<"AI方為(B/W) : ";
	cin>>me;
	
	me = toupper(me[0]);
	if(me == "W")
		rival = "B";
	else if(me == "B")
		rival = "W";
	else
	{
		me = "W";
		rival = "B";
	}
		
	/****************init**************/
	for(int i = 0;i < 19;i++)
	{
		checkerboard[i] = new string[19];
	}
	
	for(i = 0 ; i < 19 ; i++)
	{	
		alphabetChangeN[alphabet[i]]=i;
		
		for(j = 0 ; j < 19 ; j++)
		{
			checkerboard[i][j] = ".";	
		}	
	}	

/****************playing chess**************/	
	cout << "AI方: " << me << " , 我方: " << rival << endl; 
	for(;!END;)
	{	
		if(FIRST_STEP)  //黑方先下一顆 
		{
			if(me == "B") 
			{
				searchArea = checkSpace(8,8,8,8);
				Strategy(2);
				searchArea = updateSearch(ourTeam.front(), searchArea);
			}
			else
			{
				rivalStrategy();
				searchArea = checkSpace(RivalTeam.front().i,RivalTeam.front().i,RivalTeam.front().j,RivalTeam.front().j);
				searchArea = updateSearch(RivalTeam.front(), searchArea);	
			}
			
			FIRST_STEP =false;	
		}
		else
		{
			//雙方輪流下兩顆棋 
			if(me == "B") 
			{
				/* first */
				rivalStrategy();
				searchArea = updateSearch(RivalTeam.back(), searchArea);		
				//printCheckerboard();	
				judge_printResult();
				if(END) return 0;
				/* second */
				rivalStrategy();	
				searchArea = updateSearch(RivalTeam.back(), searchArea);	
				printCheckerboard();	
				judge_printResult();
				if(END) return 0;
				/*** take turns ***/
				
				/* first */
				Strategy(1);
				searchArea = updateSearch(ourTeam.back(), searchArea);
				//printCheckerboard();
				judge_printResult();
				if(END) return 0;
				/* second */
				Strategy(2);
				searchArea = updateSearch(ourTeam.back(), searchArea);
			}
			else
			{
				/* first */
				Strategy(1);
				searchArea = updateSearch(ourTeam.back(), searchArea);
				//printCheckerboard();
				judge_printResult();
				if(END) return 0;
				/* second */
				Strategy(2);
				searchArea = updateSearch(ourTeam.back(), searchArea);
				printCheckerboard();
				judge_printResult();
				if(END) return 0;
				/*** take turns ***/
				/* first */
				rivalStrategy();	
				searchArea = updateSearch(RivalTeam.back(), searchArea);	
				//printCheckerboard();	
				judge_printResult();
				if(END) return 0;
				/* second */
				rivalStrategy();	
				searchArea = updateSearch(RivalTeam.back(), searchArea);
			}
			//cout<<searchArea.xLow+1<<":"<<searchArea.xHeight+1<<"/"<<alphabet[searchArea.yLow]<<":"<<alphabet[searchArea.yHeight]<<endl;
		}
		
		printCheckerboard();	
		judge_printResult();
		if(END) return 0;
	}
	
	
	return 0;
}

/****************Our Strategy: 1 step**************/
void Strategy(int actLeft)
{
	int searchDepth = 0;
	int alpha = -1 * INF;
	int beta = INF;
	checkSpace LocalArea;
	
	vector<position> moves;
	moves = getAvailableMoves(checkerboard, searchArea);
	int iUtility = -1 * INF;
	vector<position> myAction;
	position takeAction;
	
	for(vector<position>::iterator iterM = moves.begin(); iterM != moves.end(); ++iterM)
	{
		LocalArea = updateSearch(*iterM, searchArea);
		int max = MaxValue(getNextState(checkerboard, *iterM, me), me, alpha, beta, searchDepth, LocalArea, actLeft);
		if(max > iUtility)
		{
			iUtility = max;
			myAction.clear();
			myAction.push_back(*iterM);
		}
		else if(max == iUtility)
		{
			myAction.push_back(*iterM);
		}
	}
	int num = myAction.size();
	takeAction = myAction[(rand() % num)];
	checkerboard[takeAction.i][takeAction.j] = me;
	ourTeam.push_back(position(takeAction.i,takeAction.j));
	cout<<"AI方: ( "<<alphabet[takeAction.i]<<" , "<<takeAction.j+1<<" )"<<"\n";
	//cout<<alphabet[searchArea.xLow]<<"-"<<alphabet[searchArea.xHeight]<<"/"<<searchArea.yLow+1<<"-"<<searchArea.yHeight+1<<endl;
}

/****************rival Strategy: 1 step**************/
void rivalStrategy()
{
	int x,y;
	string x_s;
	
	cout<<"我方x (A-S) : ";
	cin>>x_s;
	x_s = toupper(x_s[0]);
	
	if( alphabetChangeN.find(x_s) == alphabetChangeN.end())
	{
		cout<<"請輸入正確範圍 A-S : \n";
		return rivalStrategy();
	}
	
	cout<<"我方y (1-19) : ";
	cin>>y;
	
	x = alphabetChangeN[x_s];	// the alphabet is directly changed to number,not need to sub 1 

	if(	x < 0 || x >18  || y < 0 || y >19 )
	{
		cout<<"請輸入正確位置:\n ";
		return rivalStrategy();
	}
	if(checkerboard[x][y-1] != ".")
	{
		cout<<"位置已被佔據，請重新輸入: \n";
		return rivalStrategy();
	}
	checkerboard[x][y-1] = rival;
	RivalTeam.push_back(position(x,y-1));
	
	return;	
}

/****************MiniMax**************/
int MaxValue(string **state, string player, int alpha, int beta, int depth, checkSpace area, int actLeft)
{
	if(judgeEnd() !="continue" || depth == 0)
		return Utility(state,area,actLeft);
	
	vector<position> Actions;
	Actions = getAvailableMoves(state,area);
	
	checkSpace nextArea;
	int v = -1 * INF;
	for(vector<position>::iterator iter = Actions.begin(); iter != Actions.end(); ++iter)
	{
		nextArea = updateSearch(*iter, area);
		int min = MinValue(getNextState(state, *iter, player), player, alpha, beta, (depth-1), nextArea, actLeft);
		if(v < min)
			v = min;
		if(v >= beta)
			return v;
		if(v > alpha)
			alpha = v;
	}
	return v;
}

int MinValue(string **state, string player, int alpha, int beta, int depth, checkSpace area, int actLeft)
{
	if(judgeEnd() !="continue" || depth == 0)
		return Utility(state,area,actLeft);
	
	vector<position> Actions;
	Actions = getAvailableMoves(state, area);
	
	checkSpace nextArea;
	int v = INF;
	for(vector<position>::iterator iter = Actions.begin(); iter != Actions.end(); ++iter)
	{
		nextArea = updateSearch(*iter, area);
		int max = MaxValue(getNextState(state, *iter, player), player, alpha, beta, (depth-1), nextArea, actLeft);
		if(v > max)
			v = max;
		if(v <= alpha)
			return v;
		if(v < beta)
			beta = v;
	}
	return v;
}

int Utility(string **state, checkSpace area,int actLeft)
{
	int Xstart = area.xLow;
	int Xend = area.xHeight;
	int Ystart = area.yLow;
	int Yend = area.yHeight;
	
	int value = 0;
	//Row
	for(int i = Ystart;i <= Yend;i++)
	{
		for(int j = Xstart;j <= Xend;j++)
		{
			if(state[i][j] != ".")
			{
				//連六 
				if(j < 14)
				{
					if(state[i][j] == state[i][j+1] && state[i][j+1] == state[i][j+2] && state[i][j+2] == state[i][j+3] && state[i][j+3] == state[i][j+4] && state[i][j+4] == state[i][j+5])
					{
						if(state[i][j] == me)
							value += 1000000;
						else
							value -= 1200000;
						j += 5;
						continue;
					}
				}
				//雙/單迫著 
				if(j < 14 && j >= 1) 
				{
					//連五子
					if(state[i][j] == state[i][j+1] && state[i][j+1] == state[i][j+2] && state[i][j+2] == state[i][j+3] && state[i][j+3] == state[i][j+4])
					{
						if(state[i][j-1] == "." && (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
						{
							if(state[i][j] == me)
							{
								if(actLeft == 1)
									value += 900000;
								else
									value += 6000;
							}
							else
								value -= 30000;
							j += 4;
						}
						else if(state[i][j-1] == "." || (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
						{
							if(state[i][j] == me)
							{
								if(actLeft == 1)
									value += 800000;
								else
									value += 2500;
							}
							else
								value -= 20000;
							j += 4;
						}
						continue;
					}
				}
				if(j < 14 && j >= 2) 
				{
					//連四子
					if(state[i][j] == state[i][j+1] && state[i][j+1] == state[i][j+2] && state[i][j+2] == state[i][j+3])
					{
						if(state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j+4] == "." || state[i][j+4] == state[i][j]) &&  (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
						{
							if(state[i][j] == me)
								value += 5000;
							else
								value -= 18000;
							j += 3;
						}
						else if((state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j])) || ((state[i][j+4] == "." || state[i][j+4] == state[i][j]) &&  (state[i][j+5] == "." || state[i][j+5] == state[i][j])) || (state[i][j-1] == "." &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j])))
						{
							if(state[i][j] == me)
								value += 2000;
							else
								value -= 11000;
							j += 3;
						}
						continue;
					}
				}
				//活/死三 
				if(j < 14 && j >= 3)
				{
					if(state[i][j] == state[i][j+1] && state[i][j+1] == state[i][j+2])
					{
						if(state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j-3] == "." || state[i][j-3] == state[i][j]) && state[i][j+3] == "." &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j]) && (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
						{
							if(state[i][j] == me)
								value += 1800;
							else
								value -= 3000;
							j += 2;
						}
						else if((state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j-3] == "." || state[i][j-3] == state[i][j])) || (state[i][j+3] == "." &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j]) && (state[i][j+5] == "." || state[i][j+5] == state[i][j])) || (state[i][j-1] == "." && state[i][j+3] == "." && ((state[i][j-2] == "." || state[i][j-2] == state[i][j]) || (state[i][j+4] == "." || state[i][j+4] == state[i][j]))))
						{
							if(state[i][j] == me)
								value += 1000;
							else
								value -= 1300;
							j += 2;
						}
						continue;
					}
				}
				//活/死二
				if(j < 14 && j >= 4)
				{
					if(state[i][j] == state[i][j+1])
					{
						if(state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j-3] == "." || state[i][j-3] == state[i][j]) && (state[i][j-4] == "." || state[i][j-4] == state[i][j]) && state[i][j+2] == "." && (state[i][j+3] == "." || state[i][j+3] == state[i][j]) &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j]) && (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
						{
							if(state[i][j] == me)
								value += 600;
							else
								value -= 1000;
							j += 1;
						}
						else if((state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j-3] == "." || state[i][j-3] == state[i][j]) && (state[i][j-4] == "." || state[i][j-4] == state[i][j])) || (state[i][j+2] == "." && (state[i][j+3] == "." || state[i][j+3] == state[i][j]) &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j]) && (state[i][j+5] == "." || state[i][j+5] == state[i][j])))
						{
							if(state[i][j] == me)
								value += 200;
							else
								value -= 250;
							j += 1;
						}
						continue;
					}
				}
				//活/死一 
				if(j < 14 && j >= 5)
				{
					if(state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j-3] == "." || state[i][j-3] == state[i][j]) && (state[i][j-4] == "." || state[i][j-4] == state[i][j]) && (state[i][j-5] == "." || state[i][j-5] == state[i][j]) && state[i][j+1] == "." && (state[i][j+2] == "." || state[i][j+2] == state[i][j]) && (state[i][j+3] == "." || state[i][j+3] == state[i][j]) &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j]) && (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
					{
						if(state[i][j] == me)
							value += 30;
						else
							value -= 35;
					}
				}
				else
				{
					if(j >= 14)
					{
						if((state[i][j-1] == "." && (state[i][j-2] == "." || state[i][j-2] == state[i][j]) && (state[i][j-3] == "." || state[i][j-3] == state[i][j]) && (state[i][j-4] == "." || state[i][j-4] == state[i][j]) && (state[i][j-5] == "." || state[i][j-5] == state[i][j])))
						{
							if(state[i][j] == me)
								value += 10;
							else
								value -= 15;
						}
					}
					if(j < 5)
					{
						if(state[i][j+1] == "." && (state[i][j+2] == "." || state[i][j+2] == state[i][j]) && (state[i][j+3] == "." || state[i][j+3] == state[i][j]) &&  (state[i][j+4] == "." || state[i][j+4] == state[i][j]) && (state[i][j+5] == "." || state[i][j+5] == state[i][j]))
						{
							if(state[i][j] == me)
								value += 10;
							else
								value -= 15;
						}
					}
				}
			}
		}
	}
	
	//Col
	for(int i = Xstart;i <= Xend;i++)
	{
		for(int j = Ystart;j <= Yend;j++)
		{
			if(state[j][i] != ".")
			{
				//連六 
				if(j < 14)
				{
					if(state[j][i] == state[j+1][i] && state[j+1][i] == state[j+2][i] && state[j+2][i] == state[j+3][i] && state[j+3][i] == state[j+4][i] && state[j+4][i] == state[j+5][i])
					{
						if(state[j][i] == me)
							value += 1000000;
						else
							value -= 1200000;
						j += 5;
						continue;
					}
				}
				//雙/單迫著 
				if(j < 14 && j >= 1)
				{
					//連五子 
					if(state[j][i] == state[j+1][i] && state[j+1][i] == state[j+2][i] && state[j+2][i] == state[j+3][i] && state[j+3][i] == state[j+4][i])
					{
						if(state[j-1][i] == "." && (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
						{
							if(state[j][i] == me)
							{
								if(actLeft == 1)
									value += 900000;
								else
									value += 6000;
							}
							else
								value -= 30000;
							j += 4;
						}
						else if(state[j-1][i] == "." || (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
						{
							if(state[j][i] == me)
							{
								if(actLeft == 1)
									value += 800000;
								else
									value += 2500;
							}
							else
								value -= 20000;
							j += 4;
						}
						continue;
					}
				}
				if(j < 14 && j >= 2)
				{
					//連四子 
					if(state[j][i] == state[j+1][i] && state[j+1][i] == state[j+2][i] && state[j+2][i] == state[j+3][i])
					{
						if(state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j+4][i] == "." || state[j+4][i] == state[j][i]) &&  (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
						{
							if(state[j][i] == me)
								value += 5000;
							else
								value -= 18000;
							j += 3;
						}
						else if((state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i])) || ((state[j+4][i] == "." || state[j+4][i] == state[j][i]) &&  (state[j+5][i] == "." || state[j+5][i] == state[j][i])) || (state[j-1][i] == "." && (state[j+4][i] == "." || state[j+4][i] == state[j][i])))
						{
							if(state[j][i] == me)
								value += 2000;
							else
								value -= 11000;
							j += 3;
						}
						continue;
					}
				}
				//活/死三 
				if(j < 14 && j >= 3)
				{
					if(state[j][i] == state[j+1][i] && state[j+1][i] == state[j+2][i])
					{
						if(state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j-3][i] == "." || state[j-3][i] == state[j][i]) && state[j+3][i] == "." &&  (state[j+4][i] == "." || state[j+4][i] == state[j][i]) && (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
						{
							if(state[j][i] == me)
								value += 1800;
							else
								value -= 3000;
							j += 2;
						}
						else if((state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j-3][i] == "." || state[j-3][i] == state[j][i])) || (state[j+3][i] == "." &&  (state[j+4][i] == "." || state[j+4][i] == state[j][i]) && (state[j+5][i] == "." || state[j+5][i] == state[j][i])) || (state[j-1][i] == "." && state[j+3][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i] || state[j+4][i] == "." || state[j+4][i] == state[j][i])))
						{
							if(state[j][i] == me)
								value += 1000;
							else
								value -= 1300;
							j += 2;
						}
						continue;
					}
				}
				//活/死二
				if(j < 14 && j >= 4)
				{
					if(state[j][i] == state[j+1][i])
					{
						if(state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j-3][i] == "." || state[j-3][i] == state[j][i]) && (state[j-4][i] == "." || state[j-4][i] == state[j][i]) && state[j+2][i] == "." && (state[j+3][i] == "." || state[j+3][i] == state[j][i]) &&  (state[j+4][i] == "." || state[j+4][i] == state[j][i]) && (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
						{
							if(state[j][i] == me)
								value += 600;
							else
								value -= 1000;
							j += 1;
						}
						else if((state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j-3][i] == "." || state[j-3][i] == state[j][i]) && (state[j-4][i] == "." || state[j-4][i] == state[j][i])) || (state[j+2][i] == "." && (state[j+3][i] == "." || state[j+3][i] == state[j][i]) &&  (state[j+4][i] == "." || state[j+4][i] == state[j][i]) && (state[j+5][i] == "." || state[j+5][i] == state[j][i])))
						{
							if(state[j][i] == me)
								value += 200;
							else
								value -= 250;
							j += 1;
						}
					}
				}
				//活/死一 
				if(j < 14 && j >= 5)
				{
					if(state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j-3][i] == "." || state[j-3][i] == state[j][i]) && (state[j-4][i] == "." || state[j-4][i] == state[j][i]) && (state[j-5][i] == "." || state[j-5][i] == state[j][i]) && state[j+1][i] == "." && (state[j+2][i] == "." || state[j+2][i] == state[j][i]) && (state[j+3][i] == "." || state[j+3][i] == state[j][i]) &&  (state[j+4][i] == "." || state[j+4][i] == state[j][i]) && (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
					{
						if(state[j][i] == me)
							value += 30;
						else
							value -= 35;
					}
				}
				else
				{
					if(j >= 5)
					{
						if((state[j-1][i] == "." && (state[j-2][i] == "." || state[j-2][i] == state[j][i]) && (state[j-3][i] == "." || state[j-3][i] == state[j][i]) && (state[j-4][i] == "." || state[j-4][i] == state[j][i]) && (state[j-5][i] == "." || state[j-5][i] == state[j][i])))
						{
							if(state[i][j] == me)
								value += 10;
							else
								value -= 15;
						}
					}
					if(j < 14)
					{
						if(state[j+1][i] == "." && (state[j+2][i] == "." || state[j+2][i] == state[j][i]) && (state[j+3][i] == "." || state[j+3][i] == state[j][i]) &&  (state[j+4][i] == "." || state[j+4][i] == state[j][i]) && (state[j+5][i] == "." || state[j+5][i] == state[j][i]))
						{
							if(state[j][i] == me)
								value += 10;
							else
								value -= 15;
						}
					}
				}
			}
		}
	}
	
	//oblique
	int i = Yend;
	int j = Xstart;
	while(j <= Xend)
	{
		if(i > 13 && (i-j) > 13)
		{
			i = 13 + j;
		}
		if(j > 13 && (j-i) > 13)
			break;
		for(int k = 0;((i+k) <= Yend && (j+k) <= Xend);k++)
		{
			if(state[i+k][j+k] != ".")
			{
				//連六 
				if((i+k) < 14 && (j+k) < 14)
				{
					if(state[i+k][j+k] == state[i+1+k][j+1+k] && state[i+1+k][j+1+k] == state[i+2+k][j+2+k] && state[i+2+k][j+2+k] == state[i+3+k][j+3+k] && state[i+3+k][j+3+k] == state[i+4+k][j+4+k] && state[i+4+k][j+4+k] == state[i+5+k][j+5+k])
					{
						if(state[i+k][j+k] == me)
							value += 1000000;
						else
							value -= 1200000;
						k += 5;
						continue;
					}
				}
				//雙/單迫著 
				if((i+k) < 14 && (j+k) < 14 && (i+k) >= 1 && (j+k) >= 1) 
				{
					//連五子
					if(state[i+k][j+k] == state[i+1+k][j+1+k] && state[i+1+k][j+1+k] == state[i+2+k][j+2+k] && state[i+2+k][j+2+k] == state[i+3+k][j+3+k] && state[i+3+k][j+3+k] == state[i+4+k][j+4+k])
					{
						if(state[i-1+k][j-1+k] == "." && (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
							{
								if(actLeft == 1)
									value += 900000;
								else
									value += 6000;
							}
							else
								value -= 30000;
							k += 4;
						}
						else if(state[i-1+k][j-1+k] == "." || (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
							{
								if(actLeft == 1)
									value += 800000;
								else
									value += 2500;
							}
							else
								value -= 20000;
							k += 4;
						}
						continue;
					}
				}
				if((i+k) < 14 && (j+k) < 14 && (i+k) >= 2 && (j+k) >= 2) 
				{
					//連四子
					if(state[i+k][j+k] == state[i+1+k][j+1+k] && state[i+1+k][j+1+k] == state[i+2+k][j+2+k] && state[i+2+k][j+2+k] == state[i+3+k][j+3+k])
					{
						if(state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
								value += 5000;
							else
								value -= 18000;
							k += 3;
						}
						else if((state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k])) || ((state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k])) || (state[i-1+k][j-1+k] == "." &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k])))
						{
							if(state[i+k][j+k] == me)
								value += 2000;
							else
								value -= 11000;
							k += 3;
						}
						continue;
					}
				}
				//活/死三 
				if((i+k) < 14 && (j+k) < 14 && (i+k) >= 3 && (j+k) >= 3)
				{
					if(state[i+k][j+k] == state[i+1+k][j+1+k] && state[i+1+k][j+1+k] == state[i+2+k][j+2+k])
					{
						if(state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i-3+k][j-3+k] == "." || state[i-3+k][j-3+k] == state[i+k][j+k]) && state[i+3+k][j+3+k] == "." &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
								value += 1800;
							else
								value -= 3000;
							k += 2;
						}
						else if((state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i-3+k][j-3+k] == "." || state[i-3+k][j-3+k] == state[i+k][j+k])) || (state[i+3+k][j+3+k] == "." &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k])) || (state[i-1+k][j-1+k] == "." && state[i+3+k][j+3+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k] || state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k])))
						{
							if(state[i+k][j+k] == me)
								value += 1000;
							else
								value -= 1300;
							k += 2;
						}
						continue;
					}
				}
				//活/死二
				if((i+k) < 14 && (j+k) < 14 && (i+k) >= 4 && (j+k) >= 4)
				{
					if(state[i+k][j+k] == state[i+1+k][j+1+k])
					{
						if(state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i-3+k][j-3+k] == "." || state[i-3+k][j-3+k] == state[i+k][j+k]) && (state[i-4+k][j-4+k] == "." || state[i-4+k][j-4+k] == state[i+k][j+k]) && state[i+2+k][j+2+k] == "." && (state[i+3+k][j+3+k] == "." || state[i+3+k][j+3+k] == state[i+k][j+k]) &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
								value += 600;
							else
								value -= 1000;
							k += 1;
						}
						else if((state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i-3+k][j-3+k] == "." || state[i-3+k][j-3+k] == state[i+k][j+k]) && (state[i-4+k][j-4+k] == "." || state[i-4+k][j-4+k] == state[i+k][j+k])) || (state[i+2+k][j+2+k] == "." && (state[i+3+k][j+3+k] == "." || state[i+3+k][j+3+k] == state[i+k][j+k]) &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k])))
						{
							if(state[i+k][j+k] == me)
								value += 200;
							else
								value -= 250;
							k += 1;
						}
						continue;
					}
				}
				//活/死一 
				if((i+k) < 14 && (j+k) < 14 && (i+k) >= 5 && (j+k) >= 5)
				{
					if(state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i-3+k][j-3+k] == "." || state[i-3+k][j-3+k] == state[i+k][j+k]) && (state[i-4+k][j-4+k] == "." || state[i-4+k][j-4+k] == state[i+k][j+k]) && (state[i-5+k][j-5+k] == "." || state[i-5+k][j-5+k] == state[i+k][j+k]) && state[i+1+k][j+1+k] == "." && (state[i+2+k][j+2+k] == "." || state[i+2+k][j+2+k] == state[i+k][j+k]) && (state[i+3+k][j+3+k] == "." || state[i+3+k][j+3+k] == state[i+k][j+k]) &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
					{
						if(state[i+k][j+k] == me)
							value += 30;
						else
							value -= 35;
					}
				}
				else
				{
					if((i+k) >= 5 && (j+k) >= 5)
					{
						if(state[i-1+k][j-1+k] == "." && (state[i-2+k][j-2+k] == "." || state[i-2+k][j-2+k] == state[i+k][j+k]) && (state[i-3+k][j-3+k] == "." || state[i-3+k][j-3+k] == state[i+k][j+k]) && (state[i-4+k][j-4+k] == "." || state[i-4+k][j-4+k] == state[i+k][j+k]) && (state[i-5+k][j-5+k] == "." || state[i-5+k][j-5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
								value += 10;
							else
								value -= 15;
						}
					}
					if((i+k) < 14 && (j+k) < 14)
					{
						if(state[i+1+k][j+1+k] == "." && (state[i+2+k][j+2+k] == "." || state[i+2+k][j+2+k] == state[i+k][j+k]) && (state[i+3+k][j+3+k] == "." || state[i+3+k][j+3+k] == state[i+k][j+k]) &&  (state[i+4+k][j+4+k] == "." || state[i+4+k][j+4+k] == state[i+k][j+k]) &&  (state[i+5+k][j+5+k] == "." || state[i+5+k][j+5+k] == state[i+k][j+k]))
						{
							if(state[i+k][j+k] == me)
								value += 10;
							else
								value -= 15;
						}
					}
				}
			}
		}
		if(i > Ystart)
			i--;
		else
			j++;
	}
	
	//inv oblique
	i = Ystart;
	j = Xstart;
	if((i+j) < 5)
		i = 5 - j;
	while(j <= Xend)
	{
		if((i+j) > 31)
			break;
		for(int k = 0;((i-k) >= Ystart && (j+k) <= Xend);k++)
		{
			if(state[i-k][j+k] != ".")
			{
				//連六 
				if((i-k) > 4 && (j+k) < 14)
				{
					if(state[i-k][j+k] == state[i-1-k][j+1+k] && state[i-1-k][j+1+k] == state[i-2-k][j+2+k] && state[i-2-k][j+2+k] == state[i-3-k][j+3+k] && state[i-3-k][j+3+k] == state[i-4-k][j+4+k] && state[i-4-k][j+4+k] == state[i-5-k][j+5+k])
					{
						if(state[i-k][j+k] == me)
							value += 1000000;
						else
							value -= 1200000;
						k += 5;
						continue;
					}
				}
				//雙/單迫著 
				if((i-k) > 4 && (j+k) < 14 && (i-k) < 18 && (j+k) >= 1) 
				{
					//連五子
					if(state[i-k][j+k] == state[i-1-k][j+1+k] && state[i-1-k][j+1+k] == state[i-2-k][j+2+k] && state[i-2-k][j+2+k] == state[i-3-k][j+3+k] && state[i-3-k][j+3+k] == state[i-4-k][j+4+k])
					{
						if(state[i+1-k][j-1+k] == "." && (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
							{
								if(actLeft == 1)
									value += 900000;
								else
									value += 6000;
							}
							else
								value -= 30000;
							k += 4;
						}
						else if(state[i+1-k][j-1+k] == "." || (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
							{
								if(actLeft == 1)
									value += 800000;
								else
									value += 2500;
							}
							else
								value -= 20000;
							k += 4;
						}
						continue;
					}
				}
				if((i-k) > 4 && (j+k) < 14 && (i-k) < 17 && (j+k) >= 2) 
				{
					//連四子
					if(state[i-k][j+k] == state[i-1-k][j+1+k] && state[i-1-k][j+1+k] == state[i-2-k][j+2+k] && state[i-2-k][j+2+k] == state[i-3-k][j+3+k])
					{
						if(state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
								value += 5000;
							else
								value -= 18000;
							k += 3;
						}
						else if((state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k])) || ((state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k])) || (state[i+1-k][j-1+k] == "." &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k])))
						{
							if(state[i-k][j+k] == me)
								value += 2000;
							else
								value -= 11000;
							k += 3;
						}
						continue;
					}
				}
				//活/死三 
				if((i-k) > 4 && (j+k) < 14 && (i-k) < 16 && (j+k) >= 3)
				{
					if(state[i-k][j+k] == state[i-1-k][j+1+k] && state[i-1-k][j+1+k] == state[i-2-k][j+2+k])
					{
						if(state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i+3-k][j-3+k] == "." || state[i+3-k][j-3+k] == state[i-k][j+k]) && state[i-3-k][j+3+k] == "." &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
								value += 1700;
							else
								value -= 3000;
							k += 2;
						}
						else if((state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i+3-k][j-3+k] == "." || state[i+3-k][j-3+k] == state[i-k][j+k])) || (state[i-3-k][j+3+k] == "." &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k])) || (state[i+1-k][j-1+k] == "." && state[i-3-k][j+3+k] == "." &&  (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k] || state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k])))
						{
							if(state[i-k][j+k] == me)
								value += 1000;
							else
								value -= 1300;
							k += 2;
						}
						continue;
					}
				}
				//活/死二
				if((i-k) > 4 && (j+k) < 14 && (i-k) < 15 && (j+k) >= 4)
				{
					if(state[i-k][j+k] == state[i-1-k][j+1+k])
					{
						if(state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i+3-k][j-3+k] == "." || state[i+3-k][j-3+k] == state[i-k][j+k]) && (state[i+4-k][j-4+k] == "." || state[i+4-k][j-4+k] == state[i-k][j+k]) && state[i-2-k][j+2+k] == "." && (state[i-3-k][j+3+k] == "." || state[i-3-k][j+3+k] == state[i-k][j+k]) &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
								value += 800;
							else
								value -= 1000;
							k += 1;
						}
						else if((state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i+3-k][j-3+k] == "." || state[i+3-k][j-3+k] == state[i-k][j+k]) && (state[i+4-k][j-4+k] == "." || state[i+4-k][j-4+k] == state[i-k][j+k])) || (state[i-2-k][j+2+k] == "." && (state[i-3-k][j+3+k] == "." || state[i-3-k][j+3+k] == state[i-k][j+k]) &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k])))
						{
							if(state[i-k][j+k] == me)
								value += 200;
							else
								value -= 250;
							k += 1;
						}
						continue;
					}
				}
				//活/死一 
				if((i-k) > 4 && (j+k) < 14 && (i-k) < 14 && (j+k) >= 5)
				{
					if(state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i+3-k][j-3+k] == "." || state[i+3-k][j-3+k] == state[i-k][j+k]) && (state[i+4-k][j-4+k] == "." || state[i+4-k][j-4+k] == state[i-k][j+k]) && (state[i+5-k][j-5+k] == "." || state[i+5-k][j-5+k] == state[i-k][j+k]) && state[i-1-k][j+1+k] == "." && (state[i-2-k][j+2+k] == "." || state[i-2-k][j+2+k] == state[i-k][j+k]) && (state[i-3-k][j+3+k] == "." || state[i-3-k][j+3+k] == state[i-k][j+k]) &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
					{
						if(state[i-k][j+k] == me)
							value += 30;
						else
							value -= 35;
					}
				}
				else
				{
					if((i-k) < 14 && (j+k) >= 5)
					{
						if(state[i+1-k][j-1+k] == "." && (state[i+2-k][j-2+k] == "." || state[i+2-k][j-2+k] == state[i-k][j+k]) && (state[i+3-k][j-3+k] == "." || state[i+3-k][j-3+k] == state[i-k][j+k]) && (state[i+4-k][j-4+k] == "." || state[i+4-k][j-4+k] == state[i-k][j+k]) && (state[i+5-k][j-5+k] == "." || state[i+5-k][j-5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
								value += 10;
							else
								value -= 15;
						}
					}
					if((i-k) > 4 && (j+k) < 14)
					{
						if(state[i-1-k][j+1+k] == "." && (state[i-2-k][j+2+k] == "." || state[i-2-k][j+2+k] == state[i-k][j+k]) && (state[i-3-k][j+3+k] == "." || state[i-3-k][j+3+k] == state[i-k][j+k]) &&  (state[i-4-k][j+4+k] == "." || state[i-4-k][j+4+k] == state[i-k][j+k]) &&  (state[i-5-k][j+5+k] == "." || state[i-5-k][j+5+k] == state[i-k][j+k]))
						{
							if(state[i-k][j+k] == me)
								value += 10;
							else
								value -= 15;
						}
					}
				}
			}
		}
		if(i < Yend)
			i++;
		else
			j++;
	}
	return value;
	
}

vector<position> getAvailableMoves(string **state, checkSpace area)
{
	int Xstart = area.xLow;
	int Xend = area.xHeight;
	int Ystart = area.yLow;
	int Yend = area.yHeight;
	
	vector<position> AvailableActions;
	for(int i = Ystart;i <= Yend;i++)
	{
		for(int j = Xstart;j <= Xend;j++)
		{
			if(state[i][j] == ".")
				AvailableActions.push_back(position(i,j));
		}
	}
	return AvailableActions;
}

string** getNextState(string **state, position a, string player)
{
	string **reState = new string*[19];
	for(int i = 0;i < 19;i++)
	{
		reState[i] = new string[19];
	}
	for(int i = 0;i < 19;i++)
	{
		for(int j = 0;j < 19;j++)
		{
			reState[i][j] = state[i][j];
		}
	}
	int y = a.i;
	int x = a.j;
	reState[y][x] = player;
	return reState;
}

checkSpace updateSearch(position p, checkSpace space)
{
	int pXLow = p.j - 2;
	int pXHeight = p.j + 2;
	int pYLow = p.i - 2;
	int pYHeight = p.i + 2;
	if(pXLow < space.xLow)
	{
		if(pXLow >= 0)
			space.xLow = pXLow;
		else
			space.xLow = 0;
	}	
	if(pXHeight > space.xHeight)
	{
		if(pXHeight < 19)
			space.xHeight = pXHeight;
		else
			space.xHeight = 18;
	}
	if(pYLow < space.yLow)
	{
		if(pYLow >= 0)
			space.yLow = pYLow;
		else
			space.yLow = 0;
	}	
	if(pYHeight > space.yHeight)
	{
		if(pYHeight < 19)
			space.yHeight = pYHeight;
		else
			space.yHeight = 18;
	}	
	return space;
}

/****************judgeEnd: win / lose / continue**************/
string judgeEnd()
{
	//me
	int r[19][19]={0};   
	int c[19][19]={0};
	int v[19][19]={0};
	int iv[19][19]={0};	
	//rival
	int rr[19][19]={0};   
	int rc[19][19]={0};
	int rv[19][19]={0};
	int riv[19][19]={0};
	
	for(int i=0;i<19;i++)
	{
		for(int j=0;j<19;j++)
		{
			//me
			if(checkerboard[i][j]=="."||checkerboard[i][j]==rival)
			{
			
				r[i][j]=0;	c[i][j]=0;	v[i][j]=0;	iv[i][j]=0;
			}
			else
			{
				if(i>=1) r[i][j]=r[i-1][j]+1;
				else	r[i][j]=1;
				if(j>=1)	c[i][j]=c[i][j-1]+1;
				else	c[i][j]=1;
				if(i>=1||j>=1) v[i][j]=v[i-1][j-1]+1;			
				else v[i][j]=1;
				if(i<18||j<18) iv[i][j]=iv[i-1][j+1]+1;	
				else iv[i][j]=1;	
				
			}
			if(r[i][j]==6||c[i][j]==6||v[i][j]==6||iv[i][j]==6)
				return "win";
			
			//rival
			if(checkerboard[i][j]=="."||checkerboard[i][j]==me)
			{
			
				rr[i][j]=0;
				rc[i][j]=0;
				rv[i][j]=0;
				riv[i][j]=0;
			}
			else
			{
				if(i>=1) rr[i][j]=rr[i-1][j]+1;
				else	rr[i][j]=1;
				if(j>=1)	rc[i][j]=rc[i][j-1]+1;
				else	rc[i][j]=1;
				if(i>=1||j>=1) rv[i][j]=rv[i-1][j-1]+1;			
				else rv[i][j]=1;
				if(i<18||j<18) riv[i][j]=riv[i-1][j+1]+1;	
				else riv[i][j]=1;	
				
			}
			if(rr[i][j]==6||rc[i][j]==6||rv[i][j]==6||riv[i][j]==6)
				return "lose";
		}
	}
	
	return "continue";

}

/****************judge & print result**************/
void judge_printResult()
{
		if(judgeEnd() !="continue")
		{
			if(judgeEnd() =="win")
				cout<<"------AI方勝利!!!------\n";
			else
				cout<<"------我方勝利!!!------\n";
			END = true;
		}
}


/****************print Current Checkerboard**************/
void printCheckerboard()
{
	int i , j ;
	cout<<setw(3)<<" "<<setw(3)<<"1"<<setw(3)<<"2"
	    <<setw(3)<<"3"<<setw(3)<<"4"<<setw(3)<<"5"
	    <<setw(3)<<"6"<<setw(3)<<"7"<<setw(3)<<"8"
	    <<setw(3)<<"9"<<setw(3)<<"10"<<setw(3)<<"11"
	    <<setw(3)<<"12"<<setw(3)<<"13"<<setw(3)<<"14"
	    <<setw(3)<<"15"<<setw(3)<<"16"<<setw(3)<<"17"
	    <<setw(3)<<"18"<<setw(3)<<"19"<<"\n";
	
	for(i = 0 ; i < 19 ; i++)
	{
		cout<<setw(3)<<alphabet[i];
		
		for(j = 0 ; j < 19 ; j++)
		{
			cout<<setw(3)<<checkerboard[i][j];
		}
		cout<<"\n";
	}
}



