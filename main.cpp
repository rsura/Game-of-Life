/**
 * Main Implementation File: main.cpp
 *
 * Full Name: Rahul Sura
 * Chapman email: sura@chapman.edu
 * Course: CPSC 350-03
 * Project: Game of Life (Simulation of Cells)
 */

// includes each simulation mode's header file
#include "ClassicMode.h"
#include "DoughnutMode.h"
#include "MirrorMode.h"

using namespace std;

/**
 * This main method takes prompts the user through options for the Game of Life. It starts off asking if the user wants to
 * start off with a random or flat file configuration, further prompting the user accordingly. It then asks what simulation mode
 * the user wants to play (classic mode, doughnut mode, or mirror mode). It then asks if the user wants to output the generations
 * into a file or the terminal window. If it will be printed in the terminal window, then it will ask the user if they want a
 * brief pause between generations or if they want to hit "enter" on their keyboard every time for a new generation
 *
 * @param Command line parameters, separated by a space
 * @return Exit code
 */
int main(int argc, char** argv) {
    cout << "Hello! Welcome to the Game of Life!" << endl; // start of the prompt
    cout << "-----------------------------------" << endl;

    // initializing of variables
    string randOrFile = "";
    string fileName = "";
    string boundaryMode = "";
    int numRows = 0;
    int numColumns = 0;
    double popDensity = 2;

    // makes sure that the user enters the case-insensitive word "random" or "file"
    while (!(randOrFile == "random" || randOrFile == "file")) {
        cout << "Would you like to start with a random configuration or flat-file configuration?" << endl;
        cout << "If you want a random configuration, please type \"random\" into the prompt and hit enter on your keyboard." << endl;
        cout << "If you want a flat-file configuration, please type \"file\" into the prompt and hit enter on your keyboard." << endl;
        cout << "---->\t";
        getline (cin, randOrFile);
        for (int i = 0; i < randOrFile.length(); i++) { // makes the input to all lower case, so that it's case-insensitive
            randOrFile[i] = tolower(randOrFile[i]);
        }
        if (!(randOrFile == "random" || randOrFile == "file")) {
            cout << "Sorry, that wasn't one of the options. Instructions will be repeated again." << endl;
        }
    }

    // if the user said they wanted a random map:
    if (randOrFile == "random") {
        string temp;
        while (numRows < 1) { // makes sure that the user types a positive integer
            cout << "Input a number representing the number of rows in the grid (a positive integer greater than 0)" << endl; // prompt
            cout << "---->\t";
            getline (cin, temp);
            try{
                numRows = stoi(temp); // tries to convert the user input from string to an integer
            } catch (invalid_argument &exception) {
                cerr << "ERROR: Invalid positive integer value" << endl;
            }
            if (numRows < 1) { // repeats instructions when dimension is an invalid input
                cout << "Sorry, that wasn't a valid dimension. Instructions will be repeated again." << endl;
            }
        }
        while (numColumns < 1) { // makes sure that the user types a positive integer
            cout << "Input a number representing the number of columns in the grid (a positive integer greater than 0)" << endl; // prompt
            cout << "---->\t";
            getline (cin, temp);
            try{
                numColumns = stoi(temp); // tries to convert the user input from string to an integer
            } catch (invalid_argument &exception) {
                cerr << "ERROR: Invalid positive integer value" << endl;
            }
            if (numColumns < 1) { // repeats instructions when dimension is an invalid input
                cout << "Sorry, that wasn't a valid dimension. Instructions will be repeated again." << endl;
            }
        }
        while (popDensity < 0 || popDensity > 1) { // makes sure that the user types a double between 0 and 1
            cout << "Input a decimal number between 0 and 1 representing the population density of this grid" << endl; // prompt
            cout << "---->\t";
            getline (cin, temp);
            try{
                popDensity = stod(temp); // tries to convert the user input from string to a double
            } catch (invalid_argument &exception) {
                cerr << "ERROR: Invalid positive double value between 0 and 1" << endl;
            }
            if (popDensity < 0 || popDensity > 1) { // repeats instructions when dimension is an invalid input
                cout << "Sorry, that wasn't a valid population density. Instructions will be repeated again." << endl;
            }
        }
    } else if (randOrFile == "file") { // if the user said they wanted to provide a valid map file:
        while (fileName == "") { // the user should not just hit enter when providing the file's name
            cout << "Please input the name of the file (or the file path of the file, if it's in a different folder)" << endl;
            cout << "---->\t";
            getline(cin, fileName);
            if (fileName == "") {
                cout << "Sorry, you didn't enter a file name. Instructions will be repeated again." << endl;
            }
        }
    }

    // makes sure that the user enters the case-insensitive word "classic" or "doughnut" or "mirror"
    while (!(boundaryMode == "classic" || boundaryMode == "doughnut" || boundaryMode == "mirror")) {
        cout << "What boundary mode would you like?" << endl; // prompt
        cout << "If you want classic mode, please type \"classic\" into the prompt and hit enter on your keyboard." << endl;
        cout << "If you want doughnut mode, please type \"doughnut\" into the prompt and hit enter on your keyboard." << endl;
        cout << "If you want mirror mode, please type \"mirror\" into the prompt and hit enter on your keyboard." << endl;
        cout << "---->\t";
        getline (cin, boundaryMode);
        for (int i = 0; i < boundaryMode.length(); i++) { // makes the input to all lower case, so that it's case-insensitive
            boundaryMode[i] = tolower(boundaryMode[i]);
        }
        if (!(boundaryMode == "classic" || boundaryMode == "doughnut" || boundaryMode == "mirror")) { // repeats instructions if invalid input
            cout << "Sorry, that wasn't one of the options. Instructions will be repeated again." << endl;
        }
    }

    if (boundaryMode == "classic") { // if it's classic mode
        if (numRows > 0) { // classic mode with random map with number of rows, columns, and population density
            ClassicMode* cm = new ClassicMode(numRows, numColumns, popDensity);
            cm->play();
            delete cm;
        } else if (fileName != ""){ // classic mode with a map provided in a flat file configuration
            try { // try catch block is if the file has invalid inputs for some reason
                ClassicMode* cm = new ClassicMode(fileName);
                cm->play();
                delete cm;
            } catch (runtime_error &exception){
                cerr << exception.what() << endl;
            }
        } else { // if somehow the user bypassed the prompts, then it creates a default classic mode simulation
            ClassicMode* cm = new ClassicMode();
            cm->play();
            delete cm;
        }
    } else if (boundaryMode == "doughnut") { // if it's doughnut mode
        if (numRows > 0) { // doughnut mode with random map with number of rows, columns, and population density
            DoughnutMode* dm = new DoughnutMode(numRows, numColumns, popDensity);
            dm->play();
            delete dm;
        } else if (fileName != ""){ // doughnut mode with a map provided in a flat file configuration
            try { // try catch block is if the file has invalid inputs for some reason
                DoughnutMode* dm = new DoughnutMode(fileName);
                dm->play();
                delete dm;
            } catch (runtime_error &exception){
                cerr << exception.what() << endl;
            }
        } else { // if somehow the user bypassed the prompts, then it creates a default doughnut mode simulation
            DoughnutMode* dm = new DoughnutMode();
            dm->play();
            delete dm;
        }
    } else if (boundaryMode == "mirror") { // if it's mirror mode
        if (numRows > 0) { // mirror mode with random map with number of rows, columns, and population density
            MirrorMode* mm = new MirrorMode(numRows, numColumns, popDensity);
            mm->play();
            delete mm;
        } else if (fileName != ""){ // mirror mode with a map provided in a flat file configuration
            try { // try catch block is if the file has invalid inputs for some reason
                MirrorMode* mm = new MirrorMode(fileName);
                mm->play();
                delete mm;
            } catch (runtime_error &exception){
                cerr << exception.what() << endl;
            }
        } else { // if somehow the user bypassed the prompts, then it creates a default doughnut mode simulation
            MirrorMode* mm = new MirrorMode();
            mm->play();
            delete mm;
        }
    }

    return 0;
}
