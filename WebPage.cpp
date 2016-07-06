/*---------------------------------------------------------------------------------------------------------------------
 * Written by: Kevin Duraj 
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

    if(format == "export") {
      filename = random_string(12);
      path = "/home/design/www/csv/" + filename;
      csv.open(path);
    }
}
/*----------------------------------------------------------------------------------------------------------------------*/
WebPage::~WebPage() {

    if(format == "export") csv.close();
}
/*----------------------------------------------------------------------------------------------------------------------*/
void WebPage::myjson(string str) {

    ptree pt;
    pt.put ("foo", "bar");
    std::ostringstream buf; 
    write_json (buf, pt, false);
    std::string json = buf.str();
    cout << json << endl;
}
/*----------------------------------------------------------------------------------------------------------------------*/
string WebPage::random_string(size_t length) {

    srand(time(0));
    //Generate random string for filename
    auto randchar = []() -> char {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str + ".csv";
}
/*----------------------------------------------------------------------------------------------------------------------
                               Parsing Query Result and Returning JSON
  ----------------------------------------------------------------------------------------------------------------------*/
void WebPage::parse_xapian_json(string result) {

    boost::replace_all(result, "\n", "|");
    result.erase(result.length()-1, 1); // erease the last | character

    vector<string> vector1;
    boost::split(vector1, result, boost::is_any_of("|"));

    ptree pt;   // json
    std::string delimiter = "=";
    size_t pos = 0;

    //ptree * children = new ptree(); 
    ptree * child    = new ptree(); 
    
    for (vector<string>::iterator it = vector1.begin(); it != vector1.end(); ++it) {

        string value = *it;
        pos = value.find(delimiter);
        std::string key_token = value.substr(0, pos);
        value.erase(0, pos + 1);
        //pt.put(key_token, value);
        child->put(key_token, value);
    }
    
    children.push_back(std::make_pair("", *child));
    delete child;

    //ptJson.add_child("Document", *children);
    //delete children;
    //write_json(std::cout, pt);

}
/*----------------------------------------------------------------------------------------------------------------------
  if((format == "search") || (format == "export")) 
  ----------------------------------------------------------------------------------------------------------------------*/
void WebPage::parse_xapian_result(string result) {

    boost::replace_all(result, "\n", "|");
    result.erase(result.length()-1, 1); // erease the last | character

    vector<string> vector1;
    boost::split(vector1, result, boost::is_any_of("|"));

    ptree pt;   // json
    std::string delimiter = "=";
    size_t pos = 0;

    for (vector<string>::iterator it = vector1.begin(); it != vector1.end(); ++it) {

        // cout << *it << "<br>" << endl;
        string value = *it;
        pos = value.find(delimiter);
        std::string key_token = value.substr(0, pos);
        value.erase(0, pos + 1);
        pt.put(key_token, value);

        //if(format == "csv") csv << "\"" << value << "\","; 
        //cout << key_token << " | " << endl;  // get key
    }
    
    if(format == "export") {
      try { csv << "\"" << pt.get<std::string>("url")         <<  "\","; } catch(...) { csv << "\"null\","; }
      try { csv << "\"" << pt.get<std::string>("title")       <<  "\","; } catch(...) { csv << "\"null\","; }
      try { csv << "\"" << pt.get<std::string>("description") <<  "\","; } catch(...) { csv << "\"null\","; }
      csv << endl;
    } 
     
    try { 
      cout << "<b>Title = </b> "  <<  "<a href=http://pacific-design.com/" << pt.get<std::string>("url") << ">" << pt.get<std::string> ("title") << "</a><br>" << endl; 
    } catch(...) { cout << "<b>url = </b> NULL<br>" << endl;  }


    try { 
      cout << "<b>Description = </b>"  <<  pt.get<std::string>("description") << "<br>" << endl;
    } catch(...) { cout << "<b>description = </b> NULL<br>" << endl;  }

}
/*--------------------------------------------------------------------------------------------------------------------------*/
int WebPage::search( string terms, int page, int items ) {


    if((format == "export") && (format != "json")) {
      cout << "<h3>Download Link: <a href=\"csv/" << filename << "\">csv/" << filename << "</a></h3><p>" << endl;
      csv  << "date, platform_content_id, yt_video_title, yt_description, yt_channel_id, yt_category_id"  << endl;
    }

    string key, value, result, delimiter;

    int start  =  (page * items) - items + 1 ;
    if(start < 1) { start = 1; }

    // ------------------------------//
    // OP_OR | OP_PHRASE | OP_NEAR
    // ------------------------------//
    query_parser.set_default_op(Xapian::Query::OP_AND);
    Xapian::Query query = query_parser.parse_query(terms, Xapian::QueryParser:: FLAG_BOOLEAN);

    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    //enquire.set_sort_by_value_then_relevance(1, 1);

    Xapian::MSet mset = enquire.get_mset(start-1, items);
    int total         = mset.get_matches_estimated();

    if (format != "json") {

    	cout << query.get_description() << "<br>" << endl;
    	cout << "Page=<b>" << page << "</b>&nbsp;&nbsp;&nbsp;Start=<b>" << start << "</b><br>" << endl; 
	    cout << "Seaching "  << db.get_doccount()  << " pages for: <b>" << terms 
	         << "</b> about <b>" << total << "</b> results found." << endl;
    }

    int counter=start;

    if(format == "json") ptJson.put("EstimatedTotal", total);

    for (Xapian::MSetIterator i = mset.begin(); i != mset.end(); i++) {

      Xapian::Document doc = i.get_document();

    	if (format != "json") {
      		cout  << "<hr><b>" << counter << ".</b>&nbsp;&nbsp<b>Relevance</b> = " << i.get_percent() 
		            << "%&nbsp;|&nbsp<b>Terms</b> = " <<  doc.termlist_count() << "</b>&nbsp;|&nbsp<p>\n" << endl;
    	} //else {
        //if(json_first > 0 ) cout << ","; 
  		  //json_first++; 
	      // }
 
    	result = doc.get_data();
    	if(format == "json") parse_xapian_json(result); 
      else parse_xapian_result(result);
    	counter++;
    } /* end of iterator if stattement */

    if(format == "json") {
      ptJson.add_child("WebPage", children);
      write_json(std::cout, ptJson); 
    }

    //if(format == "json") cout << "] }" << endl;					

    return total;

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
