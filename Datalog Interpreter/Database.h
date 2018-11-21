#pragma once
#ifndef DATABASE_H_
#define DATABASE_H_
#include "Relation.h"
#include <map>

using namespace std;

class Database : public map<string, Relation>
{
private:
public:
	Database() {};

	Database(DatalogProgram program)
	{
		vector<Predicate> schemes = program.getSchemes();
		vector<Predicate> facts = program.getFacts();
		for (Predicate pred : schemes)
		{
			Relation relation(pred, facts);
			this->insert(pair<string, Relation>(relation.getName(), relation));
		}
	}

	~Database() {};

	string toString()
	{
		string output;
		for (pair<string, Relation> pair : *this)
		{
			output += pair.first + "\n" + pair.second.toString();
		}
		return output;
	}
};

#endif