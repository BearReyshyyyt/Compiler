#include <iostream>
#include "FSA.h"

#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H

// returns an error message from giver error state
std::string getError(States);
// returns true if the give state is an error state
bool isError(States);

#endif