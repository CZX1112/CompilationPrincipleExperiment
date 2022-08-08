#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

#include <vector>

enum class Process
{
    BEGIN,
    IN_KEYWORD,
    IN_ID,
    IN_INTEGER,
    IN_FLOAT,
    IN_OPERATOR,
    IN_DELIMITER,
    IN_CHAR,
    END
};

class Scanner
{
public:
    Scanner();
    std::vector<Token> getTokens(const std::string FileName);
    std::string Preprocessing(std::string InputStr, int InputStrLen);

private:
    Process proc;
    std::string FileName;
    std::string code;
    std::string::const_iterator iter;
    size_t line;


    void dealBegin(std::string& name, TokenType& type);
    void dealKeyWord(std::string& name, TokenType& type);
    void dealID(std::string& name, TokenType& type);
    void dealInteger(std::string& name, TokenType& type);
    void dealFloat(std::string& name, TokenType& type);
    void dealOperator(std::string& name, TokenType& type);
    void dealDelimiter(std::string& name, TokenType& type);
    //void dealString(std::string &name, TokenType &type);
    void dealChar(std::string& name, TokenType& type);
    Token dealEnd(const std::string& name, const TokenType& type);

    void skipBlank();
    bool openFile(const std::string& FileName);
    Token getNextToken();

};

#endif // SCANNER_H
