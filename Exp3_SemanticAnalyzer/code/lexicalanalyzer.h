#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#define Max 655        //最大代码长度
#define MaxCodeLength 655           //最大代码长度
#define WordMaxNum 256              //变量最大个数
#define DigitNum 256                //常量最大个数
#define MaxKeyWord 34               //关键字数量
#define MaxUnaryOptNum 7            //单目运算符最大个数
#define MaxBinaryOptNum 6           //双目运算符最大个数
#define MaxEndNum 11                //界符最大个数

#include <iostream>

using namespace std;

enum errorType
{
    VarExceed = 1,
    PointError = 2,
    ConExceed = 3
};

typedef struct DisplayTable
{
    int Index;       //标识符所在表的下标
    int type;        //标识符的类型
    int line;        //标识符所在表的行数
    char symbol[20]; //标识符所在表的名称
} Table;

int TableNum = 0;           //display表的下标
char Word[WordMaxNum][20];  //标识符表
char Digit[WordMaxNum][20]; //数字表
int WordNum = 0;            //变量表的下标
int DigNum = 0;             //常量表的下标
bool errorFlag = 0;         //错误标志

const char *const KeyWord[MaxKeyWord] = {"int", "void", "if", "else", "while", "return",
                                         "signed", "char", "double", "unsigned", "const",
                                         "goto", "for", "float", "break", "class", "case",
                                         "do", "long", "typedef", "static", "friend",
                                         "new", "enum", "try", "short", "continue", "sizeof",
                                         "switch", "private", "catch", "delete", "public", "struct"};
const char OptA[] = {'+', '-', '*', '/', '=', '<', '>'}; // 单目运算
const char *OptB[] = {"<=", ">=", ":=", "<>", "==", "!="};                //双目运算符
const char End[] = {
    '(', ')', ',', ';', '.', '[',
    ']', ':', '{', '}', '"'}; // 界符

/*
0——33：关键字："int", "void", "if", "else", "while", "return"
34：标识符
35：整数
36：小数
37——42：双目运算符："<=", ">=", ":=", "<>", "==", "!="
43——49：单目运算符：'+', '-', '*', '/', '=', '<', '>'
50——60：'(', ')', ',', ';', '.', '[', ']', ':', '{', '}', '"'
*/

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();
    void Scanner(char ch[], int chLen, Table table[Max], int nLine);
    void error(char str[20], int nLine, int errorType);
};

#endif // LEXICALANALYZER_H
