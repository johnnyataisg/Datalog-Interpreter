#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(string input)
{
	istream = new InputStream(input);
}

LexicalAnalyzer::~LexicalAnalyzer()
{
	delete istream;
	tokenList.clear();
}

bool LexicalAnalyzer::generateList()
{
	char c;
	int lineNumber;
	string temp;
	state = START;
	while (state != MYEOF)
	{
		state = START;
		temp = "";
		c = istream->currentChar();
		lineNumber = istream->getLineNumber();
		isComma(c, temp, lineNumber);
	}
	return true;
}

bool LexicalAnalyzer::isComma(char& c, string& temp, int& lineNumber)
{
	if (c == ',')
	{
		state = END;
		temp += c;
		Token token(temp, COM, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isPeriod(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isPeriod(char& c, string& temp, int& lineNumber)
{
	if (c == '.')
	{
		state = END;
		temp += c;
		Token token(temp, PER, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isQMark(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isQMark(char& c, string& temp, int& lineNumber)
{
	if (c == '?')
	{
		state = END;
		temp += c;
		Token token(temp, QM, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isLeftPar(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isLeftPar(char& c, string& temp, int& lineNumber)
{
	if (c == '(')
	{
		state = END;
		temp += c;
		Token token(temp, LPAR, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isRightPar(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isRightPar(char& c, string& temp, int& lineNumber)
{
	if (c == ')')
	{
		state = END;
		temp += c;
		Token token(temp, RPAR, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isColon(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isColon(char&c, string& temp, int& lineNumber)
{
	if (c == ':')
	{
		state = COLOND;
		temp += c;
		istream->advance();
		c = istream->currentChar();
		lineNumber = istream->getLineNumber();
		if (c == '-')
		{
			state = END;
			temp += c;
			Token token(temp, COLD, lineNumber);
			tokenList.push_back(token);
			istream->advance();
		}
		else
		{
			state = END;
			Token token(temp, COL, lineNumber);
			tokenList.push_back(token);
		}
		return true;
	}
	else
	{
		return isMultiply(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isMultiply(char& c, string& temp, int& lineNumber)
{
	if (c == '*')
	{
		state = END;
		temp += c;
		Token token(temp, MUL, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isAdd(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isAdd(char& c, string& temp, int& lineNumber)
{
	if (c == '+')
	{
		state = END;
		temp += c;
		Token token(temp, ADD, lineNumber);
		tokenList.push_back(token);
		istream->advance();
		return true;
	}
	else
	{
		return isIdKeyword(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isIdKeyword(char& c, string& temp, int& lineNumber)
{
	if (isalpha(c))
	{
		state = KEYID;
		temp += c;
		istream->advance();
		while (state != END)
		{
			c = istream->currentChar();
			lineNumber = istream->getLineNumber();
			if (!isalpha(c) && !isdigit(c))
			{
				isKeyword(c, temp, lineNumber);
			}
			else
			{
				state = KEYID;
				temp += c;
				istream->advance();
			}
		}
		return true;
	}
	else
	{
		return isString(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isKeyword(char& c, string& temp, int& lineNumber)
{
	state = END;
	if (temp == "Schemes")
	{
		Token token(temp, SCH, lineNumber);
		tokenList.push_back(token);
	}
	else if (temp == "Facts")
	{
		Token token(temp, FAC, lineNumber);
		tokenList.push_back(token);
	}
	else if (temp == "Rules")
	{
		Token token(temp, RUL, lineNumber);
		tokenList.push_back(token);
	}
	else if (temp == "Queries")
	{
		Token token(temp, QUE, lineNumber);
		tokenList.push_back(token);
	}
	else
	{
		Token token(temp, ID, lineNumber);
		tokenList.push_back(token);
	}
	return true;
}

bool LexicalAnalyzer::isString(char& c, string& temp, int& lineNumber)
{
	if (c == '\'')
	{
		state = STRING;
		temp += c;
		lineNumber = istream->getLineNumber();
		istream->advance();
		while (state != END)
		{
			c = istream->currentChar();
			if (c == -1)
			{
				state = END;
				Token token(temp, NA, lineNumber);
				tokenList.push_back(token);
			}
			else if (c == '\'')
			{
				state = STRPEND;
				temp += c;
				istream->advance();
				c = istream->currentChar();
				if (c == '\'')
				{
					state = STRING;
					temp += c;
					istream->advance();
				}
				else
				{
					state = END;
					Token token(temp, STR, lineNumber);
					tokenList.push_back(token);
				}
			}
			else
			{
				state = STRING;
				temp += c;
				istream->advance();
			}
		}
		return true;
	}
	else
	{
		return isWhitespace(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isWhitespace(char& c, string& temp, int& lineNumber)
{
	if (isspace(c))
	{
		state = END;
		istream->advance();
		return true;
	}
	else
	{
		return isComment(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isComment(char& c, string& temp, int& lineNumber)
{
	if (c == '#')
	{
		state = NBCOM;
		lineNumber = istream->getLineNumber();
		temp += c;
		istream->advance();
		c = istream->currentChar();
		return isLineBlock(c, temp, lineNumber);
	}
	else
	{
		return isEOF(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isLineBlock(char& c, string& temp, int& lineNumber)
{
	if (c == '|')
	{
		temp += c;
		state = BCOM;
		lineNumber = istream->getLineNumber();
		istream->advance();
		c = istream->currentChar();
		while (state != END)
		{
			c = istream->currentChar();
			isBlock(c, temp, lineNumber);
		}
		return true;
	}
	else
	{
		state = NCOM;
		while (c != '\n')
		{
			temp += c;
			istream->advance();
			c = istream->currentChar();
			lineNumber = istream->getLineNumber();
		}
		state = END;
		//Token token(temp, CMT, lineNumber);
		//tokenList.push_back(token);
		istream->advance();
		return true;
	}
}

bool LexicalAnalyzer::isBlock(char& c, string& temp, int& lineNumber)
{
	if (c == '|')
	{
		state = BCOMPEND;
		char test = c;
		istream->advance();
		c = istream->currentChar();
		if (c == '#')
		{
			state = END;
			temp = temp + test + c;
			//Token token(temp, CMT, lineNumber);
			//tokenList.push_back(token);
			istream->advance();
		}
		else if (c == -1)
		{
			state = END;
			temp += test;
			//Token token(temp, NA, lineNumber);
			//tokenList.push_back(token);
		}
		else if (c == '|')
		{
			state = BCOM;
			temp += test;
		}
		else
		{
			state = BCOM;
			temp = temp + test + c;
			istream->advance();
		}
	}
	else if (c == -1)
	{
		state = END;
		Token token(temp, NA, lineNumber);
		tokenList.push_back(token);
	}
	else
	{
		state = BCOM;
		temp += c;
		istream->advance();
	}
	return true;
}

bool LexicalAnalyzer::isEOF(char& c, string& temp, int& lineNumber)
{
	if (c == -1)
	{
		state = MYEOF;
		Token token(temp, EF, lineNumber);
		tokenList.push_back(token);
		return true;
	}
	else
	{
		return isUndefined(c, temp, lineNumber);
	}
}

bool LexicalAnalyzer::isUndefined(char& c, string& temp, int& lineNumber)
{
	state = END;
	temp += c;
	Token token(temp, NA, lineNumber);
	tokenList.push_back(token);
	istream->advance();
	return true;
}

list<Token> LexicalAnalyzer::getList()
{
	return tokenList;
}

InputStream* LexicalAnalyzer::getStream()
{
	return istream;
}

State LexicalAnalyzer::getState()
{
	return state;
}

string LexicalAnalyzer::print()
{
	string output;
	int i = 0;
	for (list<Token>::iterator iter = tokenList.begin(); iter != tokenList.end(); ++iter)
	{
		stringstream iss;
		iss << iter->getLineNum();
		string num = iss.str();
		output += "(" + myList[iter->getType()] + "," + "\"" + iter->getValue() + "\"," + num + ")" + "\n";
		i++;
	}
	stringstream iss2;
	iss2 << i;
	string num2 = iss2.str();
	output += "Total Tokens = " + num2 + "\n";
	return output;
}