/*
 * Liquid.h
 *
 *  Created on: Jan 9, 2020
 *      Author: Prof. Lupoli
 */

#ifndef LIQUID_H_
#define LIQUID_H_

#include <iostream>
#include <string>
using namespace std;

#include "Package.h"


class Liquid : public Package {
public:
	Liquid() : Package() {
		this->liquidType = "None";
		this->flammable = false;
	}

	// called a parameterized constructor
	Liquid(Address a, Dimension d, string id, float weight, string type, bool f)
		: Package(a, d, id, weight)
	{
		this->liquidType = type;
		this->flammable = f;
	}

	bool isFlammable() const { return flammable; }
	void setFlammable(bool flammable) { this->flammable = flammable; }

	const string& getLiquidType() const { return liquidType; }
	void setLiquidType(const string& liquidType) { this->liquidType = liquidType; }

	// data members
private:
	string liquidType;
	bool flammable;
};



#endif /* LIQUID_H_ */
