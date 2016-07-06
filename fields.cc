#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <cassert>
#include <cctype>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/types.h>
#include <xapian.h>

using namespace std;

using Xapian::Utf8Iterator;

using Xapian::Unicode::is_wordchar;

class Fields {
    mutable Xapian::docid did_cached;
    mutable map<string, string> fields;
    Xapian::Database db;

    void read_fields(Xapian::docid did) const;

  public:
    Fields() : did_cached(0) { } 

    const string & get_field(Xapian::docid did, const string & field) const {
        if (did != did_cached) read_fields(did);
        return fields[field];
    }   
};

void Fields::read_fields(Xapian::docid did) const
{
    fields.clear();
    did_cached = did;
    const string & data = db.get_document(did).get_data();

    // Parse document data.
    string::size_type i = 0;
    const string & names = option["fieldnames"];
    if (!names.empty()) {
        // Each line is a field, with fieldnames taken from corresponding
        // entries in the tab-separated list specified by $opt{fieldnames}.
        string::size_type n = 0;
        do {
            string::size_type n0 = n;
            n = names.find('\t', n);
            string::size_type i0 = i;
            i = data.find('\n', i);
            fields.insert(make_pair(names.substr(n0, n  - n0),
                                    data.substr(i0, i - i0)));
        } while (++n && ++i);
    } else {
        // Each line is a field, in the format NAME=VALUE.  We assume the field
        // name doesn't contain an "=".  Lines without an "=" are currently
        // just ignored.
        do {
            string::size_type i0 = i;
            i = data.find('\n', i);
            string line = data.substr(i0, i - i0);
            string::size_type j = line.find('=');
            if (j != string::npos) {
                string & value = fields[line.substr(0, j)];
                if (!value.empty()) value += '\t';
                value += line.substr(j + 1);
            }
        } while (++i);
    }
}

