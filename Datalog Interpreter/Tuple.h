#pragma once
#ifndef TUPLE_H_
#define TUPLE_H_


using namespace std;

class Tuple : public vector<string>
{
private:
public:
	Tuple() {};

	Tuple(vector<Parameter*> vec)
	{
		for (Parameter* param : vec)
		{
			this->push_back(param->toString());
		}
	}

	~Tuple() {};

};

#endif