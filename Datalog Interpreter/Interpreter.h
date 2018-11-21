#pragma once
#ifndef INTERPRETER_H_
#define INTERPRETER_H_
#include "Database.h"

using namespace std;

class Interpreter
{
private:
	DatalogProgram datalog;
	Database database;
public:
	Interpreter() {};

	Interpreter(DatalogProgram datalogProgram)
	{
		this->datalog = datalogProgram;
		this->database = Database(datalogProgram);
	}

	~Interpreter() {};

	Database getDatabase()
	{
		return this->database;
	}

	Relation runQuery(int index)
	{
		Predicate pred = datalog.getQueries().at(index);
		Relation relation = database[pred.getID()];
		vector<int> projectIndexes;
		vector<string> rename;
		map<string, int> rpMap;
		for (size_t i = 0; i < pred.getParamList().size(); i++)
		{
			string value = pred.getParamList().at(i)->toString();
			if (pred.getParamList().at(i)->isConstant())
			{
				relation.select(i, value);
			}
			else if (rpMap.find(value) == rpMap.end())
			{
				projectIndexes.push_back(i);
				rename.push_back(value);
				rpMap.insert(pair<string, int>(value, i));
			}
			else
			{
				int index = rpMap[value];
				for (Tuple tpl : relation.getTuples())
				{
					string value2 = tpl.at(index);
					if (tpl.at(i) != value2)
					{
						relation.deleteTuple(tpl);
					}
				}
			}
		}
		relation.project(projectIndexes);
		relation.rename(rename);
		return relation;
	}

	vector<pair<string, Relation>> runQueries()
	{
		vector<pair<string, Relation>> db;
		for (size_t i = 0; i < datalog.getQueries().size(); i++)
		{
			Relation relation = runQuery(i);
			int size = relation.getTuples().size();
			if (size == 0)
			{
				db.push_back(pair<string, Relation>(datalog.getQueries().at(i).toString() + "? No", relation));
			}
			else
			{
				stringstream ss;
				ss << size;
				db.push_back(pair<string, Relation>(datalog.getQueries().at(i).toString() + "? Yes(" + ss.str() + ")", relation));
			}
		}
		return db;
	}

	string printQueryResults()
	{
		string output;
		for (pair<string, Relation> pair : runQueries())
		{
			output += pair.first + "\n" + pair.second.toString();
		}
		return output;
	}


};

#endif