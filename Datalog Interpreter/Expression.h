#ifndef EXPRESSION_H_
#define EXPRESSION_H_

using namespace std;

class Expression : public Parameter
{
private:
	Parameter* left;
	Parameter* right;
	string oper;
	bool constant = false;
public:
	Expression() {};
	virtual~Expression() {};
	void setLeft(Parameter* param)
	{
		left = param;
	}
	void setRight(Parameter* param)
	{
		right = param;
	}
	void setOper(string str)
	{
		oper = str;
	}
	string toString()
	{
		string output;
		output += "(" + this->left->toString() + this->oper + this->right->toString() + ")";
		return output;
	}
	bool isConstant()
	{
		return this->constant;
	}
};

#endif