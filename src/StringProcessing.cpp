#include "StringProcessing.h"

using namespace std;

void explode(string str, string deliminator, queue<string>* results){
    size_t idx;
    idx = str.find_first_of(deliminator);
    while(idx != string::npos){
        if(idx > 0){
            results->push(str.substr(0,idx));
        }
        str = str.substr(idx+1);
        idx = str.find_first_of(deliminator);
    }
    if(str.length() > 0){
        results->push(str);
    }
}