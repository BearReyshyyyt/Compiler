#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "token.h"

enum nonTerms{
    PARSER,
    PROG,
    BLOCK,
    VARS,
    STATS,
    STAT,
    MSTAT,
    IN,
    OUT,
    IF_,
    LOOP,
    ASSIGN,
    GOTO_,
    LABEL,
    R0_,
    EXPR,
    N_,
    A_,
    M_,
    R_,
    none = -1
};


class nodeTK{
    public:
    nonTerms label;
    std::vector<int> printOrder;
    int tkIndex = 0;
    Token* tk[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    int ntIndex = 0;
    nodeTK* children[4] = {nullptr, nullptr, nullptr, nullptr};
    //std::string offset = "";

    nodeTK();
    nodeTK(nonTerms);
    void pushTk(Token);
    void pushChild(nodeTK*);
    nonTerms getLabel();
    std::string printLabel();
    Token* getTk(int);
    nodeTK* getChild(int);
    void preOrder();
};

#endif //NODE_H