/**
 * Implementation File: GenericGame.cpp
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */
#include "GenericGame.h"

using namespace std;


/**
 * Default constructor:
 * Creates a simulation where the starting map is a default 7 x 7 map with a population density of 0.5
 */
GenericGame::GenericGame(){
    mGenOne = new Map(); // new default map
    bufferGrid = new Map(mGenOne->getNumRows() + 2, mGenOne->getNumColumns() + 2); // creates an empty buffered grid for using later
    mGenTwo = new Map(); // new map for generation 2
    mGenTwo->makeEmptyMap();
    mGenThree = new Map(); // new map for generation 3
    mGenThree->makeEmptyMap();
    mGenFour = new Map(); // new map for generation 4
    mGenFour->makeEmptyMap();
}

/**
 * Overloaded constructor:
 * Creates a simulation where the starting map is a map with given dimensions and a given population density
 *
 * @param an unsigned int representing number of rows
 * @param an unsigned int representing number of columns
 * @param a double representing population density
 */
GenericGame::GenericGame(unsigned int numRows, unsigned int numColumns, double popDensity){
    mGenOne = new Map(numRows, numColumns); // new numRows x numColumns map
    mGenOne->populateMap(popDensity); // populates the map
    bufferGrid = new Map(mGenOne->getNumRows() + 2, mGenOne->getNumColumns() + 2); // creates an empty buffered grid for using later
    mGenTwo = new Map(numRows, numColumns); // new numRows x numColumns map for generation 2
    mGenThree = new Map(numRows, numColumns); // new numRows x numColumns map for generation 3
    mGenFour = new Map(numRows, numColumns); // new numRows x numColumns map for generation 4
}

/**
 * Overloaded constructor:
 * Creates a simulation from a file that represents the starting map of generation 0
 *
 * @param a string representing the file name
 */
GenericGame::GenericGame(string fileName){
    try{
        mGenOne = new Map(fileName); // new map from fileName
    } catch (invalid_argument &exception){ // if the file's inputs are bad
        string runtimeMessage = "ERROR: INVALID FILE INPUT";
        runtimeMessage += "\nThe file had invalid inputs. Please make sure that the following are true:";
        runtimeMessage += "\n\t1. The first line is a single positive integer representing the length of the row";
        runtimeMessage += "\n\t2. The second line is a single positive integer representing the length of the column";
        runtimeMessage += "\n\t3. The grid has the same dimensions as stated in lines 1 and 2 and contains ONLY a \'X\' or a \'-\' for each character";
        throw runtime_error(runtimeMessage); // throws runtime error
    }
    bufferGrid = new Map(mGenOne->getNumRows() + 2, mGenOne->getNumColumns() + 2); // creates an empty buffered grid for using later
    mGenTwo = new Map(mGenOne->getNumRows(), mGenOne->getNumColumns()); // new map with same dimensions as generation 1 for generation 2
    mGenThree = new Map(mGenOne->getNumRows(), mGenOne->getNumColumns()); // new map with same dimensions as generation 1 for generation 3
    mGenFour = new Map(mGenOne->getNumRows(), mGenOne->getNumColumns()); // new map with same dimensions as generation 1 for generation 4
}

/**
 * Starts the simulation
 */
void GenericGame::play() {
    string options = "";
    while (!(options == "pauses" || options == "enters" || options == "file")) { // user input should be one of these options
        //prompt
        cout << "Would you like a brief pause between generations, press the enter key for each generation, or have the simulation output to a file?" << endl;
        cout << "If you want a brief pause between generations outputted on the console, please type \"pauses\" into the prompt and hit enter on your keyboard." << endl;
        cout << "If you want to just hit enter between generations outputted on the console, please type \"enters\" into the prompt and hit enter on your keyboard." << endl;
        cout << "If you want the simulation outputted in a file, please type \"file\" into the prompt and hit enter on your keyboard." << endl;
        cout << "---->\t";

        getline (cin, options);
        for (int i = 0; i < options.length(); ++i) { // makes the input case insensitive for checking
            options[i] = tolower(options[i]);
        }
        if (!(options == "pauses" || options == "enters" || options == "file")) { // if the input is not one of the options listed
            cout << "Sorry, that wasn't one of the options. Instructions will be repeated again." << endl;
        }
    }
    // runs the appropriate method based on what the user decides the output method to be
    if (options == "pauses") {
        consoleWithPause();
    } else if (options == "enters"){
        consoleWithEnter();
    } else if (options == "file"){
        fileOutput();
    }
}

/**
 * Checks if the map of cells are stable by checking if any of the last 4 generations of the map are equal to each other
 *
 * @return a boolean representing if the map is stable or not
 */
