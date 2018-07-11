
#include "pch.h"
#include "robot.h"

#include <algorithm>
#include <vector>

#include <iostream>

int Robot::maxDepth = 3;

const int xp[] = {0,0,1,-1};
const int yp[] = {1,-1,0,0};

const int Robot::value[11] = {0,10,21,33,46,60,75,81,100,1000,10000};

bool inBoard(int x,int y)
{
	return x>=0 && x<15 && y>=0 && y<15;
}

Node Robot::makeAutoMove(Player* player, int depth = 0)
{   
	int getV = getValue(player);
	if (depth == maxDepth || getV > 7000 || getV < -7000)
	{
		Node t = Node(0, 0, player);
		t.value = -getV;
		return t;
	}
	std::vector<Node> c;

	Board *&b = player->board;

	for (int i = 0; i<15; i++)
		for (int j = 0; j<15; j++)
			if (check(player,i,j))
			{
				Node t(i,j);

				b->board[i][j] = player->token;
				t.value = getValue(player);

				b->board[i][j] = 0;

				c.push_back(t);
			}

	sort(c.begin(),c.end()-1);

	Node ans;
	ans.value = - 0x3f3f3f3f;

	for (int i = 0; i<7 && i<=(c.size()-7); i++)
	{
		//if (depth==3)
		//	std::cout << "> " << depth << "---" << c[i].value << " " << player->token << std::endl;

		b->board[c[i].x][c[i].y] = player->token;
		Node t = makeAutoMove( b->player[ player->token==1?2:1 ], depth+1);

		if (t.value > ans.value)
		{
			ans = c[i];
			ans.value = t.value;
		}
		b->board[c[i].x][c[i].y] = 0;
	}

	if (depth)
		ans.value = -ans.value;

	return ans;
}
  
const int plus[] = {-2,-1,0,1,2};

bool Robot::check(Player* player, int x, int y)
{
	Board *&t = player->board;
	if (t->board[x][y])
		return false;

	bool ret = false;
	for (int i = 0; i<5; i++)
		for (int j = 0; j<5; j++)
			if (inBoard(x+plus[i],y+plus[j]))
				if ( t->board[ x + plus[i] ][ y + plus[j] ] )
					ret = true;

	return ret;
}

int Robot::getValue(Player* player)
{
	Board *&b = player->board;

	int num[3][10];
	int c[16];

	memset(num, 0, sizeof(num));

	for (int i = 0; i<15; i++)
	{
		memset(c, 0, sizeof(c));
		for (int j = 0; j<15; j++)
			c[j] = b->board[i][j];
		c[15] = 3;

		getLineValue(c,num);

		memset(c, 0, sizeof(c));
		for (int j = 0; j<15; j++)
			c[j] = b->board[j][i];
		c[15] = 3;

		getLineValue(c,num);

		memset(c, 0, sizeof(c));
		for (int j = 0; i+j<15; j++)
			c[j] = b->board[i+j][j];
		c[15 - i] = 3;

		getLineValue(c,num);

		memset(c, 0, sizeof(c));
		for (int j = 0; i+j<15; j++)
			if (i!=0)
				c[j] = b->board[j][i+j];
		c[15 - i] = 3;

		getLineValue(c,num);

		memset(c, 0, sizeof(c));
		for (int j = 0; (i+j)<15; j++)
			c[j] = b->board[i+j][14-j];
		c[15-i] = 3;
	
		getLineValue(c,num);

		memset(c, 0, sizeof(c));
		for (int j = 0; j<=i; j++)
			if (i!=14)
				c[j] = b->board[j][i-j];
		c[i + 1] = 3;
		
		getLineValue(c,num);
		
	}

	int ans = calcuValue(num[player->token]);

	ans -= calcuValue(num[ player->token==1?2:1 ]);

	return ans;
}

void Robot::getLineValue(int c[],int num[][10])
{
	for (int i = 0; i<15; i++)
	{
		if (c[i]==0 || c[i]==3 )
			continue;

		int j = 0;
		while (c[i+j]==c[i] && (i+j)<15 && j<4)
			j++;
		
		if (c[i]!=c[i+j])
			j--;

		if (i==0 && j!=4)
		{
			if (c[i + j + 1] != 0)
			{
				i += j;
				continue;
			}
		}
		else if (c[i - 1] != 0 && c[i + j + 1] != 0 && j!=4)
		{
			i += j;
			continue;
		}else if ( (i==0 || c[i-1]!=0 || c[i+j+1]!=0) && (j!=4) )
			num[ c[i] ][j+6] ++;
		else num[ c[i] ][j+1] ++;

	}
}

int Robot::calcuValue(int num[])
{
	int ans = 0;
	if (num[5]>0)
		ans += value[10] * num[5];
	else if (num[4]>0 || (num[4+5]+num[3] >= 2))
		ans += value[9] * (num[4] + (num[4+5] + num[3])/2);
	else if (num[3]>=2)
		ans += value[8] * ( (num[3])/2 );
	else if (num[3]+num[3+5] >= 2)
		ans += value[7] * ( (num[3] + num[3+5])/2 );
	else if (num[4+5]>0)
		ans += value[6] * num[4+5];
	else if (num[3]>0)
		ans += value[5] * num[3];
	else if (num[2]>=2)
		ans += value[4] * ( (num[2])/2 );
	else if (num[3+5]>0)
		ans += value[3] * num[3+5];
	else if (num[2]>0)
		ans += value[2] * num[2];
	else if (num[2+5]>0)
		ans += value[1] * num[2+5];
	else if ( (num[1] + num[1+5]) > 0)
		ans += value[0] * (num[1] + num[1+5]);

	return ans;
}