#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "FSA.h"
#include "node.h"
Token tk;

void error(std::string expectation){
    std::cout << "Parser Error: expected " << expectation << " but got " 
            << tk.getInstance() << " which is " << tk.getIDString() 
            << "at line " << tk.getLineNum() << std::endl;

    exit(-1);
}


nodeTK* parser(std::ifstream &file) {
    nodeTK * root;
    tk = scanner(file);      // initializing the root and pulling the first token
    root = prog(file);
    
    tk = scanner(file);
    if(tk.getID() == EOFTK){  // checking that it reached the end of the file
        return root;
    }else{
        error("EOF");
        return nullptr;
    }
}
nodeTK* prog(std::ifstream &file) {
    nodeTK * p = new nodeTK(PROG);

    if(!tk.getInstance().compare("data")){ // if vars exist
        p->pushChild(vars(file));
        
        
        if(tk.getInstance().compare("main") == 0){ // checks the rest of the programs main and block
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(block(file));
            return p;
        }else{ error("'main'"); }
        
    }else if(!tk.getInstance().compare("main")){  // in case no vars are apparent
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(block(file));
        return p;
    }else{ error("'data' or 'main'"); }
}
nodeTK* block(std::ifstream &file) {
    nodeTK * p = new nodeTK(BLOCK);
    if(!tk.getInstance().compare("begin")){ // processing the block
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(vars(file));           // processing vars
        p->pushChild(stats(file));          // processing statements
        if(!tk.getInstance().compare("end")){ 
            p->pushTk(tk);
            return p;
        }else{ error("'end'"); }
    }else{ error("'begin'"); }
    return nullptr;
}

