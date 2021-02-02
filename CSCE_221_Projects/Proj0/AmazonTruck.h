/*
 * AmazonTruck.h
 *
 *  Created on: Jan 9, 2020
 *      Author: Prof. Lupoli
 */

#ifndef AMAZONTRUCK_H_
#define AMAZONTRUCK_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <ostream>
#include <sstream>
// stof, stoi, etc. -- string conversion
#include <iomanip>
using namespace std;

#include "Liquid.h"
#include "Package.h"

class AmazonTruck
{
public:
    AmazonTruck(short v) { volume = v; }
    ~AmazonTruck() { delete truck; }


    bool isFull()
    {
        // calculate volume remaining in the truck
        float volumeRemaining = this->volume - totalVolumeSoFar();
        // return if the truck is close to or completely full
        if (volumeRemaining <= 0)
            return true;
        return false;
    }
    void makeEmpty() {
        // Clears truck with a built-in vector function
        this->truck->clear();
    }
    float totalVolumeSoFar()
    {
        // Sums up the total volume contained in the truck currently
        // Overflow will be prevented in fillFromFile.
        short totalVolume = 0.0;
        for(unsigned int i = 0; i < this->truck->size(); ++i) {
            totalVolume += this->truck->at(i).getDimension().getWidth() * this->truck->at(i).getDimension().getHeight() * this->truck->at(i).getDimension().getDepth();
        }
        return totalVolume;
    }

    /*
	 * 1. How will you parse the address from the data file to match address, town, etc...?
	 */
    void fillFromFile(string filename);
    void displayLastItemInTruck() {
        cout << this->truck->at(this->truck->size() - 1);
    }

private:
    /*
	 * No other data members are allowed!!!
	 */
    vector<Package> *truck = new vector<Package>;
    short volume;
    /*
	 * No other data members are allowed!!!
	 */
};

inline void AmazonTruck::fillFromFile(string filename)
{
    // check for valid file
    ifstream ifs{filename};

    //Initialize variables for objects
    string data[11]; // stores values as strings so that some may be casted later
    string a1; // data[0]
    string city; // data[1]
    string state; // data[2]
    int zip; // data[3]
    short int width; // data[4]
    short int height; // data[5]
    short int depth; // data[6]
    string ID; // data[7]
    float weight; // data[8]
    string liquidType; // data[9]
    bool flammable; // data[10]
    string line; // data for line by line iteration through csv

    while (getline(ifs, line))
    {
        if(this->isFull())
            break;
        // Put values from file into variables
        istringstream ss(line);
        getline(ss, data[0], ',');
        a1 = data[0];
        getline(ss, data[1], ',');
        city = data[1];
        getline(ss, data[2], ',');
        state = data[2];
        getline(ss, data[3], ',');
        zip = stoi(data[3]);
        getline(ss, data[4], ',');
        width = stoi(data[4]);
        getline(ss, data[5], ',');
        height = stoi(data[5]);
        getline(ss, data[6], ',');
        depth = stoi(data[6]);
        // Check for disgruntled workers
        if (width <= 0 || height <= 0 || depth <= 0)
            continue;
        getline(ss, data[7], ',');
        ID = data[7];
        getline(ss, data[8], ',');
        weight = stof(data[8]);
        getline(ss, data[9], ',');
        liquidType = data[9];
        getline(ss, data[10], ',');
        flammable = stoi(data[10]);
        
        //Object Creation
        Address a = Address(a1, city, state, zip);
        Dimension d = Dimension(width, height, depth);

        // cout << string(20, '-') << endl;

        Liquid l = Liquid(a, d, ID, weight, liquidType, flammable);
        // breaks out if truck is full or close enough to full that the next package/liquid doesn't fit
        if((l.getDimension().getWidth()*l.getDimension().getHeight()*l.getDimension().getDepth()) > (this->volume - this->totalVolumeSoFar()))
            break;
        //Load package onto truck
        this->truck->push_back(l);
    }
}

#endif /* AMAZONTRUCK_H_ */
