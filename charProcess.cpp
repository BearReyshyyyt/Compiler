#include <iostream>
#include "charProcess.h"


//returns the character value of the given character
//  -im using enumerated value instead of their
//  -ascii values cuz these values are going to
//  -be the "column" index and using the ascii
//  -would make that table way bigger than it
//  -alreadi is.
Characters getCharVal(char lookup){
    if(isalpha(lookup))
        return letter;
    if(lookup == '_')
        return underscroll;
    if(isdigit(lookup))
        return digit;
    if(isspace(lookup))
        return WS;
    if(lookup == ':')
        return colon;
    if(lookup == '>')
        return greater;
    if(lookup == '=')
        return equal;
    if(lookup == '<')
        return lesser;
    if(lookup == '+')
        return plus;
    if(lookup == '-')
        return minus;
    if(lookup == '*')
        return star;
    if(lookup == '/')
        return slash;
    if(lookup == '%')
        return percent;
    if(lookup == '.')
        return dot;
    if(lookup == '(')
        return LParen;
    if(lookup == ')')
        return RParen;
    if(lookup == ',')
        return comma;
    if(lookup == '{')
        return LCurly;
    if(lookup == '}')
        return RCurly;
    if(lookup == ';')
        return semi;
    if(lookup == '[')
        return LBracket;
    if(lookup == ']')
        return RBracket;
    return NoChar;
}

//returns the character a certain index in a string
//i pretty sure this already exists though but 
//whatever.
char getChar(std::string s, int index){
    return s[index];
}