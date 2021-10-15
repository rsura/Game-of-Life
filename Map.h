/**
 * Header File: Map.h
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */

#ifndef MAP_H // header guards
#define MAP_H

#include <iostream>
#include <fstream> // includes file input and output
#include <math.h> // includes random and round function
#include <unistd.h> // includes micro sleep function (usleep)
#include <exception> // includes runtime_error error
#include <stdexcept> // includes invalid_argument error

using namespace std;

class Map{
    public:
        Map(); // default constructor - shouldn't be used in general
        Map(unsigned int numRows, unsigned int numColumns); // new map based on number of rows and columns
        Map(string fileName); // creates a new map based on a text file with the contents
        ~Map(); // destructor

        string getMapString(); // gets map in a string format
        void print(); // prints the map
        void populateMap(double popDensity); // randomly populates the map with a given population density
        void makeEmptyMap(); // makes the whole map empty (fills all cells with the '-' character)

        void updateGrid(int rowNum, int columnNum, char val); //updates the map at a certain position with an 'X' or '-'
        char getGridElement(int rowNum, int columnNum); // gets the value of the map at the given position
        unsigned int getNumRows(); // gets number of rows
        unsigned int getNumColumns(); // gets number of columns
        bool isValidFile(string fileName); // checks if the file is valid
    private:
        bool isValidLine(string fileLine); // checks if each line of the given file is valid (helper function to isValidFile)
        char** grid; // the grid of characters that this map
        unsigned int mNumRows; // number of rows
        unsigned int mNumColumns; // number of columns
        string mFileName; // the file name (if passed into one of the overloaded constructors)
};

#endif
