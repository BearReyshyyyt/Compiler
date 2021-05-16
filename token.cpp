#include <iostream>
#include "token.h"

// default constructor
Token::Token(){
    tokenID = noState;
    instance = "\0";
    tempInst = "\0";
    lineNum = -1;

}

// Token constructor
Token::Token(States ID, std::string inst, int line){
    tokenID = ID;
    instance = inst;
    tempInst = inst;
    lineNum = line;
}
// reassigns the tokens enumerated type
void Token::setTkID(States st){
    tokenID = st;
} 
// return enumerated token type
States Token::getID(){
    return tokenID;
}
// return string token type
std::string Token::getIDString(){
    return tokens[tokenID - 1000];
}
// returns token itself
std::string Token::getInstance(){
    return instance;
}
// returns the token's line
int Token::getLineNum(){
    return lineNum;
}

void Token::copyTk(Token tk){
    tokenID = tk.getID();
    instance = tk.getInstance();
    lineNum = tk.getLineNum();
}

std::string Token::getTempInst(){
    return tempInst;
}
void Token::setTempInst(std::string inst){
    tempInst = inst;
}

// int main () {

//     Token name = returnTk();

//     std::cout << name.instance;

//     return 0;
// }