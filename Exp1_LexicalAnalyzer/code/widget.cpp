#include "widget.h"
#include "ui_widget.h"

#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>
#include <QPushButton>
#include <string>

//QIODevice::NotOpen	未打开
//QIODevice::ReadOnly	以只读方式打开
//QIODevice::WriteOnly	以只写方式打开
//QIODevice::ReadWrite	以读写方式打开
//QIODevice::Append     以追加的方式打开，新增加的内容将被追加到文件末尾
//QIODevice::Truncate	以重写的方式打开，在写入新的数
//QIODevice::Text       在读取时，将行结束符转换成 \n；在写入时，将行结束符转换成本地格式，例如 Win32 平台上是 \r\n
//QIODevice::Unbuffered	忽略缓存

#define MaxCodeLength 655           //最大代码长度
#define WordMaxNum 256              //变量最大个数
#define DigitNum 256                //常量最大个数
#define MaxKeyWord 34               //关键字数量
#define MaxUnaryOptNum 7            //单目运算符最大个数
#define MaxBinaryOptNum 6           //双目运算符最大个数
#define MaxEndNum 11                //界符最大个数

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

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("词法分析器");

    QObject::connect(ui->writeButton,SIGNAL(clicked(bool)),this,SLOT(writeButtonSlot()));
    QObject::connect(ui->readButton ,SIGNAL(clicked(bool)),this,SLOT( readButtonSlot()));
}

//判断是否为数字
bool isNum(char ch)
{
    if (ch>='0' && ch<='9')
        return true;
    return false;
}

//判断是否为字母
bool isLetter(char ch)
{
    if(ch >= 'a' && ch <= 'z')
        return true;
    return false;
}

//错误处理
void error(char str[20], int nLine, int errorType)
{
    qDebug() << " \nError :    ";
    switch (errorType)
    {
    case VarExceed:
        qDebug() << "第" << nLine - 1 << "行" << str << " 变量的长度超过限制！\n";
        errorFlag = 1;
        break;
    case PointError:
        qDebug() << "第" << nLine - 1 << "行" << str << " 小数点错误！\n";
        errorFlag = 1;
        break;
    case ConExceed:
        qDebug() << "第" << nLine - 1 << "行" << str << " 常量的长度超过限制！\n";
        errorFlag = 1;
        break;
    }
}

