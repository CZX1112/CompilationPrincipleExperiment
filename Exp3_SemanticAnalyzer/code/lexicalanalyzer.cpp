#include "lexicalanalyzer.h"
#include <cstring>

LexicalAnalyzer::LexicalAnalyzer()
{

}

void LexicalAnalyzer::error(char str[20], int nLine, int errorType)
{
    cout << " \nError :    ";
    switch (errorType)
    {
    case VarExceed:
        cout << "第" << nLine - 1 << "行" << str << " 变量的长度超过限制！\n";
        errorFlag = 1;
        break;
    case PointError:
        cout << "第" << nLine - 1 << "行" << str << " 小数点错误！\n";
        errorFlag = 1;
        break;
    case ConExceed:
        cout << "第" << nLine - 1 << "行" << str << " 常量的长度超过限制！\n";
        errorFlag = 1;
        break;
    }
}

void LexicalAnalyzer::Scanner(char ch[], int chLen, Table table[Max], int nLine)
{
    int chIndex = 0;

    while (chIndex < chLen) //对输入的字符扫描
    {
        /*处理空格和tab*/
        while (ch[chIndex] == ' ' || ch[chIndex] == 9) //忽略空格和tab
        {
            chIndex++;
        }
        /*处理换行符*/
        while (ch[chIndex] == 10) //遇到换行符，行数加1
        {
            nLine++;
            chIndex++;
        }

        /*标识符*/
        if (isalpha(ch[chIndex])) //以字母、下划线开头
        {
            char str[256];
            int strLen = 0;
            while (isalpha(ch[chIndex]) || ch[chIndex] == '_') //是字母、下划线
            {
                str[strLen++] = ch[chIndex];
                chIndex++;
                while (isdigit(ch[chIndex])) //不是第一位，可以为数字
                {
                    str[strLen++] = ch[chIndex];
                    chIndex++;
                }
            }
            str[strLen] = 0;      //字符串结束符
            if (strlen(str) > 20) //标识符超过规定长度，报错处理
            {
                error(str, nLine, 1);
            }
            else
            {
                int i;
                for (i = 0; i < MaxKeyWord; i++)      //与关键字匹配
                    if (strcmp(str, KeyWord[i]) == 0) //是关键字，写入table表中
                    {
                        strcpy(table[TableNum].symbol, str);
                        table[TableNum].type = i; //关键字
                        table[TableNum].line = nLine;
                        table[TableNum].Index = i;
                        TableNum++;
                        break;
                    }
                if (i >= MaxKeyWord) //不是关键字
                {

                    table[TableNum].Index = WordNum;
                    strcpy(Word[WordNum++], str);
                    table[TableNum].type = MaxKeyWord; //变量标识符
                    strcpy(table[TableNum].symbol, str);
                    table[TableNum].line = nLine;
                    TableNum++;
                }
            }
        }

        /*常数*/
        // else if(isdigit(ch[chIndex])&&ch[chIndex]!='0') //遇到数字
        else if (isdigit(ch[chIndex])) //遇到数字
        {
            int flag = 0;
            char str[256];
            int strLen = 0;
            while (isdigit(ch[chIndex]) || ch[chIndex] == '.') //数字和小数点
            {
                if (ch[chIndex] == '.') // flag表记小数点的个数，0时为整数，1时为小数，2时出错
                    flag++;
                str[strLen++] = ch[chIndex];
                chIndex++;
            }
            str[strLen] = 0;
            if (strlen(str) > 20) //常量标识符超过规定长度20，报错处理
            {
                error(str, nLine, 3);
            }
            if (flag == 0)
            {
                table[TableNum].type = MaxKeyWord + 1; //整数
            }
            if (flag == 1)
            {
                table[TableNum].type = MaxKeyWord + 2; //小数
            }
            if (flag > 1)
            {
                error(str, nLine, 2);
            }
            table[TableNum].Index = DigNum;
            strcpy(Digit[DigNum++], str);

            strcpy(table[TableNum].symbol, str);
            table[TableNum].line = nLine;
            TableNum++;
        }

        /*运算符*/
        else
        {
            int errorFlag; //用来区分是不是无法识别的标识符，0为运算符，1为界符

            char str[3];
            str[0] = ch[chIndex];
            str[1] = ch[chIndex + 1];
            str[2] = '\0';
            int i;
            for (i = 0; i < MaxBinaryOptNum; i++) // MaxOptBNum)
                if (strcmp(str, OptB[i]) == 0)
                {
                    errorFlag = 0;
                    table[TableNum].type = MaxKeyWord + 3 + i;
                    strcpy(table[TableNum].symbol, str);
                    table[TableNum].line = nLine;
                    table[TableNum].Index = i;
                    TableNum++;
                    chIndex = chIndex + 2;
                    break;
                }
            if (i >= MaxBinaryOptNum)
            {
                for (int k = 0; k < MaxUnaryOptNum; k++)
                    if (OptA[k] == ch[chIndex])
                    {
                        errorFlag = 0;
                        table[TableNum].type = MaxKeyWord + 3 + MaxBinaryOptNum + k;
                        table[TableNum].symbol[0] = ch[chIndex];
                        table[TableNum].symbol[1] = 0;
                        table[TableNum].line = nLine;
                        table[TableNum].Index = k;
                        TableNum++;
                        chIndex++;
                        break;
                    }

                /*界符*/
                for (int j = 0; j < MaxEndNum; j++)
                    if (End[j] == ch[chIndex])
                    {
                        errorFlag = 1;
                        table[TableNum].line = nLine;
                        table[TableNum].symbol[0] = ch[chIndex];
                        table[TableNum].symbol[1] = 0;
                        table[TableNum].Index = j;
                        table[TableNum].type = MaxKeyWord + 3 + MaxBinaryOptNum + MaxUnaryOptNum + j;
                        TableNum++;
                        chIndex++;
                    }
                /*其他无法识别字符*/
                if (errorFlag != 0 && errorFlag != 1) //开头的不是字母、数字、运算符、界符
                {
                    char str[256];
                    int strLen = -1;
                    str[strLen++] = ch[chIndex];
                    chIndex++;

                    while (*ch != ' ' || *ch != 9 || ch[chIndex] != 10) //
                    {
                        str[strLen++] = ch[chIndex];
                        chIndex++;
                    }
                    str[strLen] = 0;
                    table[TableNum].type = 1100000;
                    strcpy(table[TableNum].symbol, str);
                    table[TableNum].line = nLine;
                    table[TableNum].Index = -2;
                    TableNum++;
                }
            }
        }
    }
}
