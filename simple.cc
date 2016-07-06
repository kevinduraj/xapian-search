#include <xapian.h>
#include <iostream>
#include "querystring.h"

using namespace std;

int main(int argc, char **argv)
{
    cout << "Content-type: text/html\r\n\r\n";

    if (argc < 2) {
        cout << "usage: " << argv[0] <<
                " <path to database> <search terms>" << endl;
        return 1;
    }

    try {
        Xapian::Database db("/data/index/0525/");
        Xapian::Enquire enquire(db);

        // Build the query object
        Xapian::Query query(Xapian::Query::OP_AND, argv + 1, argv + argc);
        cout << "Performing query `" << query.get_description() << "'" << endl;

        enquire.set_query(query);

        Xapian::MSet matches = enquire.get_mset(0, 10);

        cout << matches.size() << " results found" << endl;

        for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) 
        {
            Xapian::Document doc = i.get_document();
            cout << "Document ID " << *i << "\t" << i.get_percent() << "% [" << doc.get_data() << "]" << endl;
        }

        cout << "Performed query `" << query.get_description() << "'" << endl;
        cout << matches.size() << " results found" << endl;
        cout << argv + 1 << " " <<  argv + argc;

    } catch(const Xapian::Error &error) {
        cout << "Exception: "  << error.get_msg() << endl;
    }
}


