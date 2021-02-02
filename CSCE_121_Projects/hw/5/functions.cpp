#include <iostream>
#include <fstream>
#include <cstring>
#include "functions.h"
#include "provided.h"

using namespace std;
using std::ifstream;

int readPrefs(char fileName[], int ngames, int prefs[]) {
    // check for valid file
    ifstream ifs {fileName};
    if(!ifs.is_open()) {
        return -1;
    }

    int count = 0;
    while (!ifs.eof()) {
        char ignore[MAX_TITLE_SIZE];
        // gets game and rating from file
        int game = 0, rating = 0;
        ifs >> game;
        ifs >> rating;
        // checks if either is valid int
        if (ifs.bad()) {
            return -1;
        } else if (ifs.fail()) {
            ifs.clear();
            ifs.getline(ignore,MAX_TITLE_SIZE);
        }
        else { // checks if the ints are in valid ranges
            if(game >= 0 && game < ngames && rating >=0 && rating <=5) {
                prefs[game] = rating;
                ++count;
            }
        }
        prefs[game] = rating;
        ++count;
    }

    return count;
}
int readPlan(char fileName[], int ngames, int plan[]) {
    // check for valid file
    ifstream ifs {fileName};
    if(!ifs.is_open()) {
        return -1;
    }
    // reads in data from the plan file into the plan array
    while (!ifs.eof()) {
        int day = 0, gameID = 0;
        ifs >> day;
        ifs >> gameID;
        plan[day] = gameID;
    }

    return 0;
}
int computeFunLevel(int start, int duration, int gamePrefs[], int ngames, int plan[]) {
    // check for duration that lasts longer than days remaining in year
    if(start + duration > 366) {
        return -1;
    }
    // sums up ratings of games included in plan
    int sum = 0;
    int count = 0;
    while(count < duration) {
        int gameID = plan[start + count];
        int gameRating = gamePrefs[gameID];
        sum += gameRating;
        ++count;
    }


    return sum;
}
int findBestVacation(int duration, int gamePrefs[], int ngames, int plan[]) {
    // check for a 0 day vacation
    if(duration == 0) {
        return 0;
    }
    
    // finds out the best start day for the most fun vacation
    int start = 1;
    for(int i = 2; i <= 366 - duration; ++i) {
        int tempFun = computeFunLevel(i,duration,gamePrefs,ngames,plan);
        int bestFun = computeFunLevel(start,duration,gamePrefs,ngames,plan);
        if(tempFun > bestFun) {
            start = i;
        }
    }

    return start;
}