/**
 * Header File: GenericGame.h
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */

#ifndef GENERICGAME_H // header guards
#define GENERICGAME_H

#include "Map.h"
using namespace std;

class GenericGame{
    public:
        GenericGame(); // default constructor - shouldn't be used in general
        GenericGame(unsigned int numRows, unsigned int numColumns, double popDensity); // new game based on number of rows and columns and population density
        GenericGame(string fileName); // new game based on file name
        virtual ~GenericGame(){} // virtual destructor - must be implemented in sub classes
        void play(); // starts the game
    protected:
        bool isStable() const; // checks if the map has stabilized
        void createNextGen(Map*& oldGen, Map*& newGen) const; // creates the next generation from a given map and a new map
        void consoleWithPause(); // method that lets the user watch an "animation" of the cells on terminal window with pauses in between
        void consoleWithEnter(); // method that lets the user press enter to see each generation of cells on terminal window
        void fileOutput(); // method that lets the user output all the generations (until stabilized) into a file
        virtual void generateBufferedGrid(Map*& smallerGrid) const = 0; // creates the buffered grid - must be implemented in the sub classes

        // counts the number of neighbors of cells that are not on the edge of the Map - helper function to createNextGen and uses bufferGrid
        int countInternalNeighbors(int rowNum, int columnNum) const;

        // The 4 maps below keeps track of the last four generations, so that if it stabilizes, we can end the game
        Map* mGenOne;
        Map* mGenTwo;
        Map* mGenThree;
        Map* mGenFour;

        Map* bufferGrid; // Map for the buffered grid



};

#endif
