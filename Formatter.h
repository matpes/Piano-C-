#pragma once
#include <iostream>
#include <fstream>
#include "Composition.h"
using namespace std;
class Formatter
{
protected:
	Composition* compozicija;
	ofstream file;
public:
	Formatter() {}
	virtual void addComp(Composition * comp) {
		compozicija  = comp;
	}
	//ofstream * getFile() { return &file; }
	virtual void createFile(string x) = 0;
	virtual ~Formatter();
};
