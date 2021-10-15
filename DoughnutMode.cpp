/**
 * Implementation File: DoughnutMode.cpp
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */
#include "DoughnutMode.h"

using namespace std;

/**
 * Default constructor:
 * Creates a doughnut mode simulation where the starting map is a default 7 x 7 map with a population density of 0.5
 */
DoughnutMode::DoughnutMode() : GenericGame::GenericGame(){
    // Creates mGenTwo, mGenThree, and mGenFour based off of previous generations
    createNextGen(mGenOne, mGenTwo);
    createNextGen(mGenTwo ,mGenThree);
    createNextGen(mGenThree, mGenFour);
}

/**
 * Overloaded constructor:
 * Creates a doughnut mode simulation where the starting map is a map with given dimensions and a given population density
 *
 * @param an unsigned int representing number of rows
 * @param an unsigned int representing number of columns
 * @param a double representing population density
 */
DoughnutMode::DoughnutMode(unsigned int numRows, unsigned int numColumns, double popDensity) : GenericGame::GenericGame(numRows, numColumns, popDensity){
    // Creates mGenTwo, mGenThree, and mGenFour based off of previous generations
    createNextGen(mGenOne, mGenTwo);
    createNextGen(mGenTwo, mGenThree);
    createNextGen(mGenThree, mGenFour);
}

/**
 * Overloaded constructor:
 * Creates a doughnut mode simulation from a file that represents the starting map of generation 0
 *
 * @param a string representing the file name
 */
DoughnutMode::DoughnutMode(string fileName) : GenericGame::GenericGame(fileName){
    // Creates mGenTwo, mGenThree, and mGenFour based off of previous generations
    createNextGen(mGenOne, mGenTwo);
    createNextGen(mGenTwo, mGenThree);
    createNextGen(mGenThree, mGenFour);
}

/**
 * Destructor - deleting all the maps of the last four generations and the buffered grid
 */
DoughnutMode::~DoughnutMode(){
    // deletes all maps
    delete mGenOne;
    delete mGenTwo;
    delete mGenThree;
    delete mGenFour;
    delete bufferGrid;
}

/**
 * Creates the buffered grid of a Map based on the rules of doughnut mode
 *
 * @param a Map
 */
void DoughnutMode::generateBufferedGrid(Map*& smallerGrid) const{
    bufferGrid->makeEmptyMap(); // makes bufferGrid empty
    for (int i = 0; i < smallerGrid->getNumRows(); ++i) {
        for (int j = 0; j < smallerGrid->getNumColumns(); ++j) {
            // fills the inner part of the bufferGrid (non-edge cells) with the smallerGrid Map parameter
            bufferGrid->updateGrid(i+1, j+1, smallerGrid->getGridElement(i,j));
        }
    }

    // Wraps each of the opposite corners' value to the opposite corner in the bufferGrid
    bufferGrid->updateGrid(0, 0, bufferGrid->getGridElement(bufferGrid->getNumRows() - 2, bufferGrid->getNumColumns() - 2)); // top left corner
    bufferGrid->updateGrid(0, bufferGrid->getNumColumns() - 1, bufferGrid->getGridElement(bufferGrid->getNumRows() - 2, 1)); // top right corner
    bufferGrid->updateGrid(bufferGrid->getNumRows() - 1, 0, bufferGrid->getGridElement(1, bufferGrid->getNumColumns() - 2)); // bottom left corner
    bufferGrid->updateGrid(bufferGrid->getNumRows() - 1, bufferGrid->getNumColumns() - 1, bufferGrid->getGridElement(1, 1)); // bottom right corner

    // copies the opposite columns' values to the opposite column in the bufferGrid (excluding corners)
    for (int i = 1; i < bufferGrid->getNumRows() - 1; ++i) {
        bufferGrid->updateGrid(i, 0, bufferGrid->getGridElement(i, bufferGrid->getNumColumns() - 2));
        bufferGrid->updateGrid(i, bufferGrid->getNumColumns() - 1, bufferGrid->getGridElement(i, 1));
    }

    // copies the opposite rows' values to the opposite rows in the bufferGrid (excluding corners)
    for (int i = 1; i < bufferGrid->getNumColumns() - 1; ++i) {
        bufferGrid->updateGrid(0, i, bufferGrid->getGridElement(bufferGrid->getNumRows() - 2, i));
        bufferGrid->updateGrid(bufferGrid->getNumRows() - 1, i, bufferGrid->getGridElement(1, i));
    }
}
