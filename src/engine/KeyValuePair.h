#ifndef _KeyValuePair_
#define _KeyValuePair_

#include <string>



class KeyValuePair {
    private:
    std::string key;
    std::string value;

    public:
    KeyValuePair(std::string key, std::string value);
    bool operator == (const KeyValuePair &other) const;
    
    std::string getKey() const ;
    std::string getValue() const;
};
#endif