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

/*----------------------------------------------------------------------------------------------------------------------------------*/
class WebPage
{

private:
  
    static const int PAGE_SIZE = 10;
    Xapian::MSet mset;

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
    void parse_xapian_result( string terms, string result );
    void printfcomma (int n);

public:

	WebPage(string xapian_index, string action);
	~WebPage();

  int search( string terms, int page, string sort );
	void browse_records( Database &db );
};

#endif /* ENGINE_PROFILE_H_ */
/*-------------------------------------------------------------------------------------------------*/
