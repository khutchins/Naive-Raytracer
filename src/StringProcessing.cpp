#include "StringProcessing.h"

using namespace std;

/*
====================
explode
	Takes a string, a deliminator string, and a queue to place the exploded 
	string in.  It then splits up the string by the deliminator and stores 
	the resulting strings in results.

	Examples: explode("Test/delimit/ foo /ba/r","/",results) would return 
	with { "Test", "delimit", " foo ", "ba", "r" } in the queue.
====================
*/
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