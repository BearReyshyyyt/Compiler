#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "node.h"
#include "parser.h"

// the are the string equivalents for each nonterminal
std::string myLabels[21] = {
        "PARSER",
        "PROG",
        "BLOCK",
        "VARS",
        "STATS",
        "STAT",
        "MSTAT",
        "IN",
        "OUT",
        "IF",
        "LOOP",
        "ASSIGN",
        "GOTO",
        "LABEL",
        "R0",
        "EXPR",
        "N",
        "A",
        "M",
        "R",
        "none"
    };

// constructor
nodeTK::nodeTK(){
    label = none;
}
// parameterized constructor
nodeTK::nodeTK(nonTerms non_terminal){
    label = non_terminal;
}
//adds a new token to the token array
void nodeTK::pushTk(Token token){
    if(tkIndex < 5){ // makes sure you are not adding more tokens than what is allowed
        printOrder.push_back(1);
        tk[tkIndex] = new Token(token.getID(), token.getInstance(), token.getLineNum());
        tkIndex++;
    }else{
        //std::cout << "your pushing too many tokens in this node with tk " << token.getInstance() << std::endl;
    }
}
//adds a new nonterminal child to the nonterminal array
void nodeTK::pushChild(nodeTK* nodeChild){
    if(ntIndex < 4 && nodeChild){ // makes sure you dont add more nonterms that what is allowed
        printOrder.push_back(0);
        children[ntIndex] = nodeChild;
        ntIndex++;
    }
    else{
        //std::cout << printLabel() << " " << nodeChild->printLabel() << std::endl;
        //std::cout << "your pushing too many non-terminals in this node or this pointer is null" << std::endl;
    }
}
//returns the enumerated label of node
nonTerms nodeTK::getLabel(){
    return label;
}
//returns the string version of the label
std::string nodeTK::printLabel(){
    std::string str = myLabels[label];
    return str;
}
//returns the token child of the given index
Token* nodeTK::getTk(int index){
    return tk[index];
}
//returns the nonterminal child of the given index
nodeTK* nodeTK::getChild(int index){
    return children[index];
}
// also prints the parse tree but this one is part of the class
void nodeTK::preOrder(){
    static std::string offset = "";

    std::cout << offset << printLabel() << std::endl;
    

    if(offset.length() > 1){
        offset.pop_back();

        if(offset.back() == '`') { 
            offset.pop_back();
            offset += " ";
        }
        offset += " ";
    }

    int tokenIndex = 0;
    int childIndex = 0;

    for(int i = 0; i < printOrder.size(); i++){
        std::string append = "| ";
        if((printOrder.size() - i) < 2){ // checks for the last child
            append = "  ";
        }
        if(printOrder.at(i) == 1){ // for terminals
            offset += append;
            offset.pop_back();
            offset.pop_back();
            if((printOrder.size() - i) < 2){
                offset += "`-";
            }else { offset += "|-"; }
            std::cout << offset << getTk(tokenIndex)->getInstance() << std::endl;

            tokenIndex++;
        }else if(printOrder.at(i) == 0){ // for nonterminal
            offset += append;
            offset.pop_back();
            offset.pop_back();
            if((printOrder.size() - i) < 2){
                offset += "`-";
            }else { offset += "|-"; }
            getChild(childIndex)->preOrder();
            
            childIndex++;
        }else{
            std::cout << "something went wrong" << std::endl;
        }
        offset.pop_back();
        offset.pop_back();
    }
}