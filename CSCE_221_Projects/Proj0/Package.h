/*
 * Package.h
 *
 *  Created on: Jan 10, 2020
 *      Author: Prof. Lupoli
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <iostream>
#include <string>
using namespace std;

#include "Address.h"
#include "Dimension.h"

class Package {
public:
	Package();
	Package(Address &a, Dimension &d, string id, float weight);

	const Address& getAddress() const { return address; }
	void setAddress(const Address& address) { this->address = address; }

	const Dimension& getDimension() const { return dimension; }
	void setDimension(const Dimension& dimension) { this->dimension = dimension; }

	const string& getId() const { return ID; }
	void setId(const string& id) { ID = id; }

	float getWeight() const { return weight; }
	void setWeight(float weight) { this->weight = weight; }

	// overloaded cout
	friend std::ostream &operator<<(std::ostream &os, const Package &source);

protected:
	string ID;
	float weight;
	Dimension dimension;
	Address address;

};

inline Package::Package()
{
	ID = "";
	weight = 0.0;
}

inline Package::Package(Address &a, Dimension &d, string id, float weight)
{
	this->address = a;
	this->dimension = d;
	this->ID = id;
	this->weight = weight;
}

inline std::ostream &operator<<(std::ostream &os, const Package &source) {
	// prints all the aspects of the package to the output stream
	os << string(20, '-') << endl;
	os << "ID: " << source.ID << "\nWeight: " << source.weight << "\n\nDimensions: \n" << source.dimension << "\n" << source.address;
	os << string(20, '-') << endl;
	return os;
}

#endif /* PACKAGE_H_ */
