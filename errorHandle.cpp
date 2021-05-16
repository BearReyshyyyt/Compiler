#include <iostream>
#include "errorHandle.h"
#include "FSA.h"

// this just holds an array of possible error messages
// index would corrospond to the enumarted errors in :Stats;
// did it this way to eliminate if statements
const int SIZE = 6;
std::string errorMessage[SIZE]{
    "Scanner Error: Identifier exceeded character limit: ",
    "Scanner Error: Integer exceeded length limit: ",
    "Scanner Error: Underscroll can only appear at beginning of identifier: ",
    "Scanner Error: Token begins with unacceptable character: ",
    "Scanner Error: Unacceptable character found: ",
    "Scanner Error: Identifiers may only begin with lowercase letters: "
};

// returns the error message with a given errorState (code)
std::string getError(States code){
    return errorMessage[((int)code * -1) - 1];
}

//returns true is the current state is an error
bool isError(States state){
    if(state < (States)0)
        return true;
    
    return false;
}

// int main(){
//     std::cout << getError((States)-1);
// }