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
    
    coveredTiles = rowDim*colDim - 1;
    
    
    Tile tempTile = {
    _agentX,
    _agentY,
    };
    
    for (int i = 0; i <= colDim; i++)
    {
        for (int j = 0; j <= rowDim; j++)
        {
            board[i][j] = -1; //covered tiles
        }
    }
    
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    
    if (totalMines == 0 && coveredTiles == 0)//all mines found
    {
        return {LEAVE,-1,-1};
    }
    
    if (number != -1)//if last action was uncover
    {
        board[lastTile.x][lastTile.y] = number;
        uncoveredFrontier.push({lastTile.x,lastTile.y});//pushes new uncovered tile into queue
    }
    
    Tile curTile;
    curTile = uncoveredFrontier.front();
    if (board[curTile.x][curTile.y] == 0)//no mines around current tile
    {
        for (int i = curTile.x-1; i <= curTile.x+1; i++)
        {
            for (int j = curTile.y-1; j <= curTile.y+1; j++)
            {
                if (i >= 0 && i < colDim && j >= 0 && j>= rowDim)
                {
                    if (board[i][j] == -1) 
                    {
                        coveredTiles--;
                        lastTile = {i, j};
                        return {UNCOVER, i, j};
                    }
                }
            }
        }
        uncoveredFrontier.pop();//remove from queue when all neighbors uncovered
    }
    else if (board[curTile.x][curTile.y] == 1)//1 mine around the tile
    {
        if (coveredTiles == totalMines)//if rest of covered tiles are all mines
        {
            //flag all covered tiles around current tile
            //subtract num of tiles around the flagged tile
            //and subtract totalMines for each flag
        }
        //if (surrounding covered tiles == number on tile)
        //flag all covered tiles around current tile
        //subtract num of tiles around the flagged tile
    }

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
