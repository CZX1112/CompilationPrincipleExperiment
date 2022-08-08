#include "token.h"

void Token::setName(const std::string& name)
{
    this->name = name;
}

std::string Token::getName() const
{
    return this->name;
}

void Token::setType(TokenType type)
{
    this->type = type;
}

TokenType Token::getType() const
{
    return this->type;
}

void Token::setLine(size_t line)
{
    this->line = line;
}

size_t Token::getLine() const
{
    return this->line;
}

std::string Token::getTypeOutput() const
{
    if (this->type == TokenType::KEYWORD)
        return "KEYWORD";
    if (this->type == TokenType::ID)
        return "ID";
    if (this->type == TokenType::INT)
        return "INT";
    if (this->type == TokenType::FLOAT)
        return "FLOAT";
    if (this->type == TokenType::CHAR)
        return "CHAR";
    if (this->type == TokenType::OPERATOR)
        return "OPERATOR";
    if (this->type == TokenType::DELIMITER)
        return "DELIMITER";
    if (this->type == TokenType::OTHER)
        return "OTHER";
    return nullptr;
}
