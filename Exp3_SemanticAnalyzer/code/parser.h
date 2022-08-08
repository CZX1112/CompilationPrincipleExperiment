#ifndef PARSER_H
#define PARSER_H

#include "production.h"
#include "lritem.h"
#include "token.h"
#include <vector>
#include <string>
#include <set>
#include <map>

typedef std::pair<std::string, int> psi;
typedef std::vector<psi> vpsi;
typedef std::set<LR1item> LR1set;

class Parser
{
private:
    std::vector<Production> grammar;   // store grammar
    std::vector<LR1set > closurelist;  // closure list
    std::map<LR1set, int> closuremap;  // closure map
    std::vector<vpsi > transfer;       // transfer function
    std::vector<std::map<std::string, psi>> action;     // action table
    std::vector<std::map<std::string, int>> go;         // goto table
    std::set<std::string > variableSet;                 // variable symbol set


    std::set<std::string> getFirst(const std::vector<std::string>& beta);   // get first(beta), beta = A1A2...An
    std::map<std::string, std::set<std::string>> firstMap;  // firstMap[X] = first(X), X is a symbol
    void buildFirstMap();                                   // build first map

    std::set<LR1item> getClosure(const LR1item& item);      // get closure
    void GetClosure(LR1set& closure);                       // merge closure
    int translate(int id, std::string name);

public:
    Parser() {}

    bool openFile(const std::string& fileName);
    void build();

    std::vector<Production> getGrammar();
    std::set<std::string> getFirst(const std::string& symbol);                   // get first(symbol)

    std::vector< LR1set > getClosurelist();
    std::map< LR1set, int> getClosuremap();
    std::vector< vpsi > getTransfer();
    std::vector< std::map<std::string, psi>> getAction();
    std::vector< std::map<std::string, int>> getGo();
    int analyse(const std::vector<Token>& tokens);
    std::set< std::string > getVariableSet();
    std::set< std::string > getTerminalSet();
    std::set<std::string > terminalSet;                 // terminal symbol set

};

#endif // PARSER_H
