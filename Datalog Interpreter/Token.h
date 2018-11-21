#pragma once
#ifndef TOKEN_H_
#define TOKEN_H_
#include <string>

using namespace std;

//Define types of different tokens
enum TokenType { COM, PER, QM, LPAR, RPAR, COL, COLD, MUL, ADD, SCH, FAC, RUL, QUE, ID, STR, CMT, NA, EF };

class Token
{
private:
	string value;
	TokenType type;
	int line_num;
public:
	Token() {};
	Token(string input1, TokenType input2, int input3)
	{
		value = input1;
		type = input2;
		line_num = input3;
	}

	string getValue()
	{
		return value;
	}

	TokenType getType()
	{
		return type;
	}

	int getLineNum()
	{
		return line_num;
	}
};

#endif