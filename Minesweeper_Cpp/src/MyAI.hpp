// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;

class MyAI : public Agent
{
private:

    double total_time_elapsed = 0.0

    struct Tile{
        int x;
        int y;
    };
    
    int** board;
    
    int totalMines; // number of mines left
    int coveredTiles; // number of covered tiles left
    int flagsSet = 0; // number of flags set
    int rowDim;
    int colDim;
    Tile lastTile; // x, y coordinates of last tile uncovered
    
    queue<Tile> uncoveredFrontier;
    queue<Tile> coveredFrontier;
    
public:

    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;


    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================

    // Helper functions?
    void pickRandomTile();
    

    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
