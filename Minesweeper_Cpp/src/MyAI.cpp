// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    
    lastTile.x = _agentX;
    lastTile.y = _agentY;
    totalMines = _totalMines;
    rowDim = _rowDimension;
    colDim = _colDimension;
    
    Tile tempTile;
    tempTile.x = _agentX;
    tempTile.y = _agentY;
    uncoveredFrontier.push(tempTile);
    
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    if (number != -1)//if last action was uncover
    {
        board[lastTile.x][lastTile.y] = number;
        
    }
    
    Tile curTile;
    curTile = uncoveredFrontier.front();

    return {LEAVE,-1,-1};
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}


// ======================================================================
// YOUR CODE BEGINS
// ======================================================================



// ======================================================================
// YOUR CODE ENDS
// ======================================================================
