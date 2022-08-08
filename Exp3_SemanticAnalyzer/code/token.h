#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

class Token
{
public:
    Token()
    {
        name = "";
        type = TokenType::OTHER;
        line = 0;
    }

    Token(std::string name, TokenType type, size_t line)
    {
        this->name = name;
        this->type = type;
        this->line = line;
    }

    operator bool()
    {
        if (type == TokenType::OTHER)
            return false;
        else
            return true;
    }

    void setName(const std::string& name);
    std::string getName() const;
    void setType(TokenType type);
    TokenType getType() const;
    std::string getTypeOutput() const;
    void setLine(size_t line);
    size_t getLine() const;

private:
    std::string name;
    TokenType type;
    size_t line;

};

#endif // TOKEN_H
