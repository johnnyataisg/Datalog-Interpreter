#pragma once
#ifndef INPUTSTREAM_H_
#define INPUTSTREAM_H_
#include <string>

using namespace std;

class InputStream
{
private:
	string str;
	int current_line;
	unsigned int char_location;
public:
	InputStream(string input)
	{
		str = input;
		current_line = 1;
		char_location = 0;
	}

	char currentChar()
	{
		if (char_location >= str.size())
		{
			return -1;
		}
		else
		{
			return str[char_location];
		}
	}

	void advance()
	{
		if (currentChar() != -1)
		{
			if (str[char_location] == '\n')
			{
				current_line++;
			}
			char_location++;
		}
	}

	int getLineNumber()
	{
		return current_line;
	}

	int getCharLocation()
	{
		return char_location;
	}

	string getString()
	{
		return str;
	}
};

#endif