from SynAnalyze import SynAnalyze
from LexAnalyze import LexAnalyze
import tkinter as tk
from tkinter.filedialog import askdirectory
from tkinter import StringVar
from tkinter import *
from tkinter import scrolledtext
from pandas import read_csv
from tkinter import ttk
from tkinter import messagebox
import csv
from tkinter import filedialog

#向分析表中插入数据
def insert(table, result):
    # 插入数据
    for index, data in enumerate(result):
        if index != 0:
            table.insert('', END, values=data)  # 添加数据到末尾

def selectPath():
    var=StringVar()
    scr = scrolledtext.ScrolledText(window, width=30, height=10, font=("隶书",18), bd =5)
    scr.place(x=0,y=50)

    path_ = askdirectory()
    print(path_)
    path_=path_.replace('\\','/')+'/LexAnaResult.txt'
    f = open(path_,'r',encoding = 'utf-8')
    for line in f:
        scr.insert('end',line)
    print(f.read())

    with open('ActionGoto.csv', 'r') as f:
        reader = csv.reader(f)
        result = list(reader)
        print(result[0])

    columns = result[0]

    screenwidth = window.winfo_screenwidth()  # 屏幕宽度
    screenheight = window.winfo_screenheight()  # 屏幕高度
    width = 1000
    height = 500
    x = int((screenwidth - width) / 2)
    y = int((screenheight - height) / 2)
    window.geometry('{}x{}+{}+{}'.format(width, height, x, y))  # 大小以及位置

    tabel_frame = tk.Frame(window)
    tabel_frame.pack()

    xscroll = Scrollbar(tabel_frame, orient=HORIZONTAL)
    yscroll = Scrollbar(tabel_frame, orient=VERTICAL)

    #columns = ['学号', '姓名', '性别', '出生年月', '籍贯', '班级', '班级', '班级', '班级', '班级', '班级']
    table = ttk.Treeview(
            master=tabel_frame,  # 父容器
            height=10,  # 表格显示的行数,height行
            columns=columns,  # 显示的列
            show='headings',  # 隐藏首列
            xscrollcommand=xscroll.set,  # x轴滚动条
            yscrollcommand=yscroll.set,  # y轴滚动条
            )
    for column in columns:
        table.heading(column=column, text=column, anchor=CENTER,
                    command=lambda name=column:
                    messagebox.showinfo('', '{}描述信息~~~'.format(name)))  # 定义表头
    table.column(column=column, width=100, minwidth=100, anchor=CENTER, )  # 定义列
    xscroll.config(command=table.xview)
    xscroll.pack(side=BOTTOM, fill=X)
    yscroll.config(command=table.yview)
    yscroll.pack(side=RIGHT, fill=Y)
    table.pack(fill=BOTH, expand=True)

    insert(table, result)

    btn_frame = Frame()
    btn_frame.pack()
    Button(btn_frame, text='添加', bg='yellow', width=20, command=insert).pack()

