#pragma once
#ifndef RELATION_H_
#define RELATION_H_
#include "Tuple.h"
#include "Header.h"
#include <map>

using namespace std;

class Relation
{
private:
	string name;
	Header header;
	set<Tuple> tuples;
public:
	Relation() {};

	Relation(string str, Header hdr)
	{
		name = str;
		header = hdr;
	}

	Relation(Predicate pred, vector<Predicate> facts)
	{
		Header hdr(pred.getParamList());
		this->name = pred.getID();
		this->header = hdr;
		for (Predicate pred : facts)
		{
			if (pred.getID() == this->name)
			{
				Tuple tuple(pred.getParamList());
				this->addTuple(tuple);
			}
		}
	}

	~Relation() {};
	
	void addTuple(Tuple tpl)
	{
		tuples.insert(tpl);
	}

	void deleteTuple(Tuple tpl)
	{
		tuples.erase(tpl);
	}

	string getName()
	{
		return this->name;
	}

	void setName(string newName)
	{
		this->name = newName;
	}

	void select(int col, string val)
	{
		for (Tuple tpl : getTuples())
		{
			if (tpl.at(col) != val)
			{
				this->deleteTuple(tpl);
			}
		}
	}

	void rename(vector<string> vec)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			this->header.at(i) = vec.at(i);
		}
	}

	void project(vector<int> indexes)
	{
		
		Header newHeader;
		set<Tuple> newTuples;
		for (int index : indexes)
		{
			newHeader.push_back(this->header.at(index));
		}
		for (Tuple tpl : this->tuples)
		{
			Tuple tuple;
			for (int index : indexes)
			{
				tuple.push_back(tpl.at(index));
			}
			newTuples.insert(tuple);
		}
		this->header = newHeader;
		this->tuples = newTuples;
	}

	Relation unionize(Relation relation)
	{
		Relation unionRelation = *this;
		if (this->getHeader() == relation.getHeader())
		{
			for (Tuple tuple : relation.getTuples())
			{
				if (unionRelation.tuples.count(tuple) == 0)
				{
					unionRelation.tuples.insert(tuple);
				}
			}
		}
		return unionRelation;
	}

	Relation join(Relation relation)
	{
		vector<pair<int, int>> matches = this->findMatchingColumns(relation);
		Relation JoinedRelation("Result", combineSchemes(relation.getHeader()));
		for (Tuple tuple1 : this->getTuples())
		{
			for (Tuple tuple2 : relation.getTuples())
			{
				if (isJoinable(tuple1, tuple2, matches))
				{
					JoinedRelation.addTuple(combineTuples(tuple1, tuple2, matches));
				}
			}
		}
		return JoinedRelation;
	}

	vector<pair<int, int>> findMatchingColumns(Relation relation)
	{
		vector<pair<int, int>> matchVector;
		for (size_t i = 0; i < this->getHeader().size(); i++)
		{
			for (size_t t = 0; t < relation.getHeader().size(); t++)
			{
				if (this->getHeader().at(i) == relation.getHeader().at(t))
				{
					matchVector.push_back(make_pair(i, t));
					break;
				}
			}
		}
		return matchVector;
	}

	Header combineSchemes(Header header)
	{
		Header CombinedScheme;
		set<string> temp;
		for (string column : this->getHeader())
		{
			CombinedScheme.push_back(column);
			temp.insert(column);
		}
		for (size_t i = 0; i < header.size(); i++)
		{
			if (temp.count(header.at(i)) == 0)
			{
				CombinedScheme.push_back(header.at(i));
			}
		}
		return CombinedScheme;
	}

	bool isJoinable(Tuple tup1, Tuple tup2, vector<pair<int, int>> matches)
	{
		for (pair<int, int> pair : matches)
		{
			if (tup1.at(pair.first) != tup2.at(pair.second))
			{
				return false;
			}
		}
		return true;
	}

	Tuple combineTuples(Tuple tup1, Tuple tup2, vector<pair<int, int>> matches)
	{
		Tuple tuple;
		set<int> temp;
		for (pair<int, int> pair : matches)
		{
			temp.insert(pair.second);
		}
		for (string element : tup1)
		{
			tuple.push_back(element);
		}
		for (size_t i = 0; i < tup2.size(); i++)
		{
			if (temp.count(i) == 0)
			{
				tuple.push_back(tup2.at(i));
			}
		}
		return tuple;
	}

	Header getHeader()
	{
		return this->header;
	}

	void setHeader(Header newHeader)
	{
		this->header = newHeader;
	}

	set<Tuple> getTuples()
	{
		return this->tuples;
	}

	void setTuples(set<Tuple> newTuples)
	{
		this->tuples = newTuples;
	}

	string toString()
	{
		string output;
		for (Tuple tup : tuples)
		{
			for (size_t i = 0; i < header.size(); i++)
			{
				if (i == 0)
				{
					output += "  ";
				}
				output += header.at(i) + "=" + tup.at(i);
				if (i != header.size() - 1)
				{
					output += ", ";
				}
				else
				{
					output += "\n";
				}
			}
		}
		return output;
	}
};

#endif