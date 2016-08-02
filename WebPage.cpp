/*---------------------------------------------------------------------------------------------------------------------
 * Written by: Kevin Duraj 
 * http://pacific-design.com
 ----------------------------------------------------------------------------------------------------------------------*/
#include "WebPage.h"
#include <xapian.h>
#include <stdio.h>
#include <string.h>
#include <regex>
#include <ctime>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
/*---------------------------------------------------------------------------------------------------------------------*/
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

/*---------------------------------------------------------------------------------------------------------------------*/
WebPage::WebPage(string xapian_index, string action) {

    db.add_database(Xapian::Database(xapian_index));
    this->format = action;

}
/*----------------------------------------------------------------------------------------------------------------------*/
WebPage::~WebPage() {

    if(format == "export") csv.close();
}
/*----------------------------------------------------------------------------------------------------------------------
  if((format == "search") || (format == "export")) 
  ----------------------------------------------------------------------------------------------------------------------*/
void WebPage::parse_xapian_result(string terms, string result)  {

    //boost::replace_all(result, "\n", "|");
    //result.erase(result.length()-1, 1); // erease the last | character

    vector<string> vector1;
    boost::split(vector1, result, boost::is_any_of("\n"));

    ptree pt;   // json
    std::string delimiter = "=";
    size_t pos = 0;

    for (vector<string>::iterator it = vector1.begin(); it != vector1.end(); ++it) {
        string value = *it;
        pos = value.find(delimiter);
        std::string key_token = value.substr(0, pos);
        value.erase(0, pos + 1);
        pt.put(key_token, value);
        //cout << key_token << "=" << value << "<br>" << endl;
    }
 
    try { 
      cout << "<b>Title = </b> "  <<  "<a href=http://pacific-design.com/" << pt.get<std::string>("url") << ">" << pt.get<std::string> ("title") << "</a><br>" << endl; 
    } catch(...) { cout << "<b>url = </b><a href=http://pacific-design.com>Pacific-Design.com</a><br>" << endl;  }


    try { 
      cout << "<b>Description = </b>"  <<  pt.get<std::string>("description") << "<br>" << endl;
    } catch(...) { cout << "<b>description = </b> NULL<br>" << endl;  }

}
/*--------------------------------------------------------------------------------------------------------------------------*/
int WebPage::search( string terms, int page, string sort ) {

    string key, value, result, delimiter;

    int start  =  (page * PAGE_SIZE) - PAGE_SIZE + 1 ;
    if(start < 1) { start = 1; }

    // ------------------------------//
    // OP_OR | OP_PHRASE | OP_NEAR
    // ------------------------------//
    query_parser.set_default_op(Xapian::Query::OP_AND);
    Xapian::Query query = query_parser.parse_query(terms, Xapian::QueryParser:: FLAG_BOOLEAN);

    Xapian::Enquire enquire(db);
    enquire.set_query(query);

    if (sort.compare("true") == 0) {
        enquire.set_sort_by_value_then_relevance(1, 1);
    }

    mset = enquire.get_mset(start-1, PAGE_SIZE);
    int total  = mset.get_matches_estimated();

    //cout << query.get_description() << "<br>" << endl;
    cout << "<div class=text1>" << endl;
    cout << "Searching " << endl;
    printfcomma(db.get_doccount());
    cout << " videos, about <b>" << endl;
    printfcomma(total);
    cout << "</b> results found." << endl;
    cout << "<br>Terms: <b>" <<  terms << "</b></div>" << endl;

    int counter=start;

    for (Xapian::MSetIterator i = mset.begin(); i != mset.end(); i++) {

        Xapian::Document doc = i.get_document();
        //for( Xapian::TermIterator t = doc.termlist_begin(); t != doc.termlist_end(); t++) cout << *t << "<br>" << endl;

        cout  << "<hr><div class=text1><b>" << counter << ".</b>&nbsp;&nbsp<b>Relevance</b>=" << i.get_percent() << "%</div>" << endl;

        result = doc.get_data();
        parse_xapian_result(terms, result);
        counter++;

    } /* end of iterator if stattement */

    return total;

}
/*----------------------------------------------------------------------------------------------------------------------*/
void WebPage::printfcomma (int n) {
    if (n < 0) {
        printf ("-");
        printfcomma (-n);
        return;
    }
    if (n < 1000) {
        printf ("%d", n);
        return;
    }
    printfcomma (n/1000);
    printf (",%03d", n%1000);
}
/*---------------------------------------------------------------------------------------------------------------------------

  // Regular expression
    regex word_regex("\\w+=");
    string res1 = std::regex_replace (str, word_regex, "|$0");
    res1.erase(0, 1); // erease first | character

    //if(format == "csv") {
    //  string result_csv = result;
    //  boost::replace_all(result_csv, "\n", ",");
    //  csv << result_csv << endl; 
    //}

---------------------------------------------------------------------------------------------------------------------------*/
