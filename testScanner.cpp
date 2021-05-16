#include <iostream>
#include <fstream>
#include <iomanip>
#include "scanner.h"

// function that runs and tests the scanner
void testScanner(std::ifstream &file){
    Token token = Token();
    while(token.getID() != EOFTK){
        token = scanner(file);
        std::cout << "token: " << std::setw(14) << token.getIDString() // prints all info on token
                  << "\tinstance: " << std::setw(9) << token.getInstance()
                  << "\tline number: " << std::setw(5) << token.getLineNum() << std::endl;
    }
}

// int main(){
//     std::ifstream file("tester.txt");

//     testScanner(file);

//     return 0;
// }