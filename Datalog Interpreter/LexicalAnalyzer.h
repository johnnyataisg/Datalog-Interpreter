#pragma once
#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_
#include <string>
#include "InputStream.h"
#include "Token.h"
#include <sstream>
#include <list>

enum State { START, END, PEND, COLOND, KEYID, STRING, STRPEND, NBCOM, NCOM, BCOM, BCOMPEND, MYEOF };


using namespace std;

class LexicalAnalyzer
{
private:
	list<Token> tokenList;
	InputStream* istream;
	State state;
	string myList[18] = { "COMMA", "PERIOD", "Q_MARK", "LEFT_PAREN", "RIGHT_PAREN", "COLON", "COLON_DASH"
						, "MULTIPLY", "ADD", "SCHEMES", "FACTS", "RULES", "QUERIES", "ID", "STRING", "COMMENT", "UNDEFINED", "EOF" };
public:
	LexicalAnalyzer(string input);
	~LexicalAnalyzer();
	bool generateList();
	bool isComma(char& c, string& temp, int& lineNumber);
	bool isPeriod(char& c, string& temp, int& lineNumber);
	bool isQMark(char& c, string& temp, int& lineNumber);
	bool isLeftPar(char& c, string& temp, int& lineNumber);
	bool isRightPar(char& c, string& temp, int& lineNumber);
	bool isColon(char& c, string& temp, int& lineNumber);
	bool isMultiply(char& c, string& temp, int& lineNumber);
	bool isAdd(char& c, string& temp, int& lineNumber);
	bool isIdKeyword(char& c, string& temp, int& lineNumber);
	bool isKeyword(char& c, string& temp, int& lineNumber);
	bool isString(char& c, string& temp, int& lineNumber);
	bool isWhitespace(char& c, string& temp, int& lineNumber);
	bool isComment(char& c, string& temp, int& lineNumber);
	bool isLineBlock(char& c, string& temp, int& lineNumber);
	bool isBlock(char& c, string& temp, int& lineNumber);
	bool isEOF(char& c, string& temp, int& lineNumber);
	bool isUndefined(char& c, string& temp, int& lineNumber);
	list<Token> getList();
	InputStream* getStream();
	State getState();
	string print();
	Token getNextToken(TokenType tt)
	{
		Token result = tokenList.front();
		if (result.getType() == EF || result.getType() != tt) 
		{
			throw result;
		}
		tokenList.pop_front();
		return result;
	}
};

#endif