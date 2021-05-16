#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>
#include "preprocessor.h"


// returns true if the given string is all whitespace
bool isJustSpace(std::string s){
    for(int i = 0; i < s.length(); i++){
        if(!isspace(s[i]))
            return false;
    }
    return true;
}

// overload to start from a specific index in the string
bool isJustSpace(std::string s, int start){
    for(int i = start; i < s.length(); i++){
        if(!isspace(s[i]))
            return false;
    }
    return true;
}

// takes a string and return it without comments
// replaces comments with whitespace
std::string preprocessor(std::string line, bool &openComment){
    static bool skipMode = false;
    bool dollarFound = false;

    for(int i = 0; i < line.length(); i++){
        
        if(line[i] == '$') {  // if it finds a $ then it examins text before and after
            if((i < (line.length() - 1)) && line[i+1] == '$'){  // if its next to another $ then 
                line[i] = 32;                                   // its either the end or start of
                line[i+1] = 32;                                 // of a comment
                skipMode = !skipMode; 
            }else if(skipMode) {//replace single $ with whitespace
                line[i] = 32;
            }
        }else if(skipMode) { // replacing lines in comments with whitespace
            line[i] = 32;
        }
    }
    openComment = skipMode;
    return line;
}

// int main(){
//     std::string s = "bobert $$a;sldkfjadlkfj$$ dslkajf;a   sa;fd";
//     std::cout << preprocessor(s);
// }