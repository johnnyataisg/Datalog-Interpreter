#include "Parser.h"

//Constructor: Takes the list of tokens inside the LexicalAnalyzer object and loads it into a vector data member of the Parser object
Parser::Parser(list<Token> list)
{
	while (!list.empty())
	{
		tokenList.push_back(list.front());
		list.pop_front();
	}
	index = 0;
}

//Destructor: Deallocates all memory used for parameter pointers by calling a clear function from each member of DatalogProgram
Parser::~Parser()
{
	for (set<Parameter*>::iterator iter = dlist.begin(); iter != dlist.end(); ++iter)
	{
		delete *iter;
	}
}

//Matches the current input token with a token that should come next and saves its value to string temp
void Parser::match(TokenType tt)
{
	if (tokenList.at(index).getType() == tt)
	{
		temp = "";
		temp = tokenList.at(index).getValue();
		index++;
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Begin the parsing process
bool Parser::parse()
{
	try 
	{
		parseDatalog();
		return true;
	}
	catch (Token token)
	{	
		cout << "Failure!" << endl << "  {" << myList[token.getType()] << "," << token.getValue() << "," << token.getLineNum() << ")" << endl;
		return false;
	}
}

//Begin a recursive process of parsing the datalog program
void Parser::parseDatalog()
{
	//Push production symbols onto the stack and continue further productions until a token is encountered and match it with the tokens from the vector
	match(SCH);
	match(COL);
	parseScheme();
	parseSchemeList();
	match(FAC);
	match(COL);
	parseFactList();
	match(RUL);
	match(COL);
	parseRuleList();
	match(QUE);
	match(COL);
	parseQuery();
	parseQueryList();
	match(EF);
}

//Parse the scheme section of the datalog program
void Parser::parseScheme()
{
	if (tokenList.at(index).getType() == ID)
	{
		state = PS;
		match(ID);
		datalog.pushSchemeList(Predicate((temp)));
		match(LPAR);
		match(ID);
		Parameter* object = new Id(temp);
		datalog.addSchemeParam(object);
		dlist.insert(object);
		parseIdList();
		match(RPAR);
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of scheme predicates
void Parser::parseSchemeList()
{
	if (tokenList.at(index).getType() == ID)
	{
		parseScheme();
		parseSchemeList();
	}
	else if (tokenList.at(index).getType() == FAC)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of ID's
void Parser::parseIdList()
{
	if (tokenList.at(index).getType() == COM)
	{
		match(COM);
		match(ID);
		if (state == PS)
		{
			Parameter* object = new Id(temp);
			datalog.addSchemeParam(object);
			dlist.insert(object);
		}
		else
		{
			Parameter* object = new Id(temp);
			datalog.addParamToHead(object);
			dlist.insert(object);
		}
		parseIdList();
	}
	else if (tokenList.at(index).getType() == RPAR)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse the fact section of the datalog program
void Parser::parseFact()
{
	if (tokenList.at(index).getType() == ID)
	{
		match(ID);
		datalog.pushFactList(Predicate(temp));
		match(LPAR);
		match(STR);
		Parameter* object = new String(temp);
		datalog.addFactParam(object);
		dlist.insert(object);
		parseStringList();
		match(RPAR);
		match(PER);
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of fact predicates
void Parser::parseFactList()
{
	if (tokenList.at(index).getType() == ID)
	{
		parseFact();
		parseFactList();
	}
	else if (tokenList.at(index).getType() == RUL)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse the rule section of the datalog program
void Parser::parseRule()
{
	if (tokenList.at(index).getType() == ID)
	{
		state = PR;
		parseHeadPredicate();
		match(COLD);
		parsePredicate();
		datalog.addPredToRules(pred);
		pred.clear();
		parsePredicateList();
		match(PER);
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of Rule rules
void Parser::parseRuleList()
{
	if (tokenList.at(index).getType() == ID)
	{
		parseRule();
		parseRuleList();
	}
	else if (tokenList.at(index).getType() == QUE)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse the head predicate for rules
void Parser::parseHeadPredicate()
{
	if (tokenList.at(index).getType() == ID)
	{
		match(ID);
		datalog.pushRuleList(Rules(Predicate(temp)));
		match(LPAR);
		match(ID);
		Parameter* object = new Id(temp);
		datalog.addParamToHead(object);
		dlist.insert(object);
		parseIdList();
		match(RPAR);
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse a predicate for rules and queries
void Parser::parsePredicate()
{
	if (tokenList.at(index).getType() == ID)
	{;
		match(ID);
		pred = Predicate(temp);
		match(LPAR);
		pred.push(parseParameter());
		//pred.push(param);
		parseParameterList();
		match(RPAR);
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of predicates for rules and queries
void Parser::parsePredicateList()
{
	if (tokenList.at(index).getType() == COM)
	{
		match(COM);
		parsePredicate();
		datalog.addPredToRules(pred);
		pred.clear();
		parsePredicateList();
	}
	else if (tokenList.at(index).getType() == PER)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse a parameter with different methods depending on its derived class type
Parameter* Parser::parseParameter()
{
	if (tokenList.at(index).getType() == STR)
	{
		match(STR);
		Parameter* object = new String(temp);
		dlist.insert(object);
		return object;
	}
	else if (tokenList.at(index).getType() == ID)
	{
		match(ID);
		Parameter* object = new Id(temp);
		dlist.insert(object);
		return object;
	}
	else if (tokenList.at(index).getType() == LPAR)
	{
		return parseExpression();
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of parameters for a predicate
void Parser::parseParameterList()
{
	if (tokenList.at(index).getType() == COM)
	{
		match(COM);
		pred.push(parseParameter());
		//pred.push(param);
		parseParameterList();
	}
	else if (tokenList.at(index).getType() == RPAR)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse an expression
Parameter* Parser::parseExpression()
{
	if (tokenList.at(index).getType() == LPAR)
	{
		match(LPAR);
		//temp2 += temp;
		Expression* object = new Expression();
		dlist.insert(object);
		object->setLeft(parseParameter());
		object->setOper(parseOperator());
		object->setRight(parseParameter());
		match(RPAR);
		return object;
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse an operator
string Parser::parseOperator()
{
	if (tokenList.at(index).getType() == ADD)
	{
		match(ADD);
		return temp;
	}
	else if (tokenList.at(index).getType() == MUL)
	{
		match(MUL);
		return temp;
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Parse the query section of the datalog program
void Parser::parseQuery()
{
	state = PQ;
	if (tokenList.at(index).getType() == ID)
	{
		parsePredicate();
		datalog.pushQueryList(pred);
		match(QM);
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of query predicates
void Parser::parseQueryList()
{
	if (tokenList.at(index).getType() == ID)
	{
		parseQuery();
		parseQueryList();
	}
	else if (tokenList.at(index).getType() == EF)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Recursive function to parse a list of strings
void Parser::parseStringList()
{
	if (tokenList.at(index).getType() == COM)
	{
		match(COM);
		match(STR);
		Parameter* object = new String(temp);
		datalog.addFactParam(object);
		dlist.insert(object);
		parseStringList();
	}
	else if (tokenList.at(index).getType() == RPAR)
	{
	}
	else
	{
		throw tokenList.at(index);
	}
}

//Return the datalog member object
DatalogProgram Parser::getDatalog()
{
	return this->datalog;
}