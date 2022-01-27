#ifndef _KeyValuePair_
#define _KeyValuePair_

#include <string>

using namespace std;

class KeyValuePair {
    private:
    string key;
    string value;

    public:
    KeyValuePair(string key, string value);
    bool operator == (const KeyValuePair &other) const;
    
    string getKey() const ;
    string getValue() const;
};
#endif