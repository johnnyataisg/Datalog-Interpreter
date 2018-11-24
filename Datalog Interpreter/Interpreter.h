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

	Relation runQuery(Predicate pred)
	{
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
			Relation relation = runQuery(datalog.getQueries().at(i));
			if (relation.getTuples().size() == 0)
			{
				db.push_back(pair<string, Relation>(datalog.getQueries().at(i).toString() + "? No", relation));
			}
			else
			{
				stringstream ss;
				ss << relation.getTuples().size();
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

	Relation evaluateRule(int index)
	{
		Rules rule = this->datalog.getRules().at(index);
		vector<Relation> relations;
		for (Predicate pred : rule.getPredicates())
		{
			relations.push_back(runQuery(pred));
		}
		Relation relation = relations.at(0);
		for (size_t i = 1; i < relations.size(); i++)
		{
			relation = relation.join(relations.at(i));
		}
		vector<int> projectIndexes;
		for (Parameter* param : rule.getHead().getParamList())
		{
			for (size_t i = 0; i < relation.getHeader().size(); i++)
			{
				if (relation.getHeader().at(i) == param->toString())
				{
					projectIndexes.push_back(i);
				}
			}
		}
		relation.project(projectIndexes);
		relation.setHeader(this->database[rule.getHead().getID()].getHeader());
		relation = this->database[rule.getHead().getID()].unionize(relation);
		return relation;
	}

	void populateRules()
	{
		for (size_t i = 0; i < this->datalog.getRules().size(); i++)
		{
			Relation afterRuleEvaluation = evaluateRule(i);
			this->database[afterRuleEvaluation.getName()] = afterRuleEvaluation;
		}
	}

	string rulePopulateResults()
	{
		int i = 0;
		int preCount;
		int postCount;
		do
		{
			preCount = countTuples();
			populateRules();
			i++;
			postCount = countTuples();
		} while (preCount != postCount);
		stringstream ss;
		ss << i;
		string output = "Schemes populated after " + ss.str() + " passes through the Rules.";
		return output;
	}

	int countTuples()
	{
		int tupleCount = 0;
		for (pair<string, Relation> pair : this->database)
		{
			tupleCount += pair.second.getTuples().size();
		}
		return tupleCount;
	}

	DatalogProgram getDatalog()
	{
		return this->datalog;
	}
};

#endif