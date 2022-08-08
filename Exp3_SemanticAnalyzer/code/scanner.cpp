#include "scanner.h"
#include "KeyWord.h"
#include "TokenType.h"
#include <fstream>
#include <cctype>
#include<iostream>

using namespace std;

Scanner::Scanner()
{
    proc = Process::BEGIN;
    line = 0;
}

void Scanner::skipBlank()
{
    while (iter != code.cend() && *iter != EOF)
    {
        if (*iter == '\n')
        {
            iter++;
            line++;
        }
        else if (isblank(*iter))
        {
            iter++;
        }
        else
        {
            break;
        }
    }
}

void Scanner::dealBegin(std::string& name, TokenType& type)
{
    if (std::isdigit(*iter))
        proc = Process::IN_INTEGER;
    else if (std::isalpha(*iter) || *iter == '_')
        proc = Process::IN_ID;
    else if (*iter == ',' || *iter == ';' || *iter == '{' || *iter == '}'
             || *iter == '[' || *iter == ']' || *iter == '(' || *iter == ')'
             || *iter == '#' || *iter == '.' || *iter == '"' || *iter == '\'')
        proc = Process::IN_DELIMITER;
    else
        proc = Process::IN_OPERATOR;
}

void Scanner::dealKeyWord(std::string& name, TokenType& type)
{
    type = TokenType::KEYWORD;
    proc = Process::END;
}

void Scanner::dealID(std::string& name, TokenType& type)
{
    if (std::isalnum(*iter))
    {
        name += *iter;
        iter++;
    }
    else if (keyWordSet.find(name) != keyWordSet.end())
    {
        proc = Process::IN_KEYWORD;
    }
    else
    {
        type = TokenType::ID;
        proc = Process::END;
    }
}

void Scanner::dealInteger(std::string& name, TokenType& type)
{
    if (std::isdigit(*iter))
    {
        name += *iter;
        iter++;
    }
    else if (*iter == '.')
    {
        name += *iter;
        iter++;
        proc = Process::IN_FLOAT;
    }
    else
    {
        type = TokenType::INT;
        proc = Process::END;
    }
}

void Scanner::dealFloat(std::string& name, TokenType& type)
{
    if (std::isdigit(*iter))
    {
        name += *iter;
        iter++;
    }
    else
    {
        type = TokenType::FLOAT;
        proc = Process::END;
    }
}

void Scanner::dealOperator(std::string& name, TokenType& type)
{
    if (*iter == '*' || *iter == '/' || *iter == '%' || *iter == '^' || *iter == '~')
    {
        name += *iter;
        iter++;
        type = TokenType::OPERATOR;
        proc = Process::END;
    }
    else if (*iter == '!' || *iter == '=' || *iter == '&' || *iter == '|' || *iter == '<' || *iter == '>' || *iter == '+' || *iter == '-')
    {
        name += *iter;
        const char lastOp = *iter;
        iter++;
        if (iter != code.cend())
        {
            if (lastOp == '!' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '=' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '&' && *iter == '&')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '|' && *iter == '|')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '<' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '>' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '+' && *iter == '+')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '-' && *iter == '-')
            {
                name += *iter;
                *iter++;
            }
            type = TokenType::OPERATOR;
            proc = Process::END;
        }
    }
    else
    {
        type = TokenType::OTHER;
        proc = Process::END;
    }
}

void Scanner::dealDelimiter(std::string& name, TokenType& type)
{
    name += *iter;
    iter++;
    type = TokenType::DELIMITER;
    proc = Process::END;
}

Token Scanner::dealEnd(const std::string& name, const TokenType& type)
{
    return Token(name, type, line);
}

void Scanner::dealChar(std::string& name, TokenType& type)
{
    if (*iter == '\'')
    {
        type = TokenType::CHAR;
        proc = Process::END;
    }
    name += *iter;
    iter++;
}

/********************编译预处理，取出无用的字符和注释**********************/
std::string Scanner::Preprocessing(std::string InputStr, int InputStrLen)
{
    //char tempString[MaxCodeLength];
    std::string tempString;
    for (int i = 0; i <= InputStrLen; i++)
    {
        if (InputStr[i] == '/' && InputStr[i + 1] == '/')
        {
            //若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (InputStr[i] != '\n')
            {
                i++;//向后扫描
            }
        }
        if (InputStr[i] == '/' && InputStr[i + 1] == '*')
        {
            //若为多行注释“/* 。。。*/”则去除该内容
            i += 2;
            while (InputStr[i] != '*' || InputStr[i + 1] != '/')
            {
                i++;//继续扫描
                if (InputStr[i] == '$')
                {
                    printf("注释出错，没有找到 */，程序结束！！！\n");
                    exit(0);
                }
            }
            i += 2;//跨过“*/”
        }


        if (InputStr[i] != '\v' && InputStr[i] != '\r')
        {
            //若出现无用字符，则过滤；否则加载
            //tempString[count++] = InputStr[i];
            tempString += InputStr[i];
        }
    }
    //tempString[count] = '\0';
    InputStr = tempString;
    return tempString;
    //strcpy(InputStr, tempString);//产生净化之后的源程序
}

bool Scanner::openFile(const std::string& FileName)
{
    std::ifstream in(FileName);
    proc = Process::BEGIN;
    this->FileName = FileName;
    code = "";
    line = 1;


    if (in)
    {
        code = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        iter = code.cbegin();

        return true;
    }
    else
    {
        std::cerr << FileName << "开启失败" << std::endl;
        return false;
    }
}

Token Scanner::getNextToken()
{
    proc = Process::BEGIN;
    std::string name;
    TokenType type = TokenType::OTHER;
    skipBlank();
    while ((iter != code.cend() && *iter != EOF) || proc == Process::END)
    {
        switch (proc)
        {
        case Process::BEGIN:
            dealBegin(name, type);
            break;
        case Process::IN_DELIMITER:
            dealDelimiter(name, type);
            break;
        case Process::IN_FLOAT:
            dealFloat(name, type);
            break;
        case Process::IN_ID:
            dealID(name, type);
            break;
        case Process::IN_INTEGER:
            dealInteger(name, type);
            break;
        case Process::IN_KEYWORD:
            dealKeyWord(name, type);
            break;
        case Process::IN_OPERATOR:
            dealOperator(name, type);
            break;
        case Process::IN_CHAR:
            dealChar(name, type);
            break;
        case Process::END:
            auto token = dealEnd(name, type);
            return token;

        }
    }
    return Token("", TokenType::OTHER, line);
}

std::vector<Token> Scanner::getTokens(const std::string FileName)
{
    std::vector<Token> tokens;
    if (openFile(FileName))
    {
        for (Token token = getNextToken(); token; token = getNextToken())
        {
            tokens.push_back(token);
        }
    }
    tokens.push_back(Token("$", TokenType::DELIMITER, line));
    return tokens;
}
