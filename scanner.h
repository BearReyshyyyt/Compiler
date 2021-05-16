#include <iostream>
#include <fstream>
#include "FSA.h"
#include "token.h"
#include "charProcess.h"

#ifndef SCANNER_H
#define SCANNER_H

// returns true if the given string matches a keyword
bool isKeyWord(std::string);
// returns a token from the given info
Token lineScanner(std::string, std::string, int&, int, bool);
Token scanner(std::ifstream&);

#endif