bool GenericGame::isStable() const{
    if(mGenOne != NULL && mGenTwo != NULL && mGenThree != NULL && mGenFour != NULL){ // makes sure that last four generations aren't null
        if (mGenOne->getMapString() == mGenFour->getMapString() ||
            mGenTwo->getMapString() == mGenFour->getMapString() ||
            mGenThree->getMapString() == mGenFour->getMapString()){ // compares last created map with the 3 previous maps to see if they're equal
            return true;
        }
    } else if (mGenTwo != NULL && mGenThree != NULL && mGenFour != NULL){ // makes sure that last three generations aren't null
        if (mGenTwo->getMapString() == mGenFour->getMapString() ||
            mGenThree->getMapString() == mGenFour->getMapString()){ // compares last created map with the 2 previous maps to see if they're equal
            return true;
        }
    } else if (mGenThree != NULL && mGenFour != NULL) { // makes sure that last two generations aren't null
        if (mGenThree->getMapString() == mGenFour->getMapString()){ // compares last created map with the previous map to see if they're equal
            return true;
        }
    }
    return false;
}

/**
 * Creates the next generation of the map from the old generation of the map
 *
 * @param a map representing the old generation
 * @param a map representing the new generation
 */
void GenericGame::createNextGen(Map*& oldGen, Map*& newGen) const{
    generateBufferedGrid(oldGen); // creates the buffered grid for next generation's calculations
    int currCellNeighbors = 0;
    for (int i = 0; i < newGen->getNumRows(); ++i) {
        for (int j = 0; j < newGen->getNumColumns(); ++j) {
            currCellNeighbors = countInternalNeighbors(i + 1, j + 1); // counts the neighbors by using the buffered grid
            if (currCellNeighbors == 0 || currCellNeighbors == 1) { // cell dying of loneliness in next generation
                newGen->updateGrid(i,j,'-');
            } else if (currCellNeighbors == 2) { // cell keeping its old state in next generation
                newGen->updateGrid(i,j,oldGen->getGridElement(i,j));
            } else if (currCellNeighbors == 3) { // cell being born in the next generation
                newGen->updateGrid(i,j,'X');
            } else if (currCellNeighbors >= 4 || currCellNeighbors <= 8) { // cell dying in the next generation due to overcrowding
                newGen->updateGrid(i,j,'-');
            } else { // somehow number of neighbors was miscalculated
                cout << "Something went wrong with calculating number of number of neighbors" << endl;
            }
        }
    }

}

/**
 * Helper function - Counts the number of neighbors for internal cells only
 *
 * @param an int representing the row number (0 indexed)
 * @param an int representing the column number (0 indexed)
 * @return an int representing the number of neighbors for the internal cell
 */
int GenericGame::countInternalNeighbors(int rowNum, int columnNum) const{
    int numNeighbors = 0;
    if (bufferGrid->getGridElement(rowNum - 1, columnNum - 1) == 'X') {++numNeighbors;} // top left neighbor
    if (bufferGrid->getGridElement(rowNum - 1, columnNum) == 'X') {++numNeighbors;} // neighbor directly above
    if (bufferGrid->getGridElement(rowNum - 1, columnNum + 1) == 'X') {++numNeighbors;} // top right neighbor
    if (bufferGrid->getGridElement(rowNum, columnNum - 1) == 'X') {++numNeighbors;} // neighbor directly left
    if (bufferGrid->getGridElement(rowNum, columnNum + 1) == 'X') {++numNeighbors;} // neighbor directly right
    if (bufferGrid->getGridElement(rowNum + 1, columnNum - 1) == 'X') {++numNeighbors;} // bottom left neighbor
    if (bufferGrid->getGridElement(rowNum + 1, columnNum) == 'X') {++numNeighbors;} // neighbor directly below
    if (bufferGrid->getGridElement(rowNum + 1, columnNum + 1) == 'X') {++numNeighbors;} // bottom right neighbor
    return numNeighbors;
}

/**
 * Generations will be printed in the terminal window with a delay of 0.1 seconds between each
 * generation being printed until it's stabilized.
 */
void GenericGame::consoleWithPause() {
    double numSeconds = 0.1; // a 0.1 second pause between each printing of generations
    int currGen = 4;
    Map m(mGenOne->getNumRows(), mGenOne->getNumColumns()); // map just for getting empty map string for comparison
    string emptyMap = m.getMapString();

    cout << "Generation 0" << endl; // printing gen 0
    mGenOne->print();
    usleep((long)(numSeconds * 1000000));

    cout << "\nGeneration 1" << endl; // printing gen 1
    mGenTwo->print();
    usleep((long)(numSeconds * 1000000));

    cout << "\nGeneration 2" << endl; // printing gen 2
    mGenThree->print();
    usleep((long)(numSeconds * 1000000));

    cout << "\nGeneration 3" << endl; // printing gen 3
    mGenFour->print();

    while(!isStable()){ // prints every consecutive generation
        usleep((long)(numSeconds * 1000000));
        Map* tempMap = mGenOne; // temp map pointer to delete the old mGenOne object later
        // shifting maps with reassignments
        mGenOne = mGenTwo;
        mGenTwo = mGenThree;
        mGenThree = mGenFour;
        mGenFour = new Map(mGenTwo->getNumRows(), mGenTwo->getNumColumns()); // new empty map with same dimensions
        createNextGen(mGenThree, mGenFour); // updates new empty map to create the next generation
        delete tempMap; // deletes that old mGenOne map
        cout << "\nGeneration " << currGen << endl;
        mGenFour->print();
        if(mGenFour->getMapString() == emptyMap){ // checks if the next generation became an empty map
            break;
        }
        ++currGen;
    }
    // asks user to press enter to exit the program
    cout << "\nThe grid of cells has stabilized. Please press enter on your keyboard to exit the program" << endl;
    cin.ignore();
}

