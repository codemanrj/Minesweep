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

    coveredTiles = rowDimension*colDimension - 1;

    board = new int*[colDimension];

    boardSize = rowDimension;

    for(int i = 0; i < colDimension; i++)
    {
        board[i] = new int[rowDimension];
    }


    for (int i = 0; i < colDimension; i++)
    {
        for (int j = 0; j < rowDimension; j++)
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
    
    //**********************************************************************
    //debugging
    //cout << "getAction called" << endl;
    //cout << "covered Tiles: " << coveredTiles << endl;
    //**********************************************************************
    number = number - getSurroundingFlagged(lastTile);//set number into effective number
    board[lastTile.x][lastTile.y] = number;
    uncoveredFrontier.push({lastTile.x,lastTile.y});//pushes new uncovered tile into queue
    //}        

    if (!actionQueue.empty())//if list of uncover actions is not empty
    {
        Tile curTile = actionQueue.front();
        actionQueue.pop();
        coveredTiles--;
        lastTile = {curTile.x, curTile.y};
        return {UNCOVER, curTile.x, curTile.y};//uncover next item in list
    }
    
    int frontierSize = uncoveredFrontier.size();
    
    for (int k = 0; k < frontierSize; k++)
    {
        if ((totalMines == coveredTiles))//all mines found
        {
            return {LEAVE,-1,-1};
        }
        
        Tile curTile; //why are we declaring a curTile? Don't we already have one defined?

        //Loop that takes out all the uncovered tiles NOT adjacent to covered unflagged tiles out of uncoveredFrontier
        //finds one with surrounding tiles to work with
        do {
        if (uncoveredFrontier.empty())//if nothing left in frontier, means there is an isolated tile
        {
            //uncover a random covered tile neighboring flagged tiles
            while (!flaggedTiles.empty())
            {
                curTile = flaggedTiles.front();
                flaggedTiles.pop();
                if (getSurroundingCovered(curTile) > 0)
                {
                    Tile randNeighbor = generateRandomNeighbor(curTile);
                    lastTile = randNeighbor;
                    coveredTiles--;
                    return {UNCOVER, randNeighbor.x, randNeighbor.y};
                }
            }
            
        }
        curTile = uncoveredFrontier.front(); 
        uncoveredFrontier.pop();
        } while (getSurroundingCovered(curTile) == 0);
        
        //cout << "curTile = x: " << curTile.x + 1 << " y: " << curTile.y + 1 << "num: " << board[curTile.x][curTile.y] << endl;
        
        if (board[curTile.x][curTile.y] == 0)//no mines around current tile
        {
            int coveredNeighbors = getSurroundingCovered(curTile);
            //if EffectiveLabel(x) = 0, then all UnMarkedNeighbors(x) must be safe
            //safely uncover all or push to actionQueue
            for (int i = curTile.x-1; i <= curTile.x+1; i++)
            {
                for (int j = curTile.y-1; j <= curTile.y+1; j++)
                {
                    if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
                    {
                        if (board[i][j] <= coveredNum) //if tile is a covered tile
                        {
                            actionQueue.push({i, j});  
                        }
                    }
                }
            }
            
            curTile = actionQueue.front();
            actionQueue.pop();
            coveredTiles--;
            lastTile = {curTile.x, curTile.y};
            return {UNCOVER, curTile.x, curTile.y};//uncover next item in list
        }

        else if (board[curTile.x][curTile.y] > 0)//1 or more mines around the tile
        {

            //get num of covered neighbors
            int coveredNeighbors = getSurroundingCovered(curTile);

            if(coveredNeighbors == 8) //first move
            {
                if(board[curTile.x][curTile.y] > 2) //if first move has 3 or more surrounding mines, guess again
                {
                    Tile r = generateRandomCoveredNonNeighbor(curTile);
                    lastTile = r;
                    coveredTiles--;
                    return {UNCOVER, r.x, r.y};
                }

            }


            //if effectivelabel(x) = NumUnMarkedNeighbors(x) then all UnMarkedNeighbors(x) must be mines
            if(board[curTile.x][curTile.y] == coveredNeighbors) 
                flagAllCoveredNeighbors(curTile);

            //if curTile has number flagged nieghboring tiles
            // all other surrounding unflagged tiles can be uncovered safely
            if (coveredNeighbors == board[curTile.x][curTile.y])
            {
                for (int i = curTile.x-1; i <= curTile.x+1; i++)
                {
                    for (int j = curTile.y-1; j <= curTile.y+1; j++)
                    {
                        if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
                        {
                            if (board[i][j] <= coveredNum && board[i][j] != flaggedNum) //if tile is a covered tile and not flagged
                            {
                                actionQueue.push({i, j});
                            }
                        }
                    }
                }
                curTile = actionQueue.front();
                actionQueue.pop();
                coveredTiles--;
                lastTile = {curTile.x, curTile.y};
                return {UNCOVER, curTile.x, curTile.y};//uncover next item in list
            }
            if (getSurroundingCovered(curTile) > 0) uncoveredFrontier.push(curTile); //requeue if there are still surrounding covered tiles
        }
    }//for loop
    
    //try model checking if it above logic doesn't return
    vector<Tile> C;
    vector<Tile> U;

    U.push_back(uncoveredFrontier.front());

    bool added = false;
    do
    {
        //cout << "U C init loop" << endl;
        //cout << "U: " << U.size() << "C: " << C.size() << endl;
        added = false;

        for(auto u : U) //for every u in U
        {
            //check neighbors
            for (int i = u.x-1; i <= u.x+1; i++)
            {
                for (int j = u.y-1; j <= u.y+1; j++)
                {
                    if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
                    {
                        if (board[i][j] <= coveredNum) //if tile is a covered tile, next step, check if inC
                        {
                            if(!C.empty())
                            {
                                bool inC = false;

                                //if covered neighbor is not in C
                                for(auto c : C)
                                {
                                    if(c.x == i && c.y == j) 
                                    {
                                        inC = true;
                                        //added = false;
                                    }
                                }
                                if(inC == false)
                                {
                                    C.push_back({i,j});
                                    added = true;
                                }
                            }
                            else //C is Empty so forsure not in C
                            {
                                C.push_back({i, j});
                                added = true;
                            }
                        }
                    }
                }
            }
        } //for loop

        //next check u in U neighbors
        for(auto c : C) //for every c in C
        {
            //check neighbors for new u tiles
            for (int i = c.x-1; i <= c.x+1; i++)
            {
                for (int j = c.y-1; j <= c.y+1; j++)
                {
                    if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
                    {
                        if (board[i][j] > 0) //if tile is a uncovered tile, next step, check if inU
                        {
                           // Tile uTile; //uTile(i, j)
                            //uTile.x = i;
                            //uTile.y = j;

                           // if(!U.empty())
                           // {
                            bool inU = false;
                            
                            Tile uTile;

                            //check for a match in U
                            for(auto u : U)
                            {
                                bool inU = false;

                                //if covered neighbor is not in U
                                for(int k = 0; k < U.size(); k++)
                                {
                                    inU = true;
                                    //added = false;
                                }
                            }

                            //if covered neighbor is not in U, add it
                            if(inU == false)
                            {
                                U.push_back({i,j});
                                added = true;
                            }
                         //   }
                          //  else //U is Empty so forsure not in U
                           // {
                           //     U.push_back(uTile);
                           //     added = true;
                            //}
                        }
                    }
                }
            }
        }
    } while (added == true && U.size()<=18 && C.size()<=18);

    int n = C.size();

    vector<float> prob(n, 0);//vector of probability
    int arr[n];
    int validNum = 0;
    float minProb = 1;
    int minIndex = 0;
    bool flagged = false;
    checkAllBinary(n, arr, 0, U, C, prob, validNum); //all "bit string" vectors
    
    ////////////////////////////////////
    //cout << "validNum: " << validNum << endl;
    //for (int i = 0; i<prob.size(); i++)
    //{
    //    cout << "( " << C.at(i).x + 1 << ", " << C.at(i).y + 1 << ") ";
    //    cout << "prob: "<< prob.at(i) << endl;
    //}
    ////////////////////////////////////
    cout << "validNum: " << validNum << endl;
    for (int i = 0; i<prob.size(); i++)
    {
        cout << "before " << prob.at(i) << endl;
        prob.at(i) = prob.at(i)/validNum;
        cout << prob.at(i) << endl;
        if (prob.at(i) == 1)
        {
            flagged = true;
            flagTile(C.at(i));
        }
        else if (prob.at(i) == 0)
            actionQueue.push(C.at(i));
            
        if (prob.at(i) < minProb)
        {
            minProb = prob.at(i);
            minIndex = i;
        }
    }
    
    if (!actionQueue.empty())//if list of uncover actions is not empty
    {
        Tile curTile = actionQueue.front();
        actionQueue.pop();
        coveredTiles--;
        lastTile = {curTile.x, curTile.y};
        return {UNCOVER, curTile.x, curTile.y};//uncover next item in list
    }
    else if (flagged == false)//if no actions taken, uncover min probability
    {
        //cout << "tMines: " << totalMines << " flags: " << flagsSet << " covTiles: " << coveredTiles << endl;
        float randomProb = totalMines - flagsSet;
        randomProb = randomProb/coveredTiles;
        cout << "minProb: " << prob.at(minIndex) << " randomProb: " << randomProb << endl;
        //cout << "calc done" << endl;
        if (prob.at(minIndex) <= randomProb)
        {
            Tile curTile = C.at(minIndex);
            coveredTiles--;
            lastTile = {curTile.x, curTile.y};
            return {UNCOVER, curTile.x, curTile.y};
        }
        else//if min probability is bigger than probability of picking fully random
        {
            cout << "do random uncover" << endl;
            Tile curTile = generateRandomNonFrontier();
            //cout << "random get" << endl;
            coveredTiles--;
            lastTile = {curTile.x, curTile.y};
            return {UNCOVER, curTile.x, curTile.y};
        }
    }
    //model checking
    
    //}//for loop
    
    //if no actions available, do random
    Tile curTile = uncoveredFrontier.front();
    uncoveredFrontier.pop();
    Tile randNeighbor = generateRandomNeighbor(curTile);
    if (getSurroundingCovered(curTile) > 0) uncoveredFrontier.push(curTile);
    //cout << "Random Tile | x: " << randNeighbor.x + 1 << " y: " << randNeighbor.y + 1 << endl;
    lastTile = randNeighbor;
    coveredTiles--;
    return {UNCOVER, randNeighbor.x, randNeighbor.y};

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
    flaggedTiles.push(myTile);
    flagsSet++;
    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
            {
                if(board[i][j] > 0) //added this so only uncovered tiles with numbers on them are affected
                    board[i][j]--;
            }
        }
    }//subtract the effective number on surrounding tiles
}