nodeTK* vars(std::ifstream &file) {
    if(!tk.getInstance().compare("data")){ // validating start of vars
        nodeTK * p = new nodeTK(VARS);  
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ID_TK){ // process identifier
            p->pushTk(tk);
            tk = scanner(file);
            if(tk.getID() == ASN2TK){ // process :=
                p->pushTk(tk);
                tk = scanner(file);
                if(tk.getID() == INT_TK){ // process integer
                    p->pushTk(tk);
                    tk = scanner(file);
                    if(tk.getID() == SEM_TK){ // process semicolon and check for start of another var
                        p->pushTk(tk);
                        tk = scanner(file);
                        p->pushChild(vars(file));
                        return p;
                    }else {error("';'"); }
                }else {error("integer"); }
            }else {error("':='"); }
        }else {error("identifier"); }
    }
    else{
        return nullptr; // returns null incase there are no vars
    }
}
nodeTK* stats(std::ifstream &file) {
    nodeTK * p = new nodeTK(STATS);
    p->pushChild(stat(file)); // processing single statment
    tk = scanner(file);
    p->pushChild(mStat(file));
    return p;
}
nodeTK* stat(std::ifstream &file){
    nodeTK * p = new nodeTK(STAT);
    if(!tk.getInstance().compare("getter")){ // processing the in statement
        p->pushChild(in(file));

        tk = scanner(file);
        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else if(!tk.getInstance().compare("outter")){ // processing out statment
        p->pushChild(out(file));

        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else if(!tk.getInstance().compare("begin")){ // processing a new block
        p->pushChild(block(file));
    }else if(!tk.getInstance().compare("if")){ // processing an if statement
        p->pushChild(IF(file));
        tk = scanner(file);
        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else if(!tk.getInstance().compare("loop")){ // processing loop statement
        p->pushChild(loop(file));
        tk = scanner(file);
        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else if(!tk.getInstance().compare("assign")){ // proecessing assignment statment
        p->pushChild(assign(file));
        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else if(!tk.getInstance().compare("proc")){ // processing proc statement
        p->pushChild(GOTO(file));
        tk = scanner(file);
        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else if(!tk.getInstance().compare("void")){ // processing void statement
        p->pushChild(label(file));
        tk = scanner(file);
        if(tk.getID() == SEM_TK){
            p->pushTk(tk);
            return p;
        }else{ error("';'"); }
    }else{
        error("keyword");  // out puts error if no statement is provided
        exit(-1);
    }
}
nodeTK* mStat(std::ifstream &file){
    // this does the same this but return nullptr because there is no requirment to have 
    // more than one statment
    nodeTK * p = new nodeTK(MSTAT);
    if(!tk.getInstance().compare("getter")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("outter")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("begin")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("if")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("loop")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("assign")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("proc")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else if(!tk.getInstance().compare("void")){
        p->pushChild(stat(file));
        tk = scanner(file);
        p->pushChild(mStat(file));
        return p;
    }else{
        return nullptr;
    }
}

nodeTK* in(std::ifstream &file){
    nodeTK * p = new nodeTK(IN); 
    p->pushTk(tk);          
    tk = scanner(file);
    if(tk.getID() == ID_TK){ 
        p->pushTk(tk);
        return p;
    }else{ error("identifier"); }
}
nodeTK* out(std::ifstream &file){
    //nodeTK a(OUT);
    nodeTK * p = new nodeTK(OUT);
    p->pushTk(tk);
    tk = scanner(file);
    if(tk.getID() == MUL_TK || tk.getID() == LPARTK || tk.getID() == ID_TK || tk.getID() == INT_TK){
        p->pushChild(expr(file));
        return p;
    }else { error("start of expression"); }
}
nodeTK* IF(std::ifstream &file){
    //nodeTK a(IF_);
    nodeTK * p = new nodeTK(IF_);
    p->pushTk(tk);
    tk = scanner(file);
    if(tk.getID() == LBRATK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        p->pushChild(R0(file));
        tk = scanner(file);
        p->pushChild(expr(file));
        if(tk.getID() == RBRATK){
            p->pushTk(tk);
            tk = scanner(file);
            if(!tk.getInstance().compare("then")){
                p->pushTk(tk);
                tk = scanner(file);
                p->pushChild(stat(file));
                
                return p;
            } else {error("'then'");
                //std::cout << "Parser Error: expected 'then' but got " << tk.getInstance() << std::endl;
            }
        } else {error("']'");
            //std::cout << "Parser Error: expected ']' but got " << tk.getInstance() << std::endl;
        }
    } else {error("'['");
        //std::cout << "Parser Error: expected '[' but got " << tk.getInstance() << std::endl;
    }
}
nodeTK* loop(std::ifstream &file){
    //nodeTK a(LOOP);
    nodeTK * p = new nodeTK(LOOP);
    p->pushTk(tk);
    tk = scanner(file);

    if(tk.getID() == LBRATK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        p->pushChild(R0(file));
        tk = scanner(file);
        p->pushChild(expr(file));
        if(tk.getID() == RBRATK){
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(stat(file));
            
            return p;
        } else {error("']'");
            //std::cout << "Parser Error: expected ']' but got " << tk.getInstance() << std::endl;
        }
    } else {error("'['");
        //std::cout << "Parser Error: expected '[' but got " << tk.getInstance() << std::endl;
    }
}
nodeTK* assign(std::ifstream &file){
    //nodeTK a(ASSIGN);
    nodeTK * p = new nodeTK(ASSIGN);
    p->pushTk(tk);
    tk = scanner(file);
    if(tk.getID() == ID_TK){
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ASN2TK){
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(expr(file));
            
            return p;
        } else {error("':='");
            //std::cout << "Parser Error: expected ':=' but got " << tk.getInstance() << std::endl;
        }
    } else {error("identifier");
        //std::cout << "Parser Error: expected identifier but got " << tk.getInstance() << std::endl;
    }
}
nodeTK* GOTO(std::ifstream &file){
    //nodeTK a(GOTO_);
    nodeTK * p = new nodeTK(GOTO_);
    p->pushTk(tk);
    tk = scanner(file);
    if(tk.getID() == ID_TK){
        p->pushTk(tk);
        
        return p;
    }else{error("identifier"); 
        //std::cout << "Parser Error: Expected identifier but got " << tk.getIDString() << std::endl; 
    }
}
nodeTK* label(std::ifstream &file){
    //nodeTK a(LABEL);
    nodeTK * p = new nodeTK(LABEL);
    p->pushTk(tk);
    tk = scanner(file);
    if(tk.getID() == ID_TK){
        p->pushTk(tk);
        
        return p;
    }else{error("identifier");
        //std::cout << "Parser Error: Expected identifier but got " << tk.getIDString() << std::endl; 
    }
}
nodeTK* R0(std::ifstream &file){
    //nodeTK a(R0_);
    nodeTK * p = new nodeTK(R0_);
    if(tk.getID() == MOR_TK){
        p->pushTk(tk);
        return p;
    }else if(tk.getID() == EQV_TK){
        p->pushTk(tk);
        return p;
    }else if(tk.getID() == LES_TK){
        p->pushTk(tk);
        return p;
    }else if(tk.getID() == MOD_TK){
        p->pushTk(tk);
        return p;
    }else if(tk.getID() == LBRATK){
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == EQV_TK){
            p->pushTk(tk);
            tk = scanner(file);
            if(tk.getID() == RBRATK){
                p->pushTk(tk);
                return p;
            }else{error("']'");
                //std::cout << "Parser Error: Expected ']' but got " << tk.getIDString() << std::endl; 
            }
        }else{error("'=='");
            //std::cout << "Parser Error: Expected '==' but got " << tk.getIDString() << std::endl; 
        }
    }else{error("relational operator");
        //std::cout << "Parser Error: Expected relational operator but got " << tk.getIDString() << std::endl; 
    }
}

nodeTK* expr(std::ifstream &file){
    //nodeTK a(EXPR);
    nodeTK * p = new nodeTK(EXPR);
    p->pushChild(N(file));
    if(tk.getID() == SUB_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        return p;
    }else{
        return p;
    }
}
nodeTK* N(std::ifstream &file){
    //nodeTK a(N_);
    nodeTK * p = new nodeTK(N_);
    p->pushChild(A(file));
    if(tk.getID() == DIV_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(N(file));
        return p;
    }if(tk.getID() == MUL_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(N(file));
        return p;
    }else{
        return p;
    }
}
nodeTK* A(std::ifstream &file){
    //nodeTK a(A_);
    nodeTK * p = new nodeTK(A_);
    p->pushChild(M(file));
    tk = scanner(file);
    if(tk.getID() == PLU_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(A(file));
        return p;
    }else{
        return p;
    }
}
nodeTK* M(std::ifstream &file){
    //nodeTK a(M_);
    nodeTK * p = new nodeTK(M_);

    if(tk.getID() == MUL_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(M(file));
        return p;
    }else{
        p->pushChild(R(file));
        return p;
    }
}
nodeTK* R(std::ifstream &file){
    //nodeTK a(R_);
    nodeTK * p = new nodeTK(R_);

    if(tk.getID() == LPARTK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        if(tk.getID() == RPARTK){
            p->pushTk(tk);
            return p;
        }
    }else if(tk.getID() == ID_TK){
        p->pushTk(tk);
        return p;
    }else if(tk.getID() == INT_TK){
        p->pushTk(tk);
        return p;
    }
}

void printParseTree(nodeTK * root){
    // this is the offset that will appear before each token and nonterminal
    static std::string offset = "";

    // first display the offset and nonterminal
    std::cout << offset << root->printLabel() << std::endl;
    
    //determining whether the last child of s specific node has 
    // been reached and changes the offset depending on that
    if(offset.length() > 1){
        offset.pop_back();

        if(offset.back() == '`') { 
            offset.pop_back();
            offset += " ";
        }
        offset += " ";
    }

    // keeping track the index for each child array
    int tokenIndex = 0;
    int childIndex = 0;

    // this is where the printing happens
    for(int i = 0; i < root->printOrder.size(); i++){
        std::string append = "| ";
        if((root->printOrder.size() - i) < 2){ // checks for the last child
            append = "  ";
        }
        if(root->printOrder.at(i) == 1){ // for terminals
            offset += append;
            offset.pop_back(); // making a custom tip for the offset
            offset.pop_back(); 
            if((root->printOrder.size() - i) < 2){
                offset += "`-";
            }else { offset += "|-"; }
            std::cout << offset << root->getTk(tokenIndex)->getInstance() << std::endl;

            tokenIndex++;
        }else if(root->printOrder.at(i) == 0){ // for nonterminal
            offset += append;
            offset.pop_back();
            offset.pop_back();
            if((root->printOrder.size() - i) < 2){
                offset += "`-";
            }else { offset += "|-"; }
            //std::cout << "offset is currently " << offset << std::endl; 
            printParseTree(root->getChild(childIndex));
            
            childIndex++;
        }else{
            std::cout << "something went wrong" << std::endl; // this should never happen
        }
        offset.pop_back(); // erase the custom tip
        offset.pop_back();
    }
}
int findInStack(std::string needle, std::stack<std::string> &stack, int scopeCount, bool copyCheck){
    int count = scopeCount;
    int index = 0;                     // index of the current item in the stack
    std::stack<std::string> tempStack; // a temporary stack to hold the pop items from the original stack
    
    while(!stack.empty()){             // while the stack is not empty check each item in the stack for the needle
        if(copyCheck && count < 1){
            while(!tempStack.empty()){
                stack.push(tempStack.top()); // this find ends here if there was not duplicate found in the same scope
                tempStack.pop();
            }
            return -1;
        }
        std::string curStr = stack.top();
        stack.pop();
        //std::cout << "comparing " << curStr << " to " << needle << " with count=" << count << " and copyCheck=" << copyCheck << std::endl;
        count--;
        if(!needle.compare(curStr)){   // if we find the needle then reload all pop items on stack and return index
            stack.push(curStr);
            while(!tempStack.empty()){
                stack.push(tempStack.top());
                tempStack.pop();
            }
            return index;
        }else{                         // otherwise push the current item on temp stack and update index
            tempStack.push(curStr);
            index++;
        }
    }
    while(!tempStack.empty()){
        stack.push(tempStack.top());
        tempStack.pop();
    }
    return -1;            // return index -1 if item was not found
}
// the determins if the specified variable name alread exist within the given vector of variable names
bool exist(std::string id, std::vector<Token*> &vector){
    for(int i = 0; i < vector.size(); i++){
        if(vector.at(i)->getInstance() == id){
            return true;
        }
    }
    
    return false;            // return index -1 if item was not found
}
//finds the correct instance of a specified variable
std::string findMyScope(std::string id, std::vector<Token*> &vector){
    for(int i = (vector.size() - 1); i >= 0; i--){ // traverse the vector backwards
        if(vector.at(i)->getInstance() == id){     // if the there is a match the return the asm name of that var
            return vector.at(i)->getTempInst();
        }
    }
    return id;            // return itself if no previous one was found. this shouldn't ever happen though
}

static int LabelCntr=0; /* counting unique labels generated */
static int VarCntr=0; /* counting unique temporaries generated */
enum nameType{VAR, LAB};
static std::string name; /* for creation of unique names */
static std::vector<std::string> asmVars; // basically the history of all created vars asm wise


std::string newName(nameType what){ 
    if (what==VAR){ // creating new temporary
        name = "T" + std::to_string(VarCntr++); /* generate a new label as T0, T1, etc */
        asmVars.push_back(name);
    }else{ // creating new Label
        name = "L" + std::to_string(LabelCntr++); /* new lables as L0, L1, etc */
    }
    return(name);
}

std::string treeDecider(nodeTK * root, std::ofstream &asmFile, bool & status){
    std::string value = "";

    static bool globals = true;
    bool creating = false;
    static std::stack<std::string> idStack;
    static std::vector<Token*> idTkVector;
    static int scopeIndex = 0;
    static std::vector<int> scopeCounts{0};
    bool printedNode = false;
    bool exprCalc = false;
    static int copies = 0;
    

    // keeping track the index for each child array
    int tokenIndex = 0;
    int childIndex = 0;

    // this is where the printing happens
    for(int i = 0; i < root->printOrder.size(); i++){
        if(root->printOrder.at(i) == 1){ // for terminals
            std::string curInstance = root->getTk(tokenIndex)->getInstance(); // assigning token instance to string
            
            if(!curInstance.compare("main")){           // this determine whether global variables are being made
                globals = false;
            }else if(!curInstance.compare("data")){     // determins if variables are about to be made
                creating = true;    
            }else if(!curInstance.compare("begin")){    // counts the level of scope
                scopeCounts.push_back(0);   
                scopeIndex++;
                //asmFile << "scope is now " << scopeIndex << std::endl;
            }else if(!curInstance.compare("end")){      // destroys the variables in current scope when scope ends
                for(int i = 0; i < scopeCounts.at(scopeIndex); i++){
                    idTkVector.pop_back();
                    idStack.pop();
                }
                scopeCounts.pop_back();
                if(scopeIndex <= 1){
                    asmFile << "STOP\n";
                    for(int i = 0; i < asmVars.size(); i++){
                        asmFile << asmVars.at(i) << " 0\n";
                    }
                }
                scopeIndex--;
            }

            if(root->getTk(tokenIndex)->getID() == ID_TK){
                if(creating){ // when creating a variable
                    //std::cout << "just got " << curInstance << std::endl;
                    if(findInStack(curInstance, idStack, scopeCounts.at(scopeIndex), true) != (-1)){ // if it all ready exists the cout error
                        std::cout << "P3 ERROR: redefinition of " << curInstance << " at line: "
                                << root->getTk(tokenIndex)->getLineNum() << std::endl;  
                                status = false;
                                //exit(-1);
                    }else{
                        idTkVector.push_back(root->getTk(tokenIndex));
                        //std::cout << "pushing " << curInstance << " into stack with size now " << idStack.size() + 1 << std::endl;
                        idStack.push(curInstance);
                        scopeCounts.at(scopeIndex)++;   // otherwise add it to stack, add 1 to varCount and set creating false
                        creating = false;
                    }
                }else if(findInStack(curInstance, idStack, scopeCounts.at(scopeIndex), false) == (-1)){ // if not creating check if the variable exist
                    std::cout << "P3 ERROR: " << curInstance << " is undefined at line: "
                            << root->getTk(tokenIndex)->getLineNum() << std::endl;  // if not then output error message 
                            status = false;
                            //exit(-1);
                }
            }

            if(!printedNode){
                switch (root->getLabel())
                {
                    case VARS:{ // creating variables 
                        if(exist(root->getTk(1)->getInstance(), idTkVector)){ // first need to see if the variable name exists in an earlier scope
                            root->getTk(1)->setTempInst("c" + std::to_string(copies));
                            copies++;
                            asmVars.push_back(root->getTk(1)->getTempInst());
                        } //if it does add a unique suffix to the tempInstance so that all vars have distinct names in asm file
                        else{
                            asmVars.push_back(root->getTk(1)->getTempInst());
                        }
                        asmFile << "LOAD " << root->getTk(3)->getInstance() << "\n";
                        asmFile << "STORE " << root->getTk(1)->getTempInst() << "\n"; // then right the necessary commands
                        // if(root->ntIndex > 0){
                        //     treeDecider(root->getChild(childIndex), asmFile);  // check child tree
                        // }
                        printedNode = true; // the printing for this node is done (prevents duplicate printing of the same node)
                    }
                        break;
                    case IN: { // getting vars
                        std::string arg1 = root->getTk(1)->getTempInst();
                        arg1 = findMyScope(arg1, idTkVector);  // need to get correct instance of var(find which scope its reffering to)
                        asmFile << "READ " << arg1 << "\n";    // commands to read the var
                        printedNode = true;  
                    }
                        break;
                    case OUT: { // display values
                        std::string arg1 = treeDecider(root->getChild(0), asmFile, status); // find correct instance of variable
                        exprCalc = true;    // not sure if i need this
                        asmFile << "WRITE " << arg1 << "\n";    // commands to display and then value
                        printedNode = true;
                    }
                        break;
                    case R_: {
                        if(root->ntIndex > 0){ // if this is a (<expr>) situation then process the expr
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                            value = arg1;
                        }else{ // if its just a variale or int, then just return the string of it
                            value = root->getTk(0)->getTempInst();
                            if(root->getTk(0)->getID() == ID_TK){
                                value = findMyScope(value, idTkVector); // if its a variable find correct instance.
                            }
                        }
                        printedNode = true;
                    }
                        break;
                    case ASSIGN: { // assignment 
                        std::string arg1 = treeDecider(root->getChild(0), asmFile, status); // process the expr
                        asmFile << "LOAD " << arg1 << "\n";                         // load the value of the expr
                        std::string arg2 = root->getTk(1)->getTempInst();           // get the variable
                        arg2 = findMyScope(arg2, idTkVector);             // get the right instance
                        asmFile << "STORE " << arg2 << "\n";              // store that value in instace
                        printedNode = true;
                    }
                        break;
                    case LABEL: { // generating a label
                        std::string arg1 = root->getTk(1)->getTempInst(); // first getting the variable
                        arg1 = findMyScope(arg1, idTkVector);             // then getting the correct instance of it
                        asmFile << arg1 << ": NOOP\n";              // writing the variable as a label to asm    
                        printedNode = true;
                    }
                        break;
                    case GOTO_: { // jump to label
                        std::string arg1 = root->getTk(1)->getTempInst();
                        arg1 = findMyScope(arg1, idTkVector);   // getting the correct instance of the variable
                        asmFile << "BR " << arg1 << "\n";       // writing a break to the label
                        printedNode = true;
                    }
                        break;
                    case R0_: { // determining the relational operator
                        if(root->getTk(0)->getID() == MOR_TK){ value = "MOR"; }
                        else if(root->getTk(0)->getID() == EQV_TK){ value = "EQV"; }
                        else if(root->getTk(0)->getID() == LES_TK){ value = "LES"; }
                        else if(root->getTk(0)->getID() == MOD_TK){ value = "OPO"; }
                        else { value = "NOTEQV"; }
                    }
                        break;
                    case IF_: { // if statement
                        std::string arg1 = treeDecider(root->getChild(0), asmFile, status); // getting the val of the first expr
                        asmFile << "LOAD " << arg1 << "\n";                         // load the first expression
                        std::string arg2 = treeDecider(root->getChild(2), asmFile, status); // getting the val of the first expr
                        asmFile << "SUB " << arg2 << "\n";                          // subing the arg2 from the acc
                        std::string cond = treeDecider(root->getChild(1), asmFile, status); // getting the expr
                        std::string lab1 = newName(LAB); // generating a label to jump too if condition not met

                        if(!cond.compare("MOR")){ asmFile << "BRNEG " << lab1 << "\n"; } // breaks for greater op
                        else if(!cond.compare("EQV")){                          // breaks for equivalent op
                            asmFile << "BRNEG " << lab1 << "\n"; 
                            asmFile << "BRPOS " << lab1 << "\n"; 
                        }else if(!cond.compare("LES")){ asmFile << "BRPOS " << lab1 << "\n"; } // breaks for less op
                        else if(!cond.compare("NOTEQV")){ asmFile << "BRZERO " << lab1 << "\n"; } // break not equal op
                        else{ // breaks for oposite op. Its a little more complicated but it makes sense if you look at it
                            std::string lab2 = newName(LAB);
                            asmFile << "SUB " << arg1 << "\n";
                            asmFile << "BRPOS " << lab2 << "\n";
                            asmFile << "STORE " << arg1 << "\n";
                            asmFile << "BRZPOS " << lab1 << "\n";
                            asmFile << lab2 << ": " << "STORE " << arg1 << "\n";
                            asmFile << "BRZNEG " << lab1 << "\n"; 
                        }
                        treeDecider(root->getChild(3), asmFile, status);    //the statements in the if statement
                        
                        asmFile << lab1 << ": NOOP\n"; // the label for skipping the instructions
                        printedNode = true;
                    }
                        break;
                    case LOOP:{
                        std::string lab0 = newName(LAB); //all this is the same as the if statment except for some small parts
                        asmFile << lab0 << ": NOOP\n";   // like this: this is the label to jump back to the top of the loop
                                                         // this is what makes the loop actually loop
                        std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                        asmFile << "LOAD " << arg1 << "\n";
                        std::string arg2 = treeDecider(root->getChild(2), asmFile, status);
                        asmFile << "SUB " << arg2 << "\n";
                        std::string cond = treeDecider(root->getChild(1), asmFile, status);
                        std::string lab1 = newName(LAB);

                        if(!cond.compare("MOR")){ asmFile << "BRNEG " << lab1 << "\n"; }
                        else if(!cond.compare("EQV")){ 
                            asmFile << "BRNEG " << lab1 << "\n"; 
                            asmFile << "BRPOS " << lab1 << "\n"; 
                        }else if(!cond.compare("LES")){ asmFile << "BRPOS " << lab1 << "\n"; }
                        else if(!cond.compare("NOTEQV")){ asmFile << "BRZERO " << lab1 << "\n"; }
                        else{ 
                            std::string lab2 = newName(LAB);
                            asmFile << "SUB " << arg1 << "\n";
                            asmFile << "BRPOS " << lab2 << "\n";
                            asmFile << "STORE " << arg1 << "\n";
                            asmFile << "BRZPOS " << lab1 << "\n";
                            asmFile << lab2 << ": " << "STORE " << arg1 << "\n";
                            asmFile << "BRZNEG " << lab1 << "\n"; 
                        }
                        treeDecider(root->getChild(3), asmFile, status); // the statment
                        asmFile << "BR " << lab0 << "\n";        // like this: take processing back to the top of the loop
                        
                        asmFile << lab1 << ": NOOP\n";           // label for breaking out of the loop
                        printedNode = true;
                    }
                        break;
                    
                    default:
                        break;
                }
            }
            tokenIndex++;

        }else if(root->printOrder.at(i) == 0){ // for nonterminal   
            if(!printedNode){
                switch (root->getLabel()) {
                    case EXPR: {
                        if(root->ntIndex > 1){
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status); // processing the value of <N> into arg1
                            std::string arg2 = treeDecider(root->getChild(1), asmFile, status); // processing the value of <expr> into arg2
                            asmFile << "LOAD " << arg1 << "\n";     // loading arg1 into the acc
                            asmFile << "SUB " << arg2 << "\n";      // subbing the acc with arg2
                            std::string arg3 = newName(VAR);        
                            asmFile << "STORE " << arg3 << "\n";    // saving the value of the subtraction
                            value = arg3;   // this will return the value of arg3
                        }else{ // when there is no subtraction
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status); // processing the value of <N> int arg1
                            value = arg1;                   // returns the value
                        }
                        printedNode = true;
                        break;
                    }
                    case N_: {
                        if(root->ntIndex > 1){// same idea here except that its either multiplication or division
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                            std::string arg2 = treeDecider(root->getChild(1), asmFile, status);
                            asmFile << "LOAD " << arg1 << "\n";
                            if(root->getTk(0)->getID() == DIV_TK){
                                asmFile << "DIV " << arg2 << "\n";
                            }else{
                                asmFile << "MULT " << arg2 << "\n";
                            }
                            std::string arg3 = newName(VAR);
                            asmFile << "STORE " << arg3 << "\n";
                            value = arg3;
                        }else{ // when there is no multiplication or division
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                            value = arg1;
                        }   
                        printedNode = true;
                        break;
                    }
                    case A_: { // samething here too but for addition
                        if(root->ntIndex > 1){
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                            std::string arg2 = treeDecider(root->getChild(1), asmFile, status);
                            asmFile << "LOAD " << arg1 << "\n";
                            asmFile << "ADD " << arg2 << "\n";
                            std::string arg3 = newName(VAR);
                            asmFile << "STORE " << arg3 << "\n";
                            value = arg3;
                        }else{ // when there is no addition
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                            value = arg1;
                        }
                        printedNode = true;
                        break;
                    }
                    case M_: { // processing negations
                        if(root->tkIndex > 0){
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status); //processing the value of <M> into arg1
                            asmFile << "LOAD " << "0" << "\n";  // loading 0 into the acc
                            asmFile << "SUB " << arg1 << "\n";  // subing arg1 from the acc
                            std::string arg2 = newName(VAR);    // storing the result in a new temp variable
                            asmFile << "STORE " << arg2 << "\n";    
                            value = arg2;
                        }else{ // when there is no negation
                            std::string arg1 = treeDecider(root->getChild(0), asmFile, status);
                            value = arg1;
                        }
                        printedNode = true;
                        break;
                    }
                    default:
                        treeDecider(root->getChild(childIndex), asmFile, status);  // check child tree
                        break;
                }
            }else if(root->getChild(childIndex)->getLabel() == VARS){
                treeDecider(root->getChild(childIndex), asmFile, status);  // check child tree
            }
            
            childIndex++;
        }else{
            std::cout << "something went wrong" << std::endl; // this should never happen
        }

    }
    return value;
}