# 界面显示源程序
def SourceProGet():
    LexScr1.delete(1.0, END)
    source_path = filedialog.askopenfilename()
    sourcefile = open(source_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        LexScr1.insert('end',line)

# 执行词法分析程序
def LexAnalyzeFunc():
    LexGrammar_path = './LexGra.txt'  # 词法规则文件相对路径
    TokenTable_path = './LexAnaResult.txt'  # 存储TOKEN表的相对路径

    lex_ana = LexAnalyze()
    lex_ana.readLexGrammar(LexGrammar_path)
    lex_ana.createNFA()
    lex_ana.createDFA()

    codelist = lex_ana.Preprocessing(LexScr1.get(1.0,'end'))
    lex_ana.analyze(codelist, TokenTable_path)

# 界面显示词法分析规则
def LexRuleDisplay():
    LexScr1.delete(1.0, END)
    LexGrammar_path = './LexGra.txt'  # 词法规则文件相对路径
    sourcefile = open(LexGrammar_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        LexScr1.insert('end',line)

# 界面显示词法分析结果
def LexResult():
    LexScr2.delete(1.0, END)
    TokenTable_path = './LexAnaResult.txt'  # 存储TOKEN表的相对路径
    sourcefile = open(TokenTable_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        LexScr2.insert('end',line)

# 执行语法分析程序
def SynAnalyzeFunc():
    SynGrammar_path = './SynGra.txt'  # 语法规则文件相对路径
    TokenTable_path = './LexAnaResult.txt'  # 存储TOKEN表的相对路径
    LRTable_path = './ActionGoto.csv'  # 存储LR表的相对路径
    FirstSets_path = './FirstSets.txt'

    syn_ana = SynAnalyze()
    syn_ana.readSynGrammar(SynGrammar_path)
    syn_ana.getTerminatorsAndNon()
    syn_ana.getFirstSets()
    sourcefile = open(FirstSets_path,'w',encoding = 'utf-8')
    result = ''
    for key,value in syn_ana.firstSet.items():
        result = result + 'FIRST(' + key + ')' + '=' + str(value) + '\n'
    sourcefile.write(result)
    syn_ana.createLRTable(LRTable_path)
    sourcefile.close()
    syn_ana.analyze(TokenTable_path,SynAnalyzeProcess_path="./StackInfo.txt")

# 界面显示语法分析规则
def SynRuleDisplay():
    SynScr1.delete(1.0, END)
    SynGrammar_path = './SynGra.txt'  # 词法规则文件相对路径
    sourcefile = open(SynGrammar_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        SynScr1.insert('end',line)

# 界面显示语法分析FIRST集合
def SynFIRSTDisplay():
    SynScr1.delete(1.0, END)
    FirstSets_path = './FirstSets.txt'
    sourcefile = open(FirstSets_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        SynScr1.insert('end',line)

# 界面显示语法分析界csv分析表
def csvdisplay():
    windowson = tk.Tk()
    windowson.title('语法分析表')
    windowson.geometry('1000x700')
    LRTable_path = './ActionGoto.csv'
    sourcefile = open(LRTable_path,'r',encoding = 'utf-8')
    reader = csv.reader(sourcefile)
    result = list(reader)
    columns = result[0]
    tabel_frame = tk.Frame(windowson)
    tabel_frame.pack()

    xscroll = Scrollbar(tabel_frame, orient=HORIZONTAL)
    yscroll = Scrollbar(tabel_frame, orient=VERTICAL)

    table = ttk.Treeview(
            master=tabel_frame,  # 父容器
            height=30,  # 表格显示的行数,height行
            columns=columns,  # 显示的列
            show='headings',  # 隐藏首列
            xscrollcommand=xscroll.set,  # x轴滚动条
            yscrollcommand=yscroll.set,  # y轴滚动条
            )
    for column in columns:
        table.heading(column=column, text=column, anchor=CENTER,
                    command=lambda name=column:
                    messagebox.showinfo('', '{}描述信息~~~'.format(name)))  # 定义表头
    table.column(column=column, width=100, minwidth=100, anchor=CENTER, )  # 定义列
    xscroll.config(command=table.xview)
    xscroll.pack(side=BOTTOM, fill=X)
    yscroll.config(command=table.yview)
    yscroll.pack(side=RIGHT, fill=Y)
    table.pack(side=BOTTOM, fill=BOTH, expand=True)

    insert(table, result)

# 界面显示语法分析分析栈
def Synstackdisplay():
    SynScr2.delete(1.0, END)
    FirstSets_path = './StackInfo.txt'
    sourcefile = open(FirstSets_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        SynScr2.insert('end',line)

# 界面显示语法分析语法树
def SynTreedisplay():
    SynScr2.delete(1.0, END)
    FirstSets_path = './SynTree.txt'
    sourcefile = open(FirstSets_path,'r',encoding = 'utf-8')
    for line in sourcefile:
        SynScr2.insert('end',line)

if __name__ == '__main__':
    # 第1步，实例化object，建立窗口window
    window = tk.Tk()
    # 第2步，给窗口的可视化起名字
    window.title('词法/语法分析器') 
    # 第3步，设定窗口的大小(长 * 宽)
    window.geometry('800x700')  # 这里的乘是小x

    #标题
    Title = tk.Label(window, text = '词法、语法分析器', font=('楷体', 40), width=20, height=1)
    #Title.pack()
    Title.place(x=125, y=20)

    #词法分析
    button1 = tk.Button(window, text = "读入程序", font=('楷体', 12), fg="red", bg="yellow", width=10, height=1, command=SourceProGet)
    button1.place(x=40, y=110)
    LexScr1 = scrolledtext.ScrolledText(window, width=45, height=13, font=("隶书",12), bd =5)
    LexScr1.place(x=0,y=150)
    button2 = tk.Button(window, text = "词法分析", font=('楷体', 12), fg="red", bg="yellow", width=10, height=1, command=LexAnalyzeFunc)
    button2.place(x=140, y=110)
    button3 = tk.Button(window, text = "词法语法", font=('楷体', 12), fg="red", bg="yellow", width=10, height=1, command=LexRuleDisplay)
    button3.place(x=240, y=110)
    LexScr2 = scrolledtext.ScrolledText(window, width=45, height=13, font=("隶书",12), bd =5)
    LexScr2.place(x=400,y=150)
    button4 = tk.Button(window, text = "词法分析结果", font=('楷体', 12), fg="red", bg="yellow", width=14, height=1, command=LexResult)
    button4.place(x=530, y=110)

    #语法分析
    buttonSyn = tk.Button(window, text = "语法分析", font=('楷体', 12), fg="red", bg="yellow", width=10, height=1, command=SynAnalyzeFunc)
    buttonSyn.place(x=30, y=380)

    button5 = tk.Button(window, text = "文法语法", font=('楷体', 12), fg="red", bg="yellow", width=10, height=1, command=SynRuleDisplay)
    button5.place(x=130, y=380)
    button6 = tk.Button(window, text = "FIRST集合", font=('楷体', 13), fg="red", bg="yellow", width=10, height=1, command=SynFIRSTDisplay)
    button6.place(x=230, y=380)
    SynScr1 = scrolledtext.ScrolledText(window, width=45, height=13, font=("隶书",12), bd =5)
    SynScr1.place(x=0,y=420)

    button7 = tk.Button(window, text = "符号栈分析过程", font=('楷体', 12), fg="red", bg="yellow", width=16, height=1, command=Synstackdisplay)
    button7.place(x=460, y=380)
    button8 = tk.Button(window, text = "语法树", font=('楷体', 12), fg="red", bg="yellow", width=10, height=1, command=SynTreedisplay)
    button8.place(x=610, y=380)
    SynScr2 = scrolledtext.ScrolledText(window, width=45, height=13, font=("隶书",12), bd =5)
    SynScr2.place(x=400,y=420)

    #显示分析表
    button9 = tk.Button(window, text = "分析表（ACTION&GOTO）", font=('楷体', 12), fg="red", bg="yellow", width=25, height=1, command=csvdisplay)
    button9.place(x=280, y=660)

    window.mainloop()