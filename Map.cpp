/**
 * Implementation File: Map.cpp
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */
#include "Map.h"

using namespace std;

/**
 * Default constructor:
 * Creates a map with default dimensions of 7x7 and a population density of 0.5
 */
Map::Map(){
    // creates a 7 x 7 grid with no file name (because no file was provided)
    mNumRows = 7;
    mNumColumns = 7;
    mFileName = "";

    // next 4 lines allocate memory to the char**
    grid = new char*[mNumRows];
    for (int i = 0; i < mNumRows; ++i){
        grid[i] = new char[mNumColumns];
    }
    makeEmptyMap(); // makes each cell in the grid to '-'
    populateMap(0.5); // populates this default map with population density of 0.5
}

/**
 * Overloaded constructor:
 * Creates a map with dimensions of given number of rows and columns
 * @param an unsigned int representing number of rows
 * @param an unsigned int representing number of columns
 */
Map::Map(unsigned int numRows, unsigned int numColumns){
    // creates a numRows x numColumns grid with no file name (because no file was provided)
    mNumRows = numRows;
    mNumColumns = numColumns;
    mFileName = "";

    // next 4 lines allocate memory to the char**
    grid = new char*[mNumRows];
    for (int i = 0; i < mNumRows; ++i){
        grid[i] = new char[mNumColumns];
    }
    makeEmptyMap(); // makes each cell in the grid to '-'
}

/**
 * Overloaded constructor:
 * Creates a map from a file where the first two lines of the file are positive integers representing number of rows and columns
 * and the rest of the lines of the file represents generation 0 of the board with 'X's and '-'s all separated by lines
 * @param a string representing the name of the file (or the relative/absolute file path if it's in a different directory)
 */
Map::Map(string fileName){
    // creating input file stream and opening the file with the name of fileName
    ifstream fileReader;
    fileReader.open(fileName);

    if(!fileReader.is_open()){ // if the file doesn't exist or doesn't have the right reading permissions
        string runtimeMessage = "ERROR: No file found with the name of \"" + fileName + "\"\n";
        runtimeMessage += "Please check if the file is in the wrong directory, has the proper reading permissions or if you have provided the extension for the file name";
        throw runtime_error(runtimeMessage); // throws the error message
    } else {
        mFileName = fileName; // assigns member variable with file name
        if (!isValidFile(fileName)) { // if the file has a valid format (a method that is described below)
            string runtimeMessage = "ERROR: INVALID FILE INPUT";
            runtimeMessage += "\nThe file had invalid inputs. Please make sure that the following are true:";
            runtimeMessage += "\n\t1. The first line is a single positive integer representing the length of the row";
            runtimeMessage += "\n\t2. The second line is a single positive integer representing the length of the column";
            runtimeMessage += "\n\t3. The grid has the same dimensions as stated in lines 1 and 2 and contains ONLY a \'X\' or a \'-\' for each character";
            throw runtime_error(runtimeMessage); // throws the error message
        } else {
            string fileLine; // used to get the string for each line in the file

            getline(fileReader, fileLine);
            mNumRows = stoi(fileLine); // number of rows from first line of the file

            getline(fileReader, fileLine); // number of columns from second line of the file
            mNumColumns = stoi(fileLine);

            // next 4 lines allocate memory to the char**
            grid = new char*[mNumRows];
            for (int i = 0; i < mNumRows; ++i){
                grid[i] = new char[mNumColumns];
            }

            // assigns each cell of the grid to the upper case version of the file's line at a certain character (in case the file has lower case 'x's)
            int currRow = 0;
            while (getline (fileReader,fileLine)) {
                for (int i = 0; i < mNumColumns; ++i) {
                    grid[currRow][i] = toupper(fileLine[i]);
                }
                ++currRow;
            }

            if (!fileReader.eof()) { // if for some reason the file reader doesn't reach the endl of the file
                throw runtime_error("File wasn't able to be read until the end"); // throws the error message
            }
        }
    }
    fileReader.close();

}

/**
 * Destructor:
 * Deletes the underlying char** grid of the map.
 */
Map::~Map(){
    if ((mNumRows > 0 && mNumColumns > 0 && grid != NULL) || (mFileName != "" && isValidFile(mFileName))) { // makes sure that the Map was fully created in the first place
        for (int i = 0; i < mNumRows; ++i){ // deletes each sub array in the 2d array
            if (grid[i] != NULL) {
                delete [] grid[i];
            }
        }
        delete [] grid; // deletes overall array
    }
}

/**
 * Gives the string version of the map for easy printing. Each row of the map is separated by '\n' characters when the string is given back
 * @returns string version of the map
 */
string Map::getMapString(){
    string mapStr = "";
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumColumns; ++j) {
            mapStr += grid[i][j]; // adding each character to the mapStr string
        }
        mapStr += "\n"; // separates each line of the map with this character
    }
    mapStr = mapStr.substr(0, mapStr.length() - 1); // removes last '\n' character
    return mapStr;
}

/**
 * Prints the map's string to the terminal window
 */
void Map::print(){
    cout << getMapString() << endl; // just prints the map's string from getMapString()
}

/**
 * Makes the map empty by assigning all the cells with '-'
 */
