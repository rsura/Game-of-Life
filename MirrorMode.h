/**
 * Header File: MirrorMode.h
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */

#ifndef MIRRORMODE_H // header guards
#define MIRRORMODE_H

#include "GenericGame.h"

class MirrorMode : public GenericGame{ // MirrorMode inheriting from abstract GenericGame
    public:
        MirrorMode(); // default constructor - shouldn't be used in general
        MirrorMode(unsigned int numRows, unsigned int numColumns, double popDensity); // new MirrorMode based on number of rows and columns and population density
        MirrorMode(string fileName); // new MirrorMode based on file name
        ~MirrorMode(); // destructor implemented here, in the sub class, instead of the abstract class, GenericGame

    private:
        // implemented here, in the sub class, instead of the abstract class, GenericGame
        // creates the bufferGrid based on the rules of MirrorMode
        void generateBufferedGrid(Map*& smallerGrid) const;
};

#endif