/********************编译预处理，取出无用的字符和注释**********************/
void Preprocessing(char InputStr[], int InputStrLen)
{
    char tempString[MaxCodeLength];
    int count = 0;
    for (int i = 0; i <= InputStrLen; i++)
    {
        if (InputStr[i] == '/' && InputStr[i + 1] == '/')
        {//若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (InputStr[i] != '\n')
            {
                i++;//向后扫描
            }
        }
        if (InputStr[i] == '/' && InputStr[i + 1] == '*')
        {//若为多行注释“/* 。。。*/”则去除该内容
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
        if (InputStr[i] != '\n' && InputStr[i] != '\t' && InputStr[i] != '\v' && InputStr[i] != '\r')
        {//若出现无用字符，则过滤；否则加载
            tempString[count++] = InputStr[i];
        }
    }
    tempString[count] = '\0';
    strcpy(InputStr, tempString);//产生净化之后的源程序
}

void Scanner(char InputStr[], int InputStrLen, Table table[MaxCodeLength], int Line)
{
    int chIndex = 0;

    //对输入的字符扫描
    while (chIndex < InputStrLen)
    {
        /*处理空格和tab*/
        while (InputStr[chIndex] == ' ' || InputStr[chIndex] == 9) //忽略空格和tab
            chIndex++;

        /*处理换行符*/
        while (InputStr[chIndex] == 10) //遇到换行符，行数加1
        {
            Line++;
            chIndex++;
        }

        /*标识符*/
        if (isalpha(InputStr[chIndex])) //以字母、下划线开头
        {
            char str[256];
            int strLen = 0;
            while (isalpha(InputStr[chIndex]) || InputStr[chIndex] == '_') //是字母、下划线
            {
                str[strLen++] = InputStr[chIndex];
                chIndex++;
                while (isdigit(InputStr[chIndex])) //不是第一位，可以为数字
                {
                    str[strLen++] = InputStr[chIndex];
                    chIndex++;
                }
            }
            str[strLen] = 0;      //字符串结束符
            if (strlen(str) > 20) //标识符超过规定长度，报错处理
            {
                error(str, Line, 1);
            }
            else
            {
                int i;
                for (i = 0; i < MaxKeyWord; i++)      //与关键字匹配
                    if (strcmp(str, KeyWord[i]) == 0) //是关键字，写入table表中
                    {
                        strcpy(table[TableNum].symbol, str);
                        table[TableNum].type = i; //关键字
                        table[TableNum].line = Line;
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
                    table[TableNum].line = Line;
                    TableNum++;
                }
            }
        }

        /*常数*/
        // else if(isdigit(ch[chIndex])&&ch[chIndex]!='0') //遇到数字
        else if (isdigit(InputStr[chIndex])) //遇到数字
        {
            int flag = 0;
            char str[256];
            int strLen = 0;
            while (isdigit(InputStr[chIndex]) || InputStr[chIndex] == '.') //数字和小数点
            {
                if (InputStr[chIndex] == '.') // flag表记小数点的个数，0时为整数，1时为小数，2时出错
                    flag++;
                str[strLen++] = InputStr[chIndex];
                chIndex++;
            }

            str[strLen] = 0;

//            if (InputStr[chIndex] >= 'a' && InputStr[chIndex] <= 'z')
//            {
//                table[TableNum].type = -5; //整数
//                strcpy(table[TableNum].symbol, "too long");
//                table[TableNum].line = Line;
//                TableNum++;
//                chIndex++;
//            }


            if (strlen(str) > 20) //常量标识符超过规定长度20，报错处理
            {
                table[TableNum].type = -1; //出现错误

                strcpy(table[TableNum].symbol, "too long");
                table[TableNum].line = Line;
                TableNum++;
                //error(str, Line, 3);
            }
            else if (flag == 0)
            {
                table[TableNum].type = MaxKeyWord + 1; //整数
            }
            else if (flag == 1)
            {
                table[TableNum].type = MaxKeyWord + 2; //小数
            }
            else if (flag > 1)
            {
                table[TableNum].type = -2; //两个小数点出错

                strcpy(table[TableNum].symbol, "double . error");
                table[TableNum].line = Line;
                TableNum++;
                //error(str, Line, 2);
            }
            if (flag <= 1 && strlen(str) <= 20)
            {
                table[TableNum].Index = DigNum;
                strcpy(Digit[DigNum++], str);

                strcpy(table[TableNum].symbol, str);
                table[TableNum].line = Line;
                TableNum++;
            }
        }

        /*运算符*/
        else
        {
            int errorFlag; //用来区分是不是无法识别的标识符，0为运算符，1为界符

            char str[3];
            str[0] = InputStr[chIndex];
            str[1] = InputStr[chIndex + 1];
            str[2] = '\0';
            int i;
            for (i = 0; i < MaxBinaryOptNum; i++) // MaxOptBNum)
                if (strcmp(str, OptB[i]) == 0)
                {
                    errorFlag = 0;
                    table[TableNum].type = MaxKeyWord + 3 + i;
                    strcpy(table[TableNum].symbol, str);
                    table[TableNum].line = Line;
                    table[TableNum].Index = i;
                    TableNum++;
                    chIndex = chIndex + 2;
                    break;
                }
            if (i >= MaxBinaryOptNum)
            {
                for (int k = 0; k < MaxUnaryOptNum; k++)
                    if (OptA[k] == InputStr[chIndex])
                    {
                        errorFlag = 0;
                        table[TableNum].type = MaxKeyWord + 3 + MaxBinaryOptNum + k;
                        table[TableNum].symbol[0] = InputStr[chIndex];
                        table[TableNum].symbol[1] = 0;
                        table[TableNum].line = Line;
                        table[TableNum].Index = k;
                        TableNum++;
                        chIndex++;
                        break;
                    }

                /*界符*/
                for (int j = 0; j < MaxEndNum; j++)
                    if (End[j] == InputStr[chIndex])
                    {
                        errorFlag = 1;
                        table[TableNum].line = Line;
                        table[TableNum].symbol[0] = InputStr[chIndex];
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
                    str[strLen++] = InputStr[chIndex];
                    chIndex++;

                    while (*InputStr != ' ' || *InputStr != 9 || InputStr[chIndex] != 10) //
                    {
                        str[strLen++] = InputStr[chIndex];
                        chIndex++;
                    }
                    str[strLen] = 0;
                    table[TableNum].type = 1100000;
                    strcpy(table[TableNum].symbol, str);
                    table[TableNum].line = Line;
                    table[TableNum].Index = -2;
                    TableNum++;
                }
            }
        }
    }
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

void Widget::writeButtonSlot()
{
     QString writeData = ui->textEdit->toPlainText();

     char StringkeyTokeys[MaxCodeLength];
     //QString转char数组
     QByteArray ba = writeData.toLocal8Bit();
     memcpy(StringkeyTokeys, ba.data(), ba.size() + 1);
     int nLine = 1;                            //初始化行数
     Table *table = new Table[MaxCodeLength];
     qDebug() << StringkeyTokeys << endl;

     //编译预处理
     Preprocessing(StringkeyTokeys, strlen(StringkeyTokeys));

     ui->textEdit_3->setText(StringkeyTokeys);

     Scanner(StringkeyTokeys, strlen(StringkeyTokeys), table, nLine); //调用扫描函数

     QFile data("E:/Lexical_Analyzer/output.txt");
     if (data.open( QIODevice::Text | QFile::WriteOnly  ))
     {
         qDebug()<<"yes write";
         QTextStream out(&data);

         qDebug() << "数据量：" << TableNum << endl;
         /*把结果打印到各个txt文档中*/

         out << "type" << "      " << "index" << endl;
         for (int i = 0; i < TableNum; i++) //打印display
         {
             //打印关键字
             if (table[i].type >= 0 && table[i].type <= 5)
                 //out << "(" << table[i].type << "    ,   " << table[i].symbol << ")" << endl;
                 out << "(" << table[i].type << "    ,   -" << ")" << endl;
             //打印标识符、整数、小数
             else if (table[i].type == 34 || table[i].type == 35 || table[i].type == 36)
                 out << "(" << table[i].type << "    ,   " << "" << table[i].Index << ")" << endl;
             //打印运算符、界符
             else
                 //out << "(" << table[i].type << "    ,   " << table[i].symbol << ")" << endl;
                 out << "(" << table[i].type << "    ,   -" << ")" << endl;
         }
     }
     else
        qDebug()<<"no write";

     data.close();
     QFile file("E:/Lexical_Analyzer/output.txt");
     file.open(QIODevice::ReadOnly | QIODevice::Text);
     QByteArray t = file.readAll();
     QString str(t);
     ui->textEdit_2->setText(str);
     //设置文本框只读
     //ui->textEdit_2->setReadOnly(true);
     file.close();
}

Widget::~Widget()
{
    delete ui;
}
