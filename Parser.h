#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
/*-------------------------------------------------------------------------------------------------------------------*/
using namespace std;
/*-------------------------------------------------------------------------------------------------------------------*/
#ifndef PARSER_H_
#define PARSER_H_
/*-------------------------------------------------------------------------------------------------------------------*/
class Parser
{
public:
	// constructor
	Parser() { };

	void parse_xapian(string& line, string& delimiter, string& key, string& value);
	void split(const string& s, char c, vector<string>& v);
	void find_replace(string & str, string const & pattern, string const & replace);
	string tolower(string str);
	void rtrim(string& s, char c);
	void padstring();

};
/*-------------------------------------------------------------------------------------------------------------------*/
#endif /* PARSER_H_ */
