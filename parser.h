#ifndef  PARSER_H
#define  PARSER_H

#include "node.h"

nodeTK * parser(std::ifstream &);
nodeTK * prog(std::ifstream &);
nodeTK * block(std::ifstream &);
nodeTK * vars(std::ifstream &);
nodeTK * stats(std::ifstream &);
nodeTK * stat(std::ifstream &);
nodeTK * in(std::ifstream &);
nodeTK * mStat(std::ifstream &);
nodeTK * out(std::ifstream &);
nodeTK * IF(std::ifstream &);
nodeTK * loop(std::ifstream &);
nodeTK * assign(std::ifstream &);
nodeTK * GOTO(std::ifstream &);
nodeTK * label(std::ifstream &);
nodeTK * R0(std::ifstream &);
nodeTK * expr(std::ifstream &);
nodeTK * N(std::ifstream &);
nodeTK * A(std::ifstream &);
nodeTK * M(std::ifstream &);
nodeTK * R(std::ifstream &);
void printParseTree(nodeTK *);
int find(std::string, std::stack<std::string>&);
std::string treeDecider(nodeTK *, std::ofstream&, bool &);

#endif //  PARSER_H

