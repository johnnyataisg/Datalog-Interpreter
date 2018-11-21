#pragma once
#ifndef STRING_H_
#define STRING_H_

using namespace std;

class String : public Parameter
{
private:
	string value;
	bool constant = true;
public:
	String() {};
	String(string str)
	{
		value = str;
	}
	virtual ~String() {};
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