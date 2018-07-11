#ifndef GOMUKU_H
#define GOMUKU_H

#include "pch.h"

#include <stack>
#include <cstring>

class Board;
class Player;

struct Node{
    int x,y;
    Player* player;
    int value;

    Node(int x=0, int y=0, Player* player=NULL):x(x),y(y),player(player)    {}

    const bool operator < (const Node &tmp)const{
        return this->value > tmp.value; 
    }
};

class Board
{
private:
    
    int getPointWinner(const Node&) const;

public:
    int board[16][16];
    std::stack<Node> back;
    Player* player[3];
    int currentPlayer;
    
    Board();
	~Board();

    int getStatus(const Node&) const;
    bool isEnd() const;
    
    int getWinner() const;
};

class Player
{
private:
    
    bool check(const Node&) const;

public:
    Board* board;
    int token;
    
    Player(Board*,int);

    bool makeMove(const Node&);
    bool takeBack();
};

#endif