void Map::makeEmptyMap(){
    for (int i = 0; i < mNumRows; ++i) {
        for (int j = 0; j < mNumColumns; ++j) {
            grid[i][j] = '-';
        }
    }
}

/**
 * Populates the map with a given population density
 *
 * @param a double from 0 to 1 representing the percentage of cells that are filled
 */
void Map::populateMap(double popDensity){
    if (popDensity < 0 || popDensity > 1) { // if the population density is not in the desired range, it throws an error
        throw runtime_error("ERROR: Invalid population density. Needs a decimal value from 0 to 1");
    }

    makeEmptyMap(); // makes map empty in case
    int totCells = round((popDensity * mNumRows) * mNumColumns); // total cells needed to be filled
    int currentlyFilled = 0;
    int randomRow;
    int randomColumn;

    while (currentlyFilled < totCells) {
        // randomly generated position
        randomRow = rand() % mNumRows;
        randomColumn = rand() % mNumColumns;

        if (grid[randomRow][randomColumn] == '-') { // if the cell is empty, fill it and increment currently filled cells
            grid[randomRow][randomColumn] = 'X';
            ++currentlyFilled;
        }
    }
}

/**
 * Helper function - checks if a line from the file is valid (filled with 'X' or '-') and is the same as mNumColumns
 *
 * @param a string representing a row of cells
 * @returns true if the line is valid, false if it's not
 */
bool Map::isValidLine(string fileLine){
    if (fileLine.length() != mNumColumns) { // if there are too little or too many cells for a given row
        return false;
    }
    for (int i = 0; i < fileLine.length(); ++i) {
        if (fileLine[i] != '-' && toupper(fileLine[i]) != 'X') { // if the cell is not a '-' or 'X' (or lower case 'x' too)
            return false;
        }
    }
    return true;
}

/**
 * Checks if the given file for creating a map is valid
 *
 * @param a string representing the name of the file (or the relative/absolute file path if it's in a different directory)
 * @returns true if the file is valid, false if it's not
 */
bool Map::isValidFile(string fileName){
    ifstream fileReader;
    fileReader.open(fileName);

    if(!fileReader.is_open()){ // if file doesn't exist
        return false;
    }

    string fileLine; // string representing each line of the file
    int numRows;
    int numColumns;

    getline(fileReader, fileLine);
    if (fileLine.empty()) { // if nothing on line 1
        fileReader.close();
        return false;
    }
    for (int i = 0; i < fileLine.length(); ++i) { // checks that each character of the line is a number
        if (!isdigit(fileLine[i])){
            fileReader.close();
            return false;
        }
    }

    try{ // even if the line is a number, it should be an integer (not larger than the integer limit)
        numRows = stoi(fileLine);
    } catch(invalid_argument &exception) {
        return false;
    }

    getline(fileReader, fileLine);
    if (fileLine.empty()) { // if nothing on line 2
        fileReader.close();
        return false;
    }
    for (int i = 0; i < fileLine.length(); ++i) { // checks that each character of the line is a number
        if (!isdigit(fileLine[i])){
            fileReader.close();
            return false;
        }
    }

    try{ // even if the line is a number, it should be an integer (not larger than the integer limit)
        numColumns = stoi(fileLine);
    } catch(invalid_argument &exception) {
        return false;
    }

    mNumRows = numRows;
    mNumColumns = numColumns;


    int currRow = 0;
    while (getline (fileReader,fileLine)) {
        if (!isValidLine(fileLine)) { // checks if each line is valid
            fileReader.close();
            return false;
        }
        ++currRow;
    }
    if (currRow != numRows) { // checks if number of rows in the file match the first integer mentioned on line 1
        fileReader.close();
        return false;
    }
    fileReader.close();
    return true;

}

/**
 * Updates a certain spot in the Map with an 'X' or a '-'
 *
 * @param An integer representing the row number (first row is row 0)
 * @param An integer representing the column number (first column is column 0)
 * @param A char representing if the cell is filled or not with an 'X' or a '-'
 */
void Map::updateGrid(int rowNum, int columnNum, char val){
    // updates grid if it's a valid spot and value of the character is '-' or 'X' (or the lowercase 'x')
    if ((rowNum > -1 && rowNum < mNumRows) && (columnNum > -1 && columnNum < mNumColumns) && (val == '-' || toupper(val) == 'X')) {
        grid[rowNum][columnNum] = toupper(val);
    } else {
        throw runtime_error("ERROR: Not a valid way to update the grid"); // throws error other wise
    }
}

/**
 * Gets the element of the map's grid
 *
 * @param An integer representing the row number (first row is row 0)
 * @param An integer representing the column number (first column is column 0)
 * @returns a char 'X' or '-' (filled cell or empty cell)
 */
char Map::getGridElement(int rowNum, int columnNum){
    return grid[rowNum][columnNum];
}

/**
 * Gets the number of rows of the grid
 *
 * @returns an unsigned int representing number of rows
 */
unsigned int Map::getNumRows(){
    return mNumRows;
}

/**
 * Gets the number of columns of the grid
 *
 * @returns an unsigned int representing number of columns
 */
unsigned int Map::getNumColumns(){
    return mNumColumns;
}
