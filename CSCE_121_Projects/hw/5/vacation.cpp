#include <iostream>
#include <fstream>
#include <cstring>
#include "provided.h"
#include "functions.h"

using namespace std;
using std::ifstream;

int main() {
    int ngames = 0, duration = 0;
    char titleFile[MAX_TITLE_SIZE], prefFile[MAX_TITLE_SIZE], planFile[MAX_TITLE_SIZE], gameTitles[MAX_NB_GAMES][MAX_TITLE_SIZE];
    int prefs[MAX_NB_GAMES], plan[366];

    cout << "Please enter ngames and duration: ";
    cin >> ngames >> duration;
    // check for valid number of games and vacation duration
    if(ngames < 0 || ngames > 200 || duration < 0 || duration > 365) {
        cout << "Invalid input." << endl;
        return 1;
    }
    // get file names
    cout << "Please enter name of file with titles: ";
    cin >> titleFile;
    cout << "Please enter name of file with preferences: ";
    cin >> prefFile;
    cout << "Please enter name of file with plan: ";
    cin >> planFile;


    int gameTitlesReturn = readGameTitles(titleFile, ngames, gameTitles);
    int prefsReturn = readPrefs(prefFile, ngames, prefs);
    int planReturn = readPlan(planFile, ngames, plan);
    // Check that files open properly
    if(gameTitlesReturn == -1 || prefsReturn == -1 || planReturn == -1) {
        cout << "Invalid file." << endl;
        return 1;
    }
    
    // Output results from files
    int dayToStart = findBestVacation(duration, prefs, ngames, plan);
    cout << "Best start day is " << dayToStart << endl;
    cout << "Games to be played: " << endl;
    printGamesPlayedInVacation(dayToStart, duration, plan, gameTitles, ngames);

    return 0;
}