//n is length of vector C and bin
void MyAI::checkAllBinary(int n, int bin[], int i, vector<Tile> &U, vector<Tile> &C, vector<float> &prob, int &validNum)
{
    if(i==n)
    {   
        for (int j = 0; j<n; j++)//set dummy flags on all C[i] tiles when bin[i] == 1 
        {
            if (bin[j] == 1)
                board[C.at(j).x][C.at(j).y] = dummyFlag;
        }
        
        bool valid = true;
        //for all uncovered tiles, check if the current model is valid
        for (auto myTile : U)
        {
            if (board[myTile.x][myTile.y] < getSurroundingDummy(myTile))
                valid = false;
        }

        if (valid == true)
        {
            validNum++;
            for (int j=0; j<n; j++)
            {
                prob.at(j) = prob.at(j) + bin[j];
            }
        }
        

        for (auto myTile : C)//resets all values
        {
            //cout << "x: " << myTile.x + 1 << "y: " << myTile.y + 1 << endl;
            board[myTile.x][myTile.y] = coveredNum;
        }

        return;
    }//if (i==n)

    bin[i] = 0;
    checkAllBinary(n, bin, i+1, U, C, prob, validNum);

    //add a check so that there won't be models with no of 1 > mines left
    bin[i] = 1;
    checkAllBinary(n, bin, i+1, U, C, prob, validNum);

}
        
