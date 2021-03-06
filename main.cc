#include <xapian.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "querystring.h"
#include "WebPage.h"
#include "Parser.h"

using namespace std;
using namespace Xapian;
using namespace boost;
using std::string;

static const std::string INDEX  = "/home/design/index";
static const int PAGE_SIZE      = 10;

struct tokens: std::ctype<char> 
{
    tokens(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table() {
        typedef std::ctype<char> cctype;
        static const cctype::mask *const_rc= cctype::classic_table();

        static cctype::mask rc[cctype::table_size];
        std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

        rc[','] = std::ctype_base::space; 
        rc[' '] = std::ctype_base::space; 
        return &rc[0];
    }
};

/*--------------------------------------------------------------------------------------------------------*/
void header(string action, string terms, string format, string page) {

 	cout << "Content-type: text/html\r\n\r\n";
 	cout << "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"stylesheet.css\"></head>" << endl;
 	cout << "<body>\n";

 	 /* DEBUG
 	     char InputData[4096];
 	     strcpy(InputData, getenv("QUERY_STRING"));
 	     cout << InputData << "<p>" << endl;
 	     cout << action << endl;
 	 */

 	cout << "<table width=1024 border=0><tr>";

    if(format == "null") { cout << "<form action=\"search\" method=\"get\">"  << endl; }
    else {
         string uri = terms;
         boost::replace_all(uri, " ", "+");
         cout << "<form action=\"search?q=" << uri << "\" method=\"get\">"  << endl;
    }

    cout << "<td valign=\"middle\"><a href=\"http://pacific-design.com\"><img src=\"pacific-design-com-home.png\"></a></td>";
    cout << "<td valign=\"top\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>";
 	cout << "<td valign=\"middle\"><nobr>" << endl;
 	cout << "<input class=\"input\" type=\"text\" name=\"q\" maxlength=\"100\" size=\"95\" value=\"" << terms << "\" >" << endl;
 	cout << "<input type=\"hidden\" name=\"page\" value=\"" << page << "\">" << endl;
 	cout << "<button class=\"button\" name=\"action\" type=\"submit\" value=\"search\"/> Search </button>" << endl;
 	cout << "</td></tr></form></table><hr>" << endl;

}
/*--------------------------------------------------------------------------------------------------------*/
void footer(string q, int total) {

  if(total > PAGE_SIZE) {
    
    boost::replace_all(q, " ", "+");
    //result.erase(result.length()-1, 1); // erease the last | character

    int last_page = (total / PAGE_SIZE) + 1;
    if(last_page > 2001) last_page = 2001;

    cout << "<hr>" << endl;

    for(int i=1; i <= last_page; i++) {
      cout << "<a href=\"http://pacific-design.com/search?q=" << q << "&page=" << i << "\">" << i << "</a> " << endl; 
    }

  }

  cout << "<hr><p><br><p><center><table border=0 width=\"800\"><tr><td align=center >" << endl;
  cout << "&copy 2016 Pacific-Design.com All Rights Reserved.</td></tr>" << endl;
  cout << "<tr><td align=center>Brought to you by Kevin Duraj since 1997 from Los Angeles, CA" << endl;
  cout << "</td></tr></table></center></body></html>" << endl;

}
/*--------------------------------------------------------------------------------------------------------*/

void web_search() {

  char q[512]; 
  char format[16]; 
  char page[16]; 
  char action[16]; 
  char sort[16];

  int total = 0;

  getAllParams();
  getParam("q", q); string query(q);
  getParam("format", format);
  getParam("page", page);
  getParam("action", action);
  getParam("sort", sort);

  if(strcmp (page,"null") == 0) strcpy(page, "1");

  header(action, q, format, page);

  if(strcmp (q,"null") != 0) {
 
    WebPage * video = new WebPage(INDEX, action);
    //total = video->search(q, atoi(page), PAGE_SIZE);
    total = video->search(query, atoi(page), sort);
    delete video;

  } else { cout << "<h2>Please enter search query</h2>" << endl; }

  if(strcmp (action,"json") != 0) footer(q, total);


}
/*--------------------------------------------------------------------------------------------------------
 * Tokenizer
 * ------------------------------------------------------------------------------------------------------*/
void tokenize() {
    string text = "one|two|three|four|five";

    char_separator<char> sep("|");
    tokenizer<char_separator<char> > tokens(text, sep);
    BOOST_FOREACH(string t, tokens) {
        cout << t <<  endl;
    }
}
/*--------------------------------------------------------------------------------------------------------
 * Main Function 
 * ------------------------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    web_search();
}
/*-------------------------------------------------------------------------------------------------------*/
/* Notes: 
 *  Split key/value pair
 *
    std::string s = "key1=value1 key2=value2 continue2 key3=value3 continue";
    std::stringstream ss(s);
    ss.imbue(std::locale(std::locale(), new tokens()));
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);
    std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "<p>\n"));
*/

