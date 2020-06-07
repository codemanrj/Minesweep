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
public:

    struct Tile{
        int x;
        int y;
        //could add more data:
        //number of surrounding covered tiles
        //
    };
    
    int** board = NULL;
    const int coveredNum = -100;
    const int flaggedNum = -50;
    const int dummyFlag = -150;
    int boardSize = 0;
    
    //int totalMines; // number of mines left
    int coveredTiles; // number of covered tiles left on the board
    int flagsSet = 0; // number of flags set
    //int rowDim;
    //int colDim;
    Tile lastTile; // x, y coordinates of last tile uncovered
    
    queue<Tile> actionQueue;
    queue<Tile> uncoveredFrontier;
    queue<Tile> coveredFrontier;
    queue<Tile> flaggedTiles;
    
    void flagTile(Tile myTile);//flags myTile
    int getSurroundingCovered(Tile myTile);//returns number of covered tiles around the myTile
    
public:

    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    ~MyAI()
    {
        for(int i = 0; i < boardSize; i++)
        {
            delete[] board[i];
            board[i] = NULL;
        }

        delete[] board;
        board = NULL;
    }

    Action getAction ( int number ) override;


    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
    
    void checkAllBinary(int n, int bin[], int i, vector<Tile> &U, vector<Tile> &C, vector<float> &prob, int &validNum);
    void flagAllCoveredNeighbors(Tile t);
    void uncoverAllCoveredNeighbors(Tile t);
    Tile generateRandomNeighbor(Tile t);
    Tile generateRandomCoveredNonNeighbor(Tile t);
    int getSurroundingFlagged(Tile myTile);
    int getSurroundingDummy(Tile myTile);
    int getTotalNeighbors(Tile myTile);
   
    
    

    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP