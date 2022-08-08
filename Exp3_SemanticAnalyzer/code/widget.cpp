#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <cstdio>
#include "scanner.h"
#include "parser.h"
#include "parsertest.h"
#include <fstream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPainter>

//#pragma warning(disable:4996)
using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("中间代码生成器");

    QObject::connect(ui->SourceProgramButton,SIGNAL(clicked(bool)),this,SLOT(readButtonSlot()));
    QObject::connect(ui->PreProcessingButton,SIGNAL(clicked(bool)),this,SLOT(PreprocessingButtonSlot()));
    QObject::connect(ui->LexResultsButton,SIGNAL(clicked(bool)),this,SLOT(LexicalResultsButtonSlot()));
    QObject::connect(ui->SynGrammarButton,SIGNAL(clicked(bool)),this,SLOT(SynGrammarButtonSlot()));
    QObject::connect(ui->FirstSetButton,SIGNAL(clicked(bool)),this,SLOT(FirstSetButtonSlot()));
    QObject::connect(ui->SysStackButton,SIGNAL(clicked(bool)),this,SLOT(SysStackButtonSlot()));
    QObject::connect(ui->AnalyzeButton,SIGNAL(clicked(bool)),this,SLOT(AnalyzeButtonSlot()));
    QObject::connect(ui->AnalyzeTestButton,SIGNAL(clicked(bool)),this,SLOT(AnalyzeTestButtonSlot()));
    QObject::connect(ui->QuaternionInformationButton,SIGNAL(clicked(bool)),this,SLOT(QuaternionInformationButtonSlot()));
}

void Widget::paintEvent(QPaintEvent *)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/picture/background1.jpg");
    //绘制背景图
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
}

void Widget::AnalyzeButtonSlot()
{
    Scanner scanner;
    auto tokens = scanner.getTokens("test_preprocessing.txt");
    Parser parser;
    parser.openFile("grammar.txt");
    parser.build();
    parser.analyse(tokens);

    //输出文件重定向
    ofstream fout;
    fout.open("FirstSet.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;
        exit(-1);
    }

    //以下输出终结符的first集
    std::cout << "first aggregate:" << std::endl;
    fout << "first aggregate:" << std::endl;
    for (auto i : parser.terminalSet)
    {
        std::set<std::string> iter = parser.getFirst(i);
        std::cout << "FIRST(" << i << ") = [";
        fout << "FIRST(" << i << ") = [";
        for (std::set<std::string >::iterator it = iter.begin(); it != iter.end(); ++it)
        {
            std::cout << '\'' << *it << '\'' << " ";
            fout << '\'' << *it << '\'' << " ";
        }

        fout << "]" << std::endl;
    }
    fout.close();
    ui->textEdit_2->setText("Analyze successfully!");
}

void Widget::AnalyzeTestButtonSlot()
{
    Scanner scanner;
    auto tokens = scanner.getTokens("test_preprocessing.txt");
    ParserTest parsertest;
    parsertest.openFile("grammar.txt");
    parsertest.build();
    parsertest.analyse(tokens);

    QFile file( "Analyze.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit_3->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void Widget::readButtonSlot()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("*.txt"));
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void Widget::PreprocessingButtonSlot()
{
    //输出文件重定向
    ofstream fout;
    fout.open("test_preprocessing.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;
        exit(-1);
    }

    //输入文件重定向
    ifstream infile;
    infile.open("test.txt", ios::in);
    if (infile.is_open() == 0)
    {
        cout << "input open failed" << endl;
    }

    std::string line;

    std::string inn;
    while (getline(infile, line))
    {
        inn += line + '\n';
    }

    cout << "source program:" << endl << inn << endl;
    Scanner scanner;
    inn = scanner.Preprocessing(inn, inn.size());

    fout << inn;

    cout << "after program preprocessing:" << endl << inn << endl;

    QString qstrinn = QString::fromStdString(inn);

    ui->textEdit->setText(qstrinn);

    fout.close();
    infile.close();
}

void Widget::LexicalResultsButtonSlot()
{
    Scanner scanner;

    auto tokens = scanner.getTokens("test_preprocessing.txt");

    //输出文件重定向
    ofstream fout;
    fout.open("Lexical_Results.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;
        exit(-1);
    }
    //cout << "Lexical results:" << endl;
    for (auto i : tokens)
        fout << i.getName() <<" " << i.getTypeOutput() << " " << i.getLine() << endl;
    fout.close();

    QFile file( "Lexical_Results.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void Widget::SynGrammarButtonSlot()
{
    QFile file( "grammar.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit_2->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void Widget::FirstSetButtonSlot()
{
    QFile file( "FirstSet.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit_2->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void Widget::SysStackButtonSlot()
{
    QFile file( "SysStack.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit_2->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void Widget::QuaternionInformationButtonSlot()
{
    QFile file( "Quaternion_information.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    ui->textEdit_4->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
}

void getToken()
{
    Scanner scanner;
    string FileName = "C:/Users/C.X/Desktop/Semantic_Analyzer/test_preprocessing.txt";

    auto tokens = scanner.getTokens(FileName);

    for (auto token : tokens)
    {
        cout << "    " << token.getName() << " " << TokenDict[token.getType()] << "  " << token.getLine() << endl;
    }
}

void getGrammar()
{
    Parser parser;
    if (parser.openFile("grammar.txt"))
        cout << "ok" << endl;
    else
    {
        cout << "oh no!" << endl;
        return;
    }
    parser.build();
    auto grammar = parser.getGrammar();
    auto closurelist = parser.getClosurelist();
    auto closuremap = parser.getClosuremap();
    for (auto lr1set : closurelist)
    {
        cout << "I" << closuremap[lr1set] << endl;
        for (auto lr1 : lr1set)
        {
            cout << "  ";
            auto id = lr1.getLeft().getLeft();
            auto point = lr1.getLeft().getRight();
            auto str = lr1.getRight();
            cout << grammar[id].getLeft() << "->";
            auto right = grammar[id].getRight();
            for (unsigned int i = 0; i < right.size(); i++)
            {
                if (i == point)
                    cout << ".";
                cout << right[i];
            }
            if (point == right.size())
                cout << ".";
            cout << "|" << str << endl;
        }
    }
    cout << endl;
    auto transfer = parser.getTransfer();
    for (unsigned int i = 0; i < transfer.size(); i++)
    {
        if (transfer[i].size())
        {
            cout << "from I" << i << ":" << endl;
            for (auto e : transfer[i])
            {
                cout << "  str=|" << e.first << "|  to  I" << e.second << endl;
            }
        }
    }
    cout << endl;

    cout << "\t";
    for (auto e : parser.getTerminalSet())
        cout << e << "\t";
    for (auto e : parser.getVariableSet())
        cout << e << "\t";
    cout << endl;

    auto action = parser.getAction();
    auto go = parser.getGo();
    for (unsigned int i = 0; i < action.size(); i++)
    {
        cout << "I" << i << "\t";

        for (auto e : parser.getTerminalSet())
        {
            for (auto one : action[i])
                if (one.first == e)
                    cout << one.second.first << one.second.second;
            cout << "\t";
        }
        for (auto e : parser.getVariableSet())
        {
            for (auto one : go[i])
                if (one.first == e)
                    cout << one.second << "\t";
            cout << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void analyse()
{
    Scanner scanner;

    auto tokens = scanner.getTokens("test_preprocessing.txt");

    for (auto i : tokens)
        cout << i.getName() <<" " << i.getTypeOutput() << " " << i.getLine() << endl;
    cout << endl;
    system("pause");
    Parser parser;
    parser.openFile("parser/grammar.txt");
    parser.build();
    parser.analyse(tokens);

    std::cout << "first集：" << endl;
    for (auto i : tokens)
    {
        std::set<std::string> iter = parser.getFirst(i.getName());
        std::cout << i.getName() << "：";
        for (std::set<std::string >::iterator it = iter.begin(); it != iter.end(); ++it)
            std::cout << *it << " ";
        std::cout << endl;
    }

}

Widget::~Widget()
{
    delete ui;
}
