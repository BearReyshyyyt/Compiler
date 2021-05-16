#include <iostream>

#ifndef CHARPROCESS_H
#define CHARPROCESS_H

// enumeration for charaters for
// for the char value lookup
enum Characters{
    letter,
    underscroll,
    digit,
    WS,
    colon,
    greater,
    equal,
    lesser,
    plus,
    minus,
    star,
    slash,
    percent,
    dot,
    LParen,
    RParen,
    comma,
    LCurly,
    RCurly,
    semi,
    LBracket,
    RBracket,
    _EOF,
    NoChar
};

// returns the integer value of char based off
// above enumeration
Characters getCharVal(char);

//returns char at given index from given string
char getChar(std::string, int);

#endif