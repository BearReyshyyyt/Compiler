#include <iostream>
#include <string>
#include "cleanString.h"
#include <ctype.h>

// cleans a string of any unecessary special characters
std::string cleanString(std::string str){
    for(unsigned int i = 0; i < str.length(); i++){
        if(!isalpha(str[i]) && str[i] != '\''){ //checks each charater of string
            str.erase(i,1);
            i--;
        }
    }
    return str;
}

// searches string for a secific character by iteration
bool examinString(std::string word, char key){
    for(unsigned int i = 0; i < word.length(); i++){
        if(word[i] == key){
            return true;
        }
    }
    return false;
}

//removes the extention of a file name
std::string clnFileName(std::string str){
    bool foundDot = false;
    for(unsigned int i = 0; i < str.length(); i++){ // check for '.' and delete it plus everything after
        if(str[i] == '.' || foundDot){
            foundDot = true;
            str.erase(i,1);
            i--;
        }
    }
    return str;
}

//adds a desired extension to a file name
std::string apndFileEx(std::string str, std::string extension){
    str += extension;
    return str;
}