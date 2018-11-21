#pragma once
#ifndef RULES_H_
#define RULES_H_

using namespace std;

class Rules
{
private:
	Predicate headPred;
	vector<Predicate> predicates;
public:
	//Default constructor
	Rules() {};

	//Constructor that sets a head predicate
	Rules(Predicate head)
	{
		headPred = head;
	}

	//Destructor
	~Rules() {};

	void setHeadID(string id1)
	{
		headPred.setID(id1);
	}

	//Getter for headPred
	Predicate getHead()
	{
		return this->headPred;
	}

	//Getter for predicates
	vector<Predicate> getPredicates()
	{
		return this->predicates;
	}

	//Setter for headPred
	void setHead(Predicate pred)
	{
		headPred = pred;
	}

	//Pushes a predicate onto the vector
	void push(Predicate pred)
	{
		predicates.push_back(pred);
	}

	void addParameter(Parameter* param)
	{
		predicates.back().push(param);
	}

	void addHeadParameter(Parameter* param)
	{
		headPred.push(param);
	}

	//Prints out each head predicate along with its associated tail predicates
	string toString()
	{
		string output;
		output += headPred.toString() + " :- ";
		for (size_t i = 0; i < predicates.size(); i++)
		{
			if (i != predicates.size() - 1)
			{
				output += predicates.at(i).toString() + ",";
			}
			else
			{
				output += predicates.at(i).toString();
			}
		}
		return output;
	}
};

#endif