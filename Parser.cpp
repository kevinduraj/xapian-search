#include "Parser.h"
/*----------------------------------------------------------------------------------------------------------------------------*/
/*                                      Parse Xapian doc.getdata() function                                                   */
/*----------------------------------------------------------------------------------------------------------------------------*/
void Parser::parse_xapian(string& line, string& delimiter, string& key, string& value)
{
  //string::size_type j = line.find('=');
  //string::size_type j = line.find("yt_description");
  string::size_type j = line.find(delimiter);

  if (j != string::npos)
  {
	  key   = line.substr(0, j);
  	value = line.substr(j );
  }
}
/*---------------------------------------------------------------------------------------------------------------------------*/
/*                                         Split String into multiple values                                                 */
/*---------------------------------------------------------------------------------------------------------------------------*/
void Parser::split(const string& s, char c, vector<string>& v)
{
  string::size_type i = 0;
  string::size_type j = s.find(c);

  while (j != string::npos)
  {
     v.push_back(s.substr(i, j-i));
     i = ++j;
     j = s.find(c, j);

     if (j == string::npos)
        v.push_back(s.substr(i, s.length( )));
  }
}
/*-------------------------------------------------------------------------------------------------------------------------*/
/*                                           Find and Replace Values                                                       */
/*-------------------------------------------------------------------------------------------------------------------------*/
void Parser::find_replace(string & str,	string const & pattern,	string const & replace)
{
  string::size_type start = str.find( pattern, 0 );
  while ( start != str.npos )
  {
    str.replace( start, pattern.size(), replace );
    start = str.find( pattern, start+replace.size() );
  }
}
/*----------------------------------------------------------------------------------------------------------------------------*/
/*                                           Convert string to lower case                                                     */
/*----------------------------------------------------------------------------------------------------------------------------*/
string Parser::tolower(string str)
{
   for (unsigned i=0; i < strlen(str.c_str()); i++)
     if (str[i] >= 0x41 && str[i] <= 0x5A)
       str[i] = str[i] + 0x20;
   return str;
}
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*                                                     Trim String                                                             */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void Parser::rtrim(string& s, char c)
{
   if (s.empty( )) return;

   string::iterator p;
   for (p = s.end( ); p != s.begin( ) && *--p == c;);

   if (*p != c)
      p++;

   s.erase(p, s.end( ));
}
/*----------------------------------------------------------------------------------------------------------------------------*/
void Parser::padstring()
{
  //s. insert(s. begin( ), 20 Ð s. length( ), ' X' );
}
/*----------------------------------------------------------------------------------------------------------------------------*/
