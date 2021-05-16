#include <iostream>
#include <string>
#include <ctype.h>

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

// returns true if the given string is all whitespace
bool isJustSpace(std::string);
// overload to start from a specific index in the string
bool isJustSpace(std::string, int);
// takes a string and return it without comments
// replaces comments with whitespace
std::string preprocessor(std::string, bool&);

#endif