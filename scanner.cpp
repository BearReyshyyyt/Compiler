#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "scanner.h"
#include "FSA.h"
#include "token.h"
#include "charProcess.h"
#include "errorHandle.h"
#include "preprocessor.h"
#include "stringMult.h"

bool printToken = false;

void printTK(Token tk){
    if(printToken){
        std::cout << tk.getIDString() << " : " << tk.getInstance() << " " << tk.getLineNum() << std::endl;
    }
}

//array of keywords
const int keyLength = 14;
std::string keyWords [keyLength] = {"begin","end","loop","whole","void","exit","getter","outter","main","if","then","assign","data","proc"};

//returns true if the current identifier is a keyword
bool isKeyWord(std::string id){
    for(int i = 0; i < keyLength; i++){
        if(id.compare(keyWords[i]) == 0){
            return true; 
        }
    }
}
void isCommentOpen(bool open, int lineNumber){
    if(open){
        std::cout << "Scanner error: comment is left open at line " << lineNumber << std::endl;
        exit(-1);
    }
}

// this takes one line at a time.
// its given the unprocessed line, processed line, index to start reading from, and line number
// it return a token or an error message 
Token lineScanner(std::string rawLine, std::string line, int &index, int lineNumber, bool eof){
    Token tempToken = Token();  //creating a null token
    if(eof){
        tempToken = Token(EOFTK, "EOF", lineNumber);
        return tempToken;
    }

    line += '\n'; //reappends that \n taken away by getline()
    States state = s1;  //setting initial state to 0
    States nextState;   
    char lookAhead = line[index]; //setting char to starting index
    std::string S = "";

    while(state < 1000){
        nextState = (States)(Table[state][getCharVal(lookAhead)]);

        //produce error message and exit program if nextstate will result in error
        if(isError(nextState)){
            std::cout << getError(nextState) << "'" << lookAhead << "'" << std::endl;
            std::cout << "line " << lineNumber << ":" << (index + 1) << ":\n" << "'" << rawLine << "'" << std::endl;
            std::string space = " ";
            std::cout << space * (index + 1) << "^";
            exit(-1);
        }
        // returning the appropriate token depending on the state
        if(nextState >= 1000) {
            Token tempToken = Token(nextState, S, lineNumber);
            if(tempToken.tokenID == ID_TK){ // checking to see if the id is a keyword
                if(isKeyWord(S)){
                    tempToken.setTkID(KW_TK); // changing token type if it is
                }
                if(tempToken.getInstance().at(0) > 64 && tempToken.getInstance().at(0) < 91){
                    std::cout << "Scanner Error: Identifiers may only begin with lowercase letters: " << "'" << tempToken.getInstance().at(0) << "'" << std::endl;
                    std::cout << "line " << lineNumber << ":" << (index + 1) << ":\n" << "'" << rawLine << "'" << std::endl;
                    std::string space = " ";
                    std::cout << space * (index + 1 - tempToken.getInstance().length()) << "^";
                    exit(-1);
                }
                return tempToken;
            }else {
                return tempToken;
            }
        }else { // if no token has been found update the state and get the next lookahead
            state = nextState;
            if(!isspace(lookAhead))
                S += lookAhead;
            lookAhead = line[++index];
        }
    }

    return tempToken;
}

// so this part of the scanner feed lines to the line scanner and returns what ever token it gets
Token scanner(std::ifstream &fileTcompile){
    static int lineNumber = 1;
    static std::string rawLine;             // all variables are used to record the current
    static std::string line;                // position of the scanner
    static bool endOfFile = false;
    static bool fileComplete = false;
    static bool openComment = false;
    static int index = 0;
    static bool lineDone = true;
    bool gotToken = false;

    if(!endOfFile){
        while(!gotToken){
            if(lineDone){
                getline(fileTcompile, rawLine);
                if(!fileComplete && !fileTcompile)  // this constatly checks if the end of the
                {                                   // has been reached yet
                    if(fileTcompile.eof()){
                        endOfFile = true;           // if it has change bool
                    }
                    else
                        std::cout << "other file read error";
                }
                if(!fileComplete){          // if the file is read fine just reset values for new line
                    lineDone = false;
                    line = preprocessor(rawLine, openComment);
                    index = 0;
                }
            }

            if(!lineDone){
                if(!isJustSpace(line, index) && !fileComplete){ // wont search line if tail is all whitespace
                    if(endOfFile){
                        fileComplete = true;
                        lineNumber--;
                    }
                    Token currentTk = lineScanner(rawLine, line, index, lineNumber, endOfFile); // creates a token
                    printTK(currentTk);
                    return currentTk;
                    
                } else if (endOfFile && !fileComplete){ // this cathes the case of trailing new lines
                    fileComplete = true;
                    if(isJustSpace(line,index)){ //this will correctly set the line number incase of trailing WS
                        lineNumber--;
                    }
                    isCommentOpen(openComment, lineNumber);
                    Token currentTk = lineScanner("EOF", "EOF", index, lineNumber, endOfFile);
                    printTK(currentTk);
                    return currentTk;
                    
                } else {
                    lineDone = true; // the line is complete will move on to next line
                }

                if(index >= line.length()){
                    lineDone = true; // line is done when index reaches the end of string
                }
            }
            lineNumber++; // incrementing the line number
        }
    }
    std::cout << "does this still run?" << std::endl;
    Token currentTk = lineScanner("EOF", "EOF", index, lineNumber, endOfFile); // this shouldn't run but incase
    printTK(currentTk);                                                        // it does it returns and eof token 
    return currentTk;                                                              
}



