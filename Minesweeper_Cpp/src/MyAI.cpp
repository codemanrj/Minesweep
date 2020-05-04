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
    rowDimension = _rowDimension;
    colDimension = _colDimension;

    coveredTiles = rowDimension*colDimension;

    board = new int*[rowDimension];

    boardSize = rowDimension;

    for(int i = 0; i < rowDimension; i++)
    {
        board[i] = new int[colDimension];
    }


    for (int i = 0; i < rowDimension; i++)
    {
        for (int j = 0; j < colDimension; j++)
        {
            board[i][j] = coveredNum; //covered tiles
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
    
    //do while there is remaining time
    
    int buf = 1;
    
    for (int k = 0; k < uncoveredFrontier.size() + buf; k++)
    {
        buf = 0;
        if ((totalMines == coveredTiles))//all mines found
        {
            return {LEAVE,-1,-1};
        }

        //if (number != -1)//if last action was uncover
        //always the case
        //number will return -1 if last action is flag
        if (number >= 0) 
        {
            board[lastTile.x][lastTile.y] = number;
            uncoveredFrontier.push({lastTile.x,lastTile.y});//pushes new uncovered tile into queue
        }


        Tile curTile;
        curTile = uncoveredFrontier.front();
        if (board[curTile.x][curTile.y] == 0)//no mines around current tile
        {
            int coveredNeighbors = getSurroundingCovered(curTile);
            //if EffectiveLabel(x) = 0, then all UnMarkedNeighbors(x) must be safe

            for (int i = curTile.x-1; i <= curTile.x+1; i++)
            {
                for (int j = curTile.y-1; j <= curTile.y+1; j++)
                {
                    if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
                    {
                        if (board[i][j] <= coveredNum) //if tile is a covered tile
                        {
                            lastTile = {i, j};
                            coveredTiles--;
                            return {UNCOVER, i, j};  
                        }
                    }
                }
            }
            if (coveredNeighbors == 0) uncoveredFrontier.pop();
        }

        else if (board[curTile.x][curTile.y] > 0)//1 or more mines around the tile
        {

            //subtract num of tiles around the flagged tile
            int coveredNeighbors = getSurroundingCovered(curTile);

            //if effectivelabel(x) = NumUnMarkedNeighbors(x) then all UnMarkedNeighbors(x) must be mines
            if(board[curTile.x][curTile.y] == coveredNeighbors) 
                flagAllCoveredNeighbors(curTile);

            //add case for random pick (how to detect when to use random?)

            //if all the neighbors are covered, randomly pick one
            int totalNeighbors = getTotalNeighbors(curTile);

            if(coveredNeighbors == totalNeighbors) //or if covered neighbors equals total neighbors
            {
                Tile randNeighbor = generateRandomNeighbor(curTile);
                coveredTiles--;
                return {UNCOVER, randNeighbor.x, randNeighbor.y};
            }

            //if curTile has number flagged nieghboring tiles
            // all other surrounding unflagged tiles can be uncovered safely
            int flaggedNeighbors = getSurroundingFlagged(curTile);

            if (flaggedNeighbors == number)
            {
                for (int i = curTile.x-1; i <= curTile.x+1; i++)
                {
                    for (int j = curTile.y-1; j <= curTile.y+1; j++)
                    {
                        if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
                        {
                            if (board[i][j] <= coveredNum && board[i][j] != flaggedNum) //if tile is a covered tile and not flagged
                            {
                                coveredTiles--;
                                return {UNCOVER, i, j};
                            }
                        }
                    }
                }
            }





            uncoveredFrontier.pop(); //pops if no action taken
            if (coveredNeighbors > 0) uncoveredFrontier.push(curTile); //requeue if there are still surrounding covered tiles
        }
    }//for loop

    return {LEAVE,-1,-1};
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================

}


// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
        
void MyAI::flagTile(Tile myTile)
{
    board[myTile.x][myTile.y] = flaggedNum;
    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
            {
                if(board[i][j] > 0) //added this so only uncoverd tiles with numbers on them are affected
                    board[i][j]--;
            }
        }
    }//subtract the effective number on surrounding tiles
}
        
int MyAI::getSurroundingCovered(Tile myTile)
{
    int count = 0;

    //returns a count of the covered neighbors

    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
            {
                if (board[i][j] <= coveredNum) //if tile is a covered tile
                {
                    count++;
                }
            }
        }
    }
    return count;
}

int MyAI::getTotalNeighbors(Tile myTile)
{
    int count = 0;

    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
            {
                    count++;
            }
        }
    }

    return count-1;
}

int MyAI::getSurroundingFlagged(Tile myTile)
{
     int count = 0;

    //returns a count of the covered neighbors

    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
            {
                if (board[i][j] <= flaggedNum) //if tile is a covered tile
                {
                    count++;
                }
            }
        }
    }
    return count;

}

void MyAI::flagAllCoveredNeighbors(Tile t)
{
    for (int i = t.x-1; i <= t.x+1; i++)
    {
        for (int j = t.y-1; j <= t.y+1; j++)
        {
            if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
            {
                if (board[i][j] <= coveredNum) //if tile is a covered tile
                {
                    flagTile({i, j});
                }
            }
        }
    }
}

MyAI::Tile MyAI::generateRandomNeighbor(Tile t)
{
    int size = getSurroundingCovered(t);

    Tile array[size];

    int count = 0;

    for (int i = t.x-1; i <= t.x+1; i++)
    {
        for (int j = t.y-1; j <= t.y+1; j++)
        {
            if (i >= 0 && i < rowDimension && j >= 0 && j < colDimension)
            {
                if (board[i][j] <= coveredNum) //if tile is a covered tile
                {
                    array[count] = {i, j};
                    count++;
                }
            }
        }
    }

    return array[rand() % size];
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
