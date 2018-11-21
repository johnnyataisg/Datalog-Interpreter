#pragma once
#ifndef PREDICATE_H_
#define PREDICATE_H_
#include <vector>
#include "Id.h"
#include "String.h"
#include "Expression.h"

using namespace std;

class Predicate
{
private:
	string id;
	vector<Parameter*> parameterList;
public:
	//Default constructor
	Predicate() {};

	//Destructor
	~Predicate() {};

	Predicate(string id1)
	{
		id = id1;
	}

	string getID()
	{
		return this->id;
	}

	//Push a parameter pointer onto this vector
	void push(Parameter* param)
	{
		parameterList.push_back(param);
	}

	//Clear all memory of parameter pointers contained by this vector
	void clear()
	{
		parameterList.clear();
		id = "";
	}

	vector<Parameter*> getParamList()
	{
		return this->parameterList;
	}

	//Returns the size of the vector
	size_t size()
	{
		return parameterList.size();
	}

	//Returns the parameter pointer at a given index of the vector
	Parameter*& at(size_t index)
	{
		return this->parameterList.at(index);
	}

	//Setter for the ID
	void setID(string ID)
	{
		id = ID;
	}
	
	//Prints out all members in the vector
	string toString()
	{
		string output;
		output += id +'(';
		for (size_t i = 0; i < parameterList.size(); i++)
		{
			output += parameterList.at(i)->toString();
			if (i < parameterList.size() - 1)
			{
				output += ',';
			}
			else
			{
				output += ')';
			}
		}
		return output;
	}
};

#endif