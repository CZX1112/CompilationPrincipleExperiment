#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <map>
#include <memory>
#include <string>
#include <vector>

enum class TokenType
{
    KEYWORD,
    ID,
    INT,
    FLOAT,
    CHAR,
    OPERATOR,
    DELIMITER,
    OTHER
};

static std::map<TokenType, std::string> TokenDict =
{
    {TokenType::KEYWORD,"KEYWORD"},
    {TokenType::ID,"ID"},
    {TokenType::INT,"INT"},
    {TokenType::FLOAT,"FLOAT"},
    {TokenType::CHAR,"CHAR"},
    {TokenType::OPERATOR,"OPERATOR"},
    {TokenType::DELIMITER,"DELIMITER"},
    {TokenType::OTHER,"OTHER"}
};


static std::vector<std::string> TokenStringType =
{
    //constant
    "INT","FLOAT","CHAR",

    //id
    "ID",

    //operator
    "+","-","*","/","&","^","~","|","++","--","||","&&","!","==","!=","<",">","<=",">=","=",

    //delimiter
    ";", "{", "}", "[", "]", "(", ")", ".","'",

    //keyword
    "char", "float", "int", "if", "else", "while", "continue", "break"
};

#endif // TOKENTYPE_H_INCLUDED
