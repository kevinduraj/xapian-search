/*-------------------------------------------------------------------------------------------------
 * WebPage.h
 * author: kevinduraj
 *-------------------------------------------------------------------------------------------------*/
#ifndef ENGINE_PROFILE_H_
#define ENGINE_PROFILE_H_
/*-------------------------------------------------------------------------------------------------*/
#include <xapian.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
/*----------------------------------------------------------------------------------------------------------------------------------*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "Parser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
/*----------------------------------------------------------------------------------------------------------------------------------*/
using namespace std;
using std::string;
using namespace Xapian;

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
/*----------------------------------------------------------------------------------------------------------------------------------*/
class WebPage
{

private:

	string xapian_index;
	Xapian::Database db;
	Xapian::QueryParser query_parser;
	Parser parser;

	vector<string> terms;
	vector<string>::iterator iter;

	string format;
	string filename;
	string path;
	ofstream csv;

	void myjson(string str);
	string random_string(size_t length);
	void parse_xapian_result( string result );
	void parse_xapian_json(string result);

  ptree ptJson;
  ptree children;
  //ptree child1, child2, child3;


public:

	WebPage(string xapian_index, string action);
	~WebPage();

	int search( string terms, int page, int items );
	void browse_records( Database &db );
};

#endif /* ENGINE_PROFILE_H_ */
/*-------------------------------------------------------------------------------------------------*/
