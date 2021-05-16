#include <iostream>
#include <string>
#include "stringMult.h"
using std::cout;
using std::endl;

// overloads the * operator to multiply the amount of times a string is printed
std::string operator*(const std::string& str, size_t times){
    std::string multiString;
    for (size_t i = 0; i < times; i++){
        multiString += str;
    }
    return multiString;
}

// int main(){
//     std::string name = "hi";
//     std::cout << name * 10;
    
//     return 0;
// }