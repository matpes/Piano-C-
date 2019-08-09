#pragma once
#include <algorithm>
#include "Formatter.h"
class MXMLFormatter :public Formatter
{
private:
public:
	MXMLFormatter();
	void createFile(string x);
	virtual ~MXMLFormatter();
};

