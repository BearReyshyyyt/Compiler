#include <iostream>
#include "FSA.h"
#ifndef TOKEN_H
#define TOKEN_H


// struct for Tokens
struct Token{
    States tokenID;         // represents the enumerated token type of the token (ie. IDTK, INTK ....)
    std::string instance;   // holds the actual token
    std::string tempInst;
    int lineNum;            // holds the line its on

    // array of token types
    std::string tokens[24] = {
        "Identifier",
        "Integer",
        "Keyword",
        "Colon",
        "Assignment 2",
        "Assignment 1",
        "Great or eql",
        "Equivilent",
        "Less or eql",
        "Plus",
        "Minus",
        "Multiplication",
        "Division",
        "Modulus",
        "dot operator",
        "L Parentheses",
        "R Parentheses",
        "Comma",
        "L Curly Brace",
        "R Culty Brace",
        "Semicolon",
        "L Bracket",
        "R Bracket",
        "EOF"
    };

    public:
    // default constructor
    Token();
    // Token constructor
    Token(States, std::string, int);
    // return enumerated token type
    States getID();
    // return string token type
    std::string getIDString();
    // returns token itself
    std::string getInstance();
    // returns the token's line
    int getLineNum();
    // reassigns the tokens enumerated type
    void setTkID(States);
    // copies the info of one token to itself
    void copyTk(Token tk);
    std::string getTempInst();
    void setTempInst(std::string);
};

#endif