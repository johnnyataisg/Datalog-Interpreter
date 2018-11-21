#pragma once
#ifndef ID_H_
#define ID_H_
#include "Parameter.h"

using namespace std;

class Id : public Parameter
{
private:
	string value;
	bool constant = false;
public:
	Id() {};
	Id(string str)
	{
		value = str;
	}
	virtual ~Id() {};
	string toString()
	{
		return this->value;
	}
	bool isConstant()
	{
		return this->constant;
	}
};

#endif