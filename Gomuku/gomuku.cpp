#include "pch.h"
#include "gomuku.h"

#define LENG 15

Board::Board()
{
    memset(board,0,sizeof(board));

    currentPlayer = 1;
    while (!back.empty())
        back.pop();
}

Board::~Board()
{
	delete player[1];
	delete player[2];
}

int Board::getStatus(const Node &point) const
{
    return board[point.x][point.y]; 
}

bool Board::isEnd() const
{
    return (getWinner()!=0) ;
}

int Board::getWinner() const
{
    int winner = 0;

    for (int i = 0; i<LENG; i++)
        for (int j = 0; j<LENG; j++)
            {
                winner = getPointWinner( Node(i,j,NULL) );
                if (winner)
                    return winner;
            }

    return winner;
}

const int xp[] = {0,1,1,1};
const int yp[] = {1,0,1,-1};
int Board::getPointWinner(const Node &point) const
{   
    int ret=0;

    if (board[point.x][point.y]==0)
        return 0;

    for (int i = 0; i<4; i++)
    {
        int x = point.x,y = point.y;
        int k;
        for (k = 0; k<4; k++, x+=xp[i], y+=yp[i])
            if (board[x][y] != board[x+xp[i]][y+yp[i]])
                break;

        if (k==4)
        {
            ret = board[point.x][point.y];
            return ret;
        }
    }

    return 0;
}

Player::Player(Board* board, int token):board(board),token(token)
{
    this->board->player[token] = this;
}

bool Player::makeMove(const Node& point)
{
    if (check(point))
    {
        this->board->board[point.x][point.y] = token;
        this->board->back.push(point);

        return true;
    }

    return false;
}

bool Player::check(const Node &point) const
{
    if (point.x <0 || point.x>=15 || point.y<0 || point.y>=15)
        return false;
    if (this->board->board[point.x][point.y] != 0)
        return false;

    return true;
}

bool Player::takeBack()
{
    Board *&b = this->board;

    if (b->back.empty())
        return false;

    Node point = b->back.top();
    b->back.pop();
    if (b->back.empty())
    {
        b->back.push(point);
        return false;
    }

    b->board[point.x][point.y] = 0;

    point = b->back.top();
    b->back.pop();
    b->board[point.x][point.y] = 0;

    return true;
}