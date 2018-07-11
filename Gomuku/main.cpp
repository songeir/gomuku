
#include "gomuku.h"
#include "robot.h"

#include <iostream>

int main()
{
    Board *board = new Board();
    Player *player1 = new Player(board,1);
    Player *player2 = new Player(board,2);

    while (!board->isEnd())
    {
        for (int i = 0; i<15; i++)
        {
            for (int j = 0; j<15; j++)
                std::cout << " " << board->board[i][j];
            std::cout << std::endl;
        }
        int x,y;
        std::cin >> x >> y;

        //std::cout << board->currentPlayer << std::endl;

        if (board->player[board->currentPlayer]->makeMove( Node(x,y,board->player[board->currentPlayer]) ))
        {
            board->currentPlayer = board->currentPlayer==1?2:1;

            if (board->player[board->currentPlayer]->makeMove( Robot::makeAutoMove(board->player[board->currentPlayer], 0)))
                board->currentPlayer = board->currentPlayer==1?2:1;
        }
    }

    return 0;
}