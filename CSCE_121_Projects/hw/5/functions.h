#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
using std::ifstream;

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int readPrefs(char fileName[], int ngames, int prefs[]);
int readPlan(char fileName[], int ngames, int plan[]);
int computeFunLevel(int start, int duration, int gamePrefs[], int ngames, int plan[]);
int findBestVacation(int duration,  int gamePrefs[], int ngames, int plan[]
);

#endif