/**
 * Header File: ClassicMode.h
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */

#ifndef CLASSICMODE_H // header guards
#define CLASSICMODE_H

#include "GenericGame.h"

class ClassicMode : public GenericGame{ // ClassicMode inheriting from abstract GenericGame
    public:
        ClassicMode(); // default constructor - shouldn't be used in general
        ClassicMode(unsigned int numRows, unsigned int numColumns, double popDensity); // new ClassicMode based on number of rows and columns and population density
        ClassicMode(string fileName); // new ClassicMode based on file name
        ~ClassicMode(); // destructor implemented here, in the sub class, instead of the abstract class, GenericGame

    private:
        // implemented here, in the sub class, instead of the abstract class, GenericGame
        // creates the bufferGrid based on the rules of ClassicMode
        void generateBufferedGrid(Map*& smallerGrid) const;
};

#endif
