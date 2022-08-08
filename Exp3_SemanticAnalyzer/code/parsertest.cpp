#include "parsertest.h"
#include "production.h"
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <queue>
#include <stack>
#include <sstream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#define test

#include <iostream>
using namespace std;

std::stack<std::string> strStackTest;
std::stack<std::string> varStackTest;
int tempNumTest;
int labelNumTest;

ofstream fout;

std::string itoTempTest(int i)
{
    std::ostringstream temp;
    temp << i << "_TEMP";
    return temp.str();
}

std::string itoLabelTest(int i)
{
    std::ostringstream temp;
    temp << i << "_LABEL";
    return temp.str();
}

std::string varStackPopTest()
{
    auto var = varStackTest.top();
    varStackTest.pop();
    return var;
}

std::string strStackPopTest()
{
    auto str = strStackTest.top();
    strStackTest.pop();
    return str;
}

std::string cmdTest(std::string a, std::string b, std::string c, std::string d)
{
    return  "(" + a + ", " + b + ", " + c + ", " + d + ")";
}

int ParserTest::translate(int id, std::string name)
{
#ifdef test
    cout << "[translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    cout << "    strSize=[" << strStackTest.size() << "]" << endl;

    fout << "[translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    fout << "    strSize=[" << strStackTest.size() << "]" << endl;
#endif // test
    switch (id)
    {
    case 3:
    {
        varStackPopTest();
        varStackPopTest();
#ifdef test
        cout << "[case 3] [var pop]" << endl;
        cout << "[case 3] [var pop]" << endl;

        fout << "[case 3] [var pop]" << endl;
        fout << "[case 3] [var pop]" << endl;
#endif // test
        break;
    }

    case 9: case 10:
    {
        auto str = strStackPopTest();
        std::ostringstream os;
        os << std::endl << str;
        strStackTest.push(os.str());
        break;
    }

    case 13:
    {
        varStackPopTest();
#ifdef test
        cout << "[case 13] [var pop]" << endl;
        fout << "[case 13] [var pop]" << endl;
#endif // test
        break;
    }

    case 15:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        strStackTest.push(os.str());
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        varStackTest.push(lvar);
#ifdef test
        cout << "[case 15] [str pop] " << endl;
        cout << "[case 15] [str pop] " << endl;
        cout << "[case 15] [str push] " << os.str() << endl;

        fout << "[case 15] [str pop] " << endl;
        fout << "[case 15] [str pop] " << endl;
        fout << "[case 15] [str push] " << os.str() << endl;
#endif // test
        break;
    }

    case 24:
    {
        auto r = varStackPopTest();
        auto l = varStackPopTest();
        std::ostringstream os;
        os << strStackPopTest();
        os << cmdTest("=", r, "", l) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(l);
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 27:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("or", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 29:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("and", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 31:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("==", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 32:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("!=", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 34:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("<", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 35:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest(">", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 36:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("<=", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 37:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest(">=", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 39:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("+", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 40:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("-", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 42:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("*", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 43:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmdTest("/", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 44: case 45: case 46: case 47:
    {
        strStackTest.push("");
#ifdef test
        cout << "[case" << id << "] [str push] []" << endl;

        fout << "[case" << id << "] [str push] []" << endl;
#endif // test
        break;
    }

    case 49:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();

        os << cmdTest(itoLabelTest(labelNumTest), "", "", "") << std::endl;
        os << lstr;
        os << cmdTest("J!=", lvar, "0", itoLabelTest(labelNumTest + 1)) << std::endl;
        os << cmdTest("J", "", "", itoLabelTest(labelNumTest + 2)) << std::endl;
        os << cmdTest(itoLabelTest(labelNumTest + 1), "", "", "") << std::endl;
        os << rstr;
        os << cmdTest("J", "", "", itoLabelTest(labelNumTest)) << std::endl;
        os << cmdTest(itoLabelTest(labelNumTest + 2), "", "", "") << std::endl;
        labelNumTest += 3;
        strStackTest.push(os.str());
        varStackTest.push(lvar);
        break;
    }

    case 52:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << lstr;
        os << cmdTest("J!=", lvar, "0", itoLabelTest(labelNumTest)) << std::endl;
        os << cmdTest("J", "", "", itoLabelTest(labelNumTest + 1)) << std::endl;
        os << cmdTest(itoLabelTest(labelNumTest), "", "", "") << std::endl;;
        os << rstr;
        os << cmdTest(itoLabelTest(labelNumTest + 1), "", "", "") << std::endl;
        labelNumTest += 2;
        strStackTest.push(os.str());
        varStackTest.push(lvar);
        break;
    }

    case 53:
    {
        auto rstr = strStackPopTest();
        auto midstr = strStackPopTest();
        auto lstr = strStackPopTest();

        std::ostringstream os;
        auto rvar = varStackPopTest();
        auto midvar = varStackPopTest();
        auto lvar = varStackPopTest();

        os << lstr;
        os << cmdTest("J!=", lvar, "0", itoLabelTest(labelNumTest)) << std::endl;
        os << cmdTest("J", "", "", itoLabelTest(labelNumTest + 1)) << std::endl;
        os << cmdTest(itoLabelTest(labelNumTest), "", "", "") << std::endl;;
        os << midstr;
        os << cmdTest("J", "", "", itoLabelTest(labelNumTest + 2)) << std::endl;
        os << cmdTest(itoLabelTest(labelNumTest + 1), "", "", "") << std::endl;
        os << rstr;
        os << cmdTest(itoLabelTest(labelNumTest + 2), "", "", "") << std::endl;
        labelNumTest += 3;
        strStackTest.push(os.str());
        varStackTest.push(lvar);
        break;
    }

    default:
        break;
    }
#ifdef test
    cout << "[/translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    cout << "    strSize=[" << strStackTest.size() << "]" << endl;
    cout << endl;

    fout << "[/translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    fout << "    strSize=[" << strStackTest.size() << "]" << endl;
    fout << endl;
#endif // test
    return 1;
}

// analyze the tokens with grammar
int ParserTest::analyse(const std::vector<Token>& tokens)
{
    tempNumTest = 0;
    labelNumTest = 0;
    build();

    std::cout << std::endl;

    fout << std::endl;

    fout.open("Analyze.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;

        fout << "output open failed" << endl;
        exit(-1);
    }

    std::stack<std::pair<unsigned int, std::string>> st;
    st.push({ 0,"$" });
    auto iter = tokens.cbegin();

    for (; ; )
    {
        auto I = st.top().first;
        std::string type;
        if (iter->getType() == TokenType::ID || iter->getType() == TokenType::CHAR
                || iter->getType() == TokenType::INT || iter->getType() == TokenType::FLOAT)
            type = TokenDict[iter->getType()];
        else
            type = iter->getName();

        if (action[I].find(type) != action[I].end())
        {
            auto act = action[I][type];
            if (act.first == "S")
            {
                if (iter->getType() == TokenType::ID || iter->getType() == TokenType::CHAR
                        || iter->getType() == TokenType::INT || iter->getType() == TokenType::FLOAT)
                {
                    varStackTest.push(iter->getName());
                    //cout<<"[pushVAR] "<<iter->getName() <<"   [at line "<<iter->getLine()<<"]"<<endl;
                }
                st.push({ act.second, type });

                //在这里输出程序运行中栈的信息
                //                std::cout << "Stack Information:" << std::endl;
                //                for (std::stack<std::pair<unsigned int, std::string>> dump = st; !dump.empty(); dump.pop())
                //                    std::cout << dump.top().first << " " << dump.top().second << '\n';
                //                std::cout << std::endl;

                //system("pause");

                iter++;
                //                //在这里输出程序运行中插入token的信息
                //                std::cout << "Inserted element:" << std::endl;
                //                std::cout << iter->getName() << " " << iter->getTypeOutput() << std::endl;
            }
            else if (act.first == "r")
            {
                auto id = act.second;
                auto right = grammar[id].getRight();
                for (unsigned int i = 0; i < right.size(); i++)
                    if (right[i] != "@")
                        st.pop();
                auto newI = st.top().first;
                st.push({ go[newI][grammar[id].getLeft()],grammar[id].getLeft() });

                // translate
#ifdef test
                cout<<"use production["<<id<<"]: "<<grammar[id].getLeft()<<"->";
                for(auto e:right)cout<<e<<" ";
                cout<<endl;

                fout<<"use production["<<id<<"]: "<<grammar[id].getLeft()<<"->";
                for(auto e:right)fout<<e<<" ";
                fout<<endl;
#endif // test
                if (!translate(id, iter->getName()))
                {
                    std::cout << "ERROR! at line " << iter->getLine() << std::endl;
                    fout << "ERROR! at line " << iter->getLine() << std::endl;
                    return iter->getLine();
                }
            }

            else if (act.first == "acc")
            {

                ofstream fout;
                fout.open("Quaternion_information.txt", ios::out);
                if (fout.is_open() == 0)
                {
                    cout << "output open failed" << endl;
                    fout << "output open failed" << endl;
                    exit(-1);
                }

                std::cout << "Accept!" << std::endl;
                fout << "Accept!" << std::endl;

                //在这里输出最后的四元式
                std::cout << "Quaternion information:" << std::endl;
                std::cout << strStackTest.top() << std::endl;

                fout << "Quaternion information:" << std::endl;
                fout << strStackTest.top() << std::endl;

                fout.close();

                return 0;
            }
        }

        else
        {
            std::cout << "ERROR! at line " << iter->getLine() << std::endl;
            fout << "ERROR! at line " << iter->getLine() << std::endl;
            return iter->getLine();
        }
    }
    return 0;
}

// get one symbol's first set
std::set<std::string> ParserTest::getFirst(const std::string& symbol)
{
    if (firstMap.find(symbol) != firstMap.end())         // have this set, then return
        return firstMap[symbol];
    std::set<std::string> ret;
    if (terminalSet.find(symbol) != terminalSet.end())   // is a terminal symbol
    {
        ret.insert(symbol);
        return ret;
    }
    else                                                // is a variable symbol
    {
        for (auto production : grammar)
        {
            if (production.getLeft() == symbol)
            {
                auto right = production.getRight();
                if (right.size() == 1 && right[0] == "@")  //insert @
                {
                    ret.insert("@");
                    continue;
                }
                else if (right.size() > 0                //insert the first terminal symbol
                         && terminalSet.find(right[0]) != terminalSet.end())
                {
                    ret.insert(right[0]);
                    continue;
                }
            }
        }
    }
    return ret;
}

// build the first Map, firstMap[x] = first(x)
void ParserTest::buildFirstMap()
{
    firstMap.clear();

    // basic
    for (auto symbol : terminalSet)
        firstMap.insert({ symbol, getFirst(symbol) });
    for (auto symbol : variableSet)
        firstMap.insert({ symbol, getFirst(symbol) });

    // merge
    for (; ; )
    {
        bool changed = false;
        for (auto symbol : variableSet)
        {
            //if(symbol == "ADD")cout<<"get first (add) "<<endl;
            for (;;)
            {
                auto temp = firstMap[symbol];
                //if(symbol == "ADD"){cout<<"first= ";for(auto e:temp)cout<<e<<" ";cout<<endl;}
                for (auto produciton : grammar)
                {
                    if (produciton.getLeft() == symbol)
                    {
                        bool allEmpty = true;
                        auto right = produciton.getRight();
                        //if(symbol == "ADD"){cout<<"  right= ";for(auto e:right)cout<<e<<" ";cout<<endl;}
                        for (auto one : right)
                        {
                            auto oneSet = firstMap[one];
                            //if(symbol == "ADD"){cout<<"  oneset= ";for(auto e:oneSet)cout<<e<<" ";cout<<endl;}
                            if (oneSet.erase("@"))
                            {
                                temp.insert(oneSet.begin(), oneSet.end());
                            }
                            else
                            {
                                temp.insert(oneSet.begin(), oneSet.end());
                                allEmpty = false;
                                break;
                            }
                        }
                        if (allEmpty)
                            temp.insert("@");
                    }
                }
                //if(symbol == "ADD"){cout<<" now tot = ";for(auto e:temp)cout<<e<<" ";cout<<endl;}
                if (temp == firstMap[symbol])
                    break;
                firstMap[symbol] = temp;
                changed = true;
            }
        }
        if (!changed)
            break;
    }
}

// get first(beta), beta = A1A2...An
std::set<std::string> ParserTest::getFirst(const std::vector<std::string>& beta)
{
    std::set<std::string> ret;
    bool allEmpty = true;
    for (auto one : beta)
    {
        auto oneSet = firstMap[one];
        if (oneSet.erase("@"))
        {
            ret.insert(oneSet.begin(), oneSet.end());
        }
        else
        {
            ret.insert(oneSet.begin(), oneSet.end());
            allEmpty = false;
            break;
        }
    }
    if (allEmpty)
        ret.insert("@");
    return ret;
}

// build action and goto table
void ParserTest::build()
{
    if (closuremap.size() != 0)                      // have built action and goto table
        return;

    buildFirstMap();

    auto closure0 = getClosure(LR1item(LR0item(0, 0), "$"));

    std::queue<LR1set> q;
    q.push(closure0);

    //closuremap[closure0] = closuremap.size();     // first insert closure, then map[closure] = 1

    closurelist.push_back(closure0);
    closuremap.insert(make_pair(closure0, closuremap.size()));


    while (!q.empty())
    {
        auto lr1set = q.front();
        q.pop();
        transfer.push_back(vpsi());

        for (auto variable : variableSet)
        {
            LR1set newset;
            for (auto lr1 : lr1set)
            {
                auto lr0 = lr1.getLeft();
                auto id = lr0.getLeft();
                auto pointpos = lr0.getRight();
                if (grammar[id].getRight().size() > pointpos && grammar[id].getRight()[pointpos] == variable)
                    newset.insert(LR1item(LR0item(id, pointpos + 1), lr1.getRight()));
            }
            if (newset.size() != 0)
            {
                GetClosure(newset);
                if (closuremap.find(newset) == closuremap.end())
                {
                    closurelist.push_back(newset);
                    closuremap.insert(make_pair(newset, closuremap.size()));
                    //closuremap[newset]=closuremap.size();
                    q.push(newset);
                }
                transfer[transfer.size() - 1].push_back(make_pair(variable, closuremap[newset]));
            }
        }

        for (auto terminal : terminalSet)
        {
            LR1set newset;
            for (auto lr1 : lr1set)
            {
                auto lr0 = lr1.getLeft();
                auto id = lr0.getLeft();
                auto pointpos = lr0.getRight();
                if (grammar[id].getRight().size() != pointpos && grammar[id].getRight()[pointpos] == terminal)
                    newset.insert(LR1item(LR0item(id, pointpos + 1), lr1.getRight()));
            }
            if (newset.size() != 0)
            {
                GetClosure(newset);
                if (closuremap.find(newset) == closuremap.end())
                {
                    closurelist.push_back(newset);
                    closuremap.insert(make_pair(newset, closuremap.size()));
                    //closuremap[newset]=closuremap.size();
                    q.push(newset);
                }
                transfer[transfer.size() - 1].push_back(std::make_pair(terminal, closuremap[newset]));
            }
        }
    }

    for (unsigned int i = 0; i < closurelist.size(); i++)
    {
        action.push_back(std::map<std::string, psi>());
        go.push_back(std::map<std::string, int>());

        for (auto lr1 : closurelist[i])
        {
            auto point = lr1.getLeft().getRight();
            auto right = grammar[lr1.getLeft().getLeft()].getRight();
            if (point < right.size() && terminalSet.find(right[point]) != terminalSet.end())
            {
                for (auto go : transfer[i])
                    if (go.first == right[point])
                    {
#ifdef test
                        if (action[i].find(right[point]) != action[i].end()
                                && action[i][right[point]] != pair<string, int>("S", go.second))
                        {
                            cout << "error 1 at action[" << i << "][" << right[point] << "]=S" << go.second;
                            cout << "  old=" << action[i][right[point]].first << action[i][right[point]].second << endl;

                            fout << "error 1 at action[" << i << "][" << right[point] << "]=S" << go.second;
                            fout << "  old=" << action[i][right[point]].first << action[i][right[point]].second << endl;
                        }
#endif // test
                        action[i][right[point]] = std::make_pair("S", go.second);
                    }
            }
        }
        for (auto tf : transfer[i])
        {
            if (variableSet.find(tf.first) != variableSet.end())
            {
#ifdef test
                if (go[i].find(tf.first) != go[i].end())
                {
                    cout << "error at go[" << i << "][" << tf.first << "]" << endl;
                    fout << "error at go[" << i << "][" << tf.first << "]" << endl;
                }
#endif // test
                go[i][tf.first] = tf.second;

            }
        }
        for (auto lr1 : closurelist[i])
        {
            auto lookahead = lr1.getRight();
            auto lr0 = lr1.getLeft();
            auto id = lr0.getLeft();
            auto point = lr0.getRight();
            auto right = grammar[id].getRight();

            if (point == right.size())
            {
#ifdef test
                if (action[i].find(lookahead) != action[i].end()
                        && action[i][lookahead] != pair<string, int>("r", id))
                {
                    cout << "error 2 at action[" << i << "][" << lookahead << "]=r" << id << " old=" << action[i][lookahead].first << action[i][lookahead].second << std::endl;
                    fout << "error 2 at action[" << i << "][" << lookahead << "]=r" << id << " old=" << action[i][lookahead].first << action[i][lookahead].second << std::endl;
                }
#endif
                // deal with if else / if
                if (!(action[i].find(lookahead) != action[i].end()
                      && action[i][lookahead] != std::pair<std::string, int>("r", id)))
                    action[i][lookahead] = std::make_pair("r", id);

                if (lookahead == "$" && id == 0)
                {
                    action[i][lookahead] = std::make_pair("acc", 0);
                }
            }
        }
    }
}

// open grammar.txt, read grammar
bool ParserTest::openFile(const std::string& fileName)
{
    std::ifstream in(fileName);

    if (in)
    {
        std::string str;
        int type = 0;
        while (in >> str)
        {
            if (str == "[terminal]")
            {
                type = 1;
                continue;
            }
            else if (str == "[variable]")
            {
                type = 2;
                continue;
            }
            else if (str == "[production]")
                break;
            else
            {
                if (type == 1)
                    terminalSet.insert(str);
                else if (type == 2)
                    variableSet.insert(str);
            }
        }

        while (getline(in, str))
        {
            if (str.empty())
                continue;
            std::string temp;
            std::istringstream iss = std::istringstream(str);
            Production production;
            iss >> temp;
            iss >> temp;
            production.setLeft(temp);
            while (iss >> temp)
            {
                if (temp == "->")
                    continue;
                production.addRight(temp);
            }
#ifdef test
            /*
            cout<<production.getLeft()<<"->";
            for(auto e:production.getRight())
              cout<<e<<" ";
            cout<<endl;
            */
#endif // test
            grammar.push_back(production);
        }
        return true;
    }
    else
    {
        std::cerr << fileName << "开启失败" << std::endl;
        return false;
    }
}

// get grammar
std::vector<Production> ParserTest::getGrammar()
{
    return grammar;
}

void ParserTest::GetClosure(LR1set& closure)
{
    std::set<LR1item> temp;

    for (;;)
    {
        temp = closure;

        for (auto lr1 : closure)
        {
            LR0item lr0 = lr1.getLeft();
            std::string lookahead = lr1.getRight();

            unsigned int pointPos = lr0.getRight();
            auto right = grammar[lr0.getLeft()].getRight();

            if (pointPos != right.size() && variableSet.find(right[pointPos]) != variableSet.end())
            {
                std::string B = right[pointPos];

                std::vector<std::string> beta;
                for (unsigned int i = pointPos + 1; i < right.size(); i++)
                    beta.push_back(right[i]);
                beta.push_back(lookahead);

                auto first = getFirst(beta);

                for (unsigned int productionID = 0; productionID < grammar.size(); productionID++)
                {
                    const Production& production = grammar[productionID];
                    if (production.getLeft() == B)
                    {
                        for (auto terminal : first)
                        {
                            LR1item newItem;
                            if (production.getRight().size() == 1 && production.getRight()[0] == "@")
                                newItem = LR1item(LR0item(productionID, 1), terminal);
                            else
                                newItem = LR1item(LR0item(productionID, 0), terminal);
                            if (closure.find(newItem) == closure.end())
                                temp.insert(newItem);
                        }
                    }
                }
            }
        }
        if (temp == closure)
            break;
        else
            closure = temp;
    }
}
// get a closure set with LR1item item
std::set<LR1item> ParserTest::getClosure(const LR1item& item)
{
    std::set<LR1item> closure = { item };
    GetClosure(closure);
    return closure;
}

// get the closure


// get closure list
std::vector< std::set<LR1item> > ParserTest::getClosurelist()
{
    return closurelist;
}

// get closure map
std::map< std::set<LR1item>, int> ParserTest::getClosuremap()
{
    return closuremap;
}

// get transfer
std::vector< vpsi > ParserTest::getTransfer()
{
    return transfer;
}

// get action table
std::vector< std::map<std::string, psi>> ParserTest::getAction()
{
    return action;
}

// get goto table
std::vector< std::map<std::string, int>> ParserTest::getGo()
{
    return go;
}

// get variable symbol set
std::set< std::string > ParserTest::getVariableSet()
{
    return variableSet;
}

// get terminal symbol set
std::set< std::string > ParserTest::getTerminalSet()
{
    return terminalSet;
}
