#ifndef ROBOT_H
#define ROBOT_H

#include "pch.h"
#include "gomuku.h"

class Robot
{
private:
    static const int value[11];
    static void getLineValue(int[],int[][10]);
    static int calcuValue(int[11]);
    static int getValue(Player*);
    static bool check(Player*,int,int);

public:

	static int maxDepth;
    static Node makeAutoMove(Player*,int);
    
};

#endif