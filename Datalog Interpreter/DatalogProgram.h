#pragma once
#ifndef DATALOG_PROGRAM_H_
#define DATALOG_PROGRAM_H_

#include <vector>
#include "Predicate.h"
#include "Rules.h"
using namespace std;

class DatalogProgram
{
private:
	vector<Predicate> schemeList;
	vector<Predicate> factList;
	vector<Rules> ruleList;
	vector<Predicate> queryList;
	set<string> domain;
public:
	//Default constructor
	DatalogProgram() {};

	//Destructor
	~DatalogProgram() 
	{ 
	}

	vector<Predicate> getSchemes()
	{
		return this->schemeList;
	}

	vector<Predicate> getFacts()
	{
		return this->factList;
	}

	vector<Rules> getRules()
	{
		return this->ruleList;
	}

	vector<Predicate> getQueries()
	{
		return this->queryList;
	}

	void pushSchemeList(Predicate pred)
	{
		schemeList.push_back(pred);
	}

	void addSchemeParam(Parameter* param)
	{
		schemeList.back().push(param);
	}

	void pushFactList(Predicate pred)
	{
		factList.push_back(pred);
	}

	void addFactParam(Parameter* param)
	{
		factList.back().push(param);
	}

	void pushRuleList(Rules rule)
	{
		ruleList.push_back(rule);
	}

	void addParamToHead(Parameter* param)
	{
		ruleList.back().addHeadParameter(param);
	}

	void addPredToRules(Predicate pred)
	{
		ruleList.back().push(pred);
	}

	void addParamToPredList(Parameter* param)
	{
		ruleList.back().addParameter(param);
	}

	void pushQueryList(Predicate pred)
	{
		queryList.push_back(pred);
	}

	void addQueryParam(Parameter* param)
	{
		queryList.back().push(param);
	}

	//Copies all ID's and strings within the list of fact predicates into a set which removes duplicates and sorts them and initializes domain to be equal to this set
	void createDomain()
	{
		for (size_t i = 0; i < factList.size(); i++)
		{
			for (size_t t = 0; t < factList.at(i).size(); t++)
			{
				domain.insert(factList.at(i).at(t)->toString());
			}
		}
	}

	//Returns all elements of domain
	string domainToString()
	{
		string output;
		createDomain();
		stringstream ss;
		ss << domain.size();
		output += "Domain(" + ss.str() + "):\n";
		for (set<string>::iterator iter = domain.begin(); iter != domain.end(); ++iter)
		{
			output += "  " + *iter + "\n";
		}
		return output;
	}

	string schemeToString()
	{
		string output;
		stringstream ss;
		ss << schemeList.size();
		output += "Schemes(" + ss.str() + "):\n";
		for (size_t i = 0; i < schemeList.size(); i++)
		{
			output += "  " + schemeList.at(i).toString() + "\n";
		}
		return output;
	}

	string factToString()
	{
		string output;
		stringstream ss;
		ss << factList.size();
		output += "Facts(" + ss.str() + "):\n";
		for (size_t i = 0; i < factList.size(); i++)
		{
			output += "  " + factList.at(i).toString() + "\n";
		}
		return output;
	}

	string ruleToString()
	{
		string output;
		stringstream ss;
		ss << ruleList.size();
		output += "Rules(" + ss.str() + "):\n";
		for (size_t i = 0; i < ruleList.size(); i++)
		{
			output += "  " + ruleList.at(i).toString() + ".\n";
		}
		return output;
	}

	string queryToString()
	{
		string output;
		stringstream ss;
		ss << queryList.size();
		output += "Queries(" + ss.str() + "):\n";
		for (size_t i = 0; i < queryList.size(); i++)
		{
			output += "  " + queryList.at(i).toString() + ".\n";
		}
		return output;
	}

	//Prints out the entire datalog program
	string toString()
	{
		string output;
		output += schemeToString() + factToString() + ruleToString() + queryToString() + domainToString();
		return output;
	}
};

#endif