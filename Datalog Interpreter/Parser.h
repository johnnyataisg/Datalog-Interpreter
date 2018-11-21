#pragma once
#ifndef PARSER_H_
#define PARSER_H_
#include <set>
#include <list>
#include <stack>
#include <sstream>
#include <iostream>
#include "DatalogProgram.h"
#include "Token.h"

using namespace std;

//Define a set of symbols used to annotate the grammar productions
enum parseState {PS, PR, PQ, STRPARAM, IDPARAM, EXPPARAM};

class Parser
{
private:
	parseState state;
	parseState substate;
	vector<Token> tokenList;
	int index;
	//Array of token types to be referenced by TokenType values
	string myList[18] = { "COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON", "COLON_DASH"
						, "MULTIPLY", "ADD", "SCHEMES", "FACTS", "RULES", "QUERIES", "ID", "STRING", "COMMENT", "UNDEFINED", "EOF" };
	DatalogProgram datalog;
	//Temporary data members to help instantialize datalog
	string temp;
	Parameter* param;
	Predicate pred;
	string temp2;
	set<Parameter*> dlist;
	
public:
	Parser(list<Token> list);
	~Parser();
	void match(TokenType tt);
	bool parse();
	void parseDatalog();
	void parseScheme();
	void parseSchemeList();
	void parseIdList();
	void parseFact();
	void parseFactList();
	void parseRule();
	void parseRuleList();
	void parseHeadPredicate();
	void parsePredicate();
	void parsePredicateList();
	Parameter* parseParameter();
	void parseParameterList();
	Parameter* parseExpression();
	string parseOperator();
	void parseQuery();
	void parseQueryList();
	void parseStringList();
	DatalogProgram getDatalog();
};

#endif