int MyAI::getSurroundingCovered(Tile myTile)
{
    int count = 0;

    //returns a count of the covered neighbors

    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
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
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
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
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
            {
                if (board[i][j] <= flaggedNum && board[i][j] > coveredNum) //if tile is a flagged tile
                {
                    count++;
                }
            }
        }
    }
    return count;

}

int MyAI::getSurroundingDummy(Tile myTile)
{
     int count = 0;

    //returns a count of the covered neighbors

    for (int i = myTile.x-1; i <= myTile.x+1; i++)
    {
        for (int j = myTile.y-1; j <= myTile.y+1; j++)
        {
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
            {
                if (board[i][j] == dummyFlag) //if tile is dummy flagged
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
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
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
            if (i >= 0 && i < colDimension && j >= 0 && j < rowDimension)
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

MyAI::Tile MyAI::generateRandomCoveredNonNeighbor(Tile t)
{
    vector<Tile> random;

    for (int i = 0; i < colDimension; i++)
    {
        for(int j = 0; j < rowDimension; j++)
        {
            if( !(i < t.x-2 && i > t.x+2) && !(j < t.y-2 && j > t.y+2))
            {
                if(board[i][j] == coveredNum)
                {
                    random.push_back({i,j});
                }
            }
        }
    }

    int size = random.size();

    return random.at(rand()%size);
}


MyAI::Tile MyAI::generateRandomNonFrontier()
{
    vector<Tile> random;
    vector<Tile> allRandom;

    for (int i = 0; i < colDimension; i++)
    {
        for(int j = 0; j < rowDimension; j++)
        {
                if(board[i][j] <= coveredNum)
                {
                    allRandom.push_back({i,j});
                    int covered = getSurroundingCovered({i,j});
                    int totalNeigh = getTotalNeighbors({i,j});

                    if((totalNeigh - covered) == 0) //if uncovered neighbors = 0, it is not in the covered frontier
                    {
                        random.push_back({i,j});
                    }
                    //else //else it is part of the uncovered frontier
                    //{
                    //    random.pop_back();
                    //}
                }
        }
    }
    int size = random.size();
    if (size == 0) 
    {
        size = allRandom.size();
        return allRandom.at(rand()%size);
    }
    else 
    {
        return random.at(rand()%size);
    }

}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
