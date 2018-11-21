#pragma once
#ifndef PARAMETER_H_
#define PARAMETER_H_

using namespace std;

class Parameter
{
private:
public:
	Parameter() {};
	virtual ~Parameter() {};
	virtual string toString() = 0;
	virtual bool isConstant() = 0;
};

#endif