#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "cleanString.h"
#include "testScanner.h"
#include "parser.h"
#include "node.h"

//This program reads informations either from file of from std in, scans the code and then
//parses it. The program can be ran 3 ways
//  1. ./prog [file]
//  2. ./prog < [file]
//  3. ./prog      // then wait for the program to propt you for user input


int main(int argc, char** argv) {
    bool stdin_is_redirected = isatty(0) == 0; //bool to check if data is redirected
    std::string fileName = "file";
    std::string targetFile;
    int numOfArgs = argc;

    // if else depending on how data is being input into the program
    if (numOfArgs < 2 && !stdin_is_redirected) { //if no arg was passed and data is not redirected
        std::ofstream userFile(fileName);
        std::string response;
        std::string text;
        bool recieveText = true;
        targetFile = "kb.asm";

        // promt the user to enter data
        std::cout << "Please enter as many words as you'd like and press enter when you are done" << std::endl;
        while(recieveText){ // loop until user is sure they are done
            text = "";
            std::cout << "continue: " << std::endl;
            std::cin.sync();
            std::getline(std::cin, text);
            text += " ";

            userFile << text;
            std::cout << "Are you sure you are done? (y/Y or n/N)" << std::endl;
            response = "";
            while(response.compare("y") && response.compare("Y") && response.compare("n") && response.compare("N")){
                std::cin >> response;
            }

            if(!response.compare("y") || !response.compare("Y")) { recieveText = false; }
        }

    }else if (argc < 2 && stdin_is_redirected){ //if there is no arg and data is from redirect
        std::ofstream userFile(fileName);
        std::string text;
        targetFile = "redir.asm";


        while(std::getline(std::cin, text)){ //imput all data to a temporary file
            userFile << text;
            userFile << "\n";
        }

    }else {  // if there is an arg
        if(examinString(argv[1], '.')){ //if they included the extension
            fileName = argv[1];
            targetFile = apndFileEx(clnFileName(argv[1]), ".asm");
        }else {                         //if they did not include the extension then add it
            fileName = apndFileEx(argv[1], ".fs");
            targetFile = apndFileEx(argv[1], ".asm");
        }
    }

    char * targetKill = new char [targetFile.length()+1];
    strcpy (targetKill, targetFile.c_str());

    std::ifstream parseFile(fileName);
    std::ofstream asmFile(targetFile);

    if(!parseFile){
        std::cout << "sorry that file does not exist" << std::endl;
        exit(0);
    }

    nodeTK * top;
    top = parser(parseFile);
    //printParseTree(top);

    bool status = true;
    treeDecider(top, asmFile, status); // this runs the code gen and returns a fail message if this part failed
    asmFile.close();
    
    if(!status){ // this means the there was a context issue
        remove(targetKill); // be very carful with this
    }else{
        std::cout << "compilation complete." << std::endl;
    }

    parseFile.close();
    remove("file"); // be super careful with this

    return 0;
}