/**
 * Generations will be printed in the terminal window and the user will need to hit enter to print the subsequent generations
 * until it's stabilized.
 */
void GenericGame::consoleWithEnter() {
    int currGen = 4;
    Map m(mGenOne->getNumRows(), mGenOne->getNumColumns()); // map just for getting empty map string for comparison
    string emptyMap = m.getMapString();

    cout << "Generation 0" << endl; // printing gen 0
    mGenOne->print();
    cin.ignore();

    cout << "Generation 1" << endl; // printing gen 1
    mGenTwo->print();
    cin.ignore();

    cout << "Generation 2" << endl; // printing gen 2
    mGenThree->print();
    cin.ignore();

    cout << "Generation 3" << endl; // printing gen 3
    mGenFour->print();

    while(!isStable()){ // prints every consecutive generation
        cin.ignore();
        Map* tempMap = mGenOne; // temp map pointer to delete the old mGenOne object later
        // shifting maps with reassignments
        mGenOne = mGenTwo;
        mGenTwo = mGenThree;
        mGenThree = mGenFour;
        mGenFour = new Map(mGenTwo->getNumRows(), mGenTwo->getNumColumns()); // new empty map with same dimensions
        createNextGen(mGenThree, mGenFour); // updates new empty map to create the next generation
        delete tempMap; // deletes that old mGenOne map
        cout << "Generation " << currGen << endl;
        mGenFour->print();
        if(mGenFour->getMapString() == emptyMap){ // checks if the next generation became an empty map
            break;
        }
        ++currGen;
    }
    // asks user to press enter to exit the program
    cout << "\nThe grid of cells has stabilized. Please press enter on your keyboard to exit the program" << endl;
    cin.ignore();
}

/**
 * The user will be prompted a file name to have the generations outputted into and the file will be created
 * with all the generations until it's stabilized.
 */
void GenericGame::fileOutput() {
    string fileName; // file name that the generations will be outputted to
    int currGen = 4;
    Map m(mGenOne->getNumRows(), mGenOne->getNumColumns()); // map just for getting empty map string for comparison
    string emptyMap = m.getMapString();

    cout << "What is the name of the file you would like your output to be in?" << endl; // prompt
    cout << "---->\t";
    getline(cin, fileName);

    ofstream fileWriter;
    fileWriter.open(fileName); // opening file writer

    fileWriter << "Generation 0" << endl; // printing gen 0
    fileWriter << mGenOne->getMapString() << endl;

    fileWriter << "\nGeneration 1" << endl; // printing gen 1
    fileWriter << mGenTwo->getMapString() << endl;

    fileWriter << "\nGeneration 2" << endl; // printing gen 2
    fileWriter << mGenThree->getMapString() << endl;

    fileWriter << "\nGeneration 3" << endl; // printing gen 3
    fileWriter << mGenFour->getMapString() << endl;

    while(!isStable()){ // prints every consecutive generation
        Map* tempMap = mGenOne; // temp map pointer to delete the old mGenOne object later
        // shifting maps with reassignments
        mGenOne = mGenTwo;
        mGenTwo = mGenThree;
        mGenThree = mGenFour;
        mGenFour = new Map(mGenTwo->getNumRows(), mGenTwo->getNumColumns()); // new empty map with same dimensions
        createNextGen(mGenThree, mGenFour); // updates new empty map to create the next generation
        delete tempMap; // deletes that old mGenOne map
        fileWriter << "\nGeneration " << currGen << endl;
        fileWriter << mGenFour->getMapString() << endl;
        if(mGenFour->getMapString() == emptyMap){ // checks if the next generation became an empty map
            break;
        }
        ++currGen;
    }
    // tells the user where to find the outputted file
    cout << "Successfully created file!" << endl;
    cout << "Please check the folder where the executable was located to find the text file: \"" << fileName << "\"" << endl;
    // asks user to press enter to exit the program
    cout << "\nThe grid of cells has stabilized. Please press enter on your keyboard to exit the program" << endl;
    cin.ignore();
}
