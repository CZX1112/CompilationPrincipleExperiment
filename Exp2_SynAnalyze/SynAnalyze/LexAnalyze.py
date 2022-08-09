import tkinter as tk
from tkinter.filedialog import askdirectory
from tkinter import StringVar
from tkinter import *
from tkinter import scrolledtext
from pandas import read_csv
from tkinter import ttk
from tkinter import messagebox
import csv
from FA import NFA, NFANode, DFA, DFANode
import hashlib

digit = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
alphabet = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
symbol = ['!', ',', ';', '[', ']',
          '(', ')', '{', '}', '+', '-', '*', '/', '%', '^', '&', '|', '=', '<', '>']
keyword = ['int', 'double', 'char', 'float', 'break', 'continue',
           'do', 'while', 'if', 'else', 'for', 'void', 'return']

class LexAnalyze(object):
    "词法分析类"
    def __init__(self):
        self.productions = list()       # 产生式列表
        self.alphabets = dict()         # 字母表（终结符）
        self.keywords = dict()
        self.NFA = None
        self.DFA = None
        self.alphabets['alphabet'] = alphabet
        self.alphabets['digit'] = digit
        self.alphabets['symbol'] = symbol
        for word in keyword:
            self.keywords[word] = 'keyword'

    def removenote(self, string):
        "用于调用，去除注释"
        CrossLine = False
        a, b = string.find('//'), string.find('/*')
        if a != -1 and (a < b or b == -1):
            string = string[:a]
        elif b != -1:
            c = string[b+2:].find('*/')
            if c > b+1:
                string = string[c+4:]
                CrossLine, string = self.removenote(string)
            else:
                CrossLine = True
                string = string[:b]
        return CrossLine, string

    # 入口参数为字符串型源代码,返回值为预处理之后包含每一行源代码的字符串

    def Preprocessing(self, sourcecode):
        "去除注释"
        sourcecode = sourcecode.split('\n')
        lines = [i.strip() for i in sourcecode]
        newcode = list()
        CrossLine_note_Flag = False
        for i in lines:
            if len(i) == 0:
                continue
            if not CrossLine_note_Flag:
                CrossLine_note_Flag, i = self.removenote(i)
                if len(i):
                    newcode.append(i)
            else:
                if i.find('*/') != -1:
                    i = i[i.index('*/')+2:]
                    CrossLine_note_Flag, i = self.removenote(i)
                    if len(i):
                        newcode.append(i)
        return newcode

    def readLexGrammar(self, filename):
        "读取词法规则"
        line_num = 0
        for line in open(filename, 'r'):
            line = line.split('\n')[0].strip()
            index = line.find(':')
            cur_left = line[0:index]
            cur_right = line[index + 1:len(line)]
            line_num += 1

            production = dict()
            production['left'] = cur_left
            index = cur_right.find(' ')

            # 右边有非终结符
            if index != -1:
                production['input'] = cur_right[0:index]
                production['right'] = cur_right[index + 1:len(cur_right)]

            # 右边没有非终结符
            else:
                production['input'] = cur_right
                production['right'] = None
            self.productions.append(production)

    def createNFA(self):
        "创建NFA"
        all_status = dict()

        def getNFANode(name, isFinal):
            if name in all_status:
                node = all_status[name]
            else:
                node = NFANode(name=name, isFinal=isFinal)
            return node
        start_node = getNFANode('start', 0)
        end_node = getNFANode('end', 1)
        all_status['start'] = start_node
        all_status['end'] = end_node
        for prod in self.productions:
            now = prod['left']
            alpha = prod['input']
            next = prod['right']
            now_node = getNFANode(now, 0)
            # 右边有非终结符，指向对应节点
            if next is not None:
                target_node = getNFANode(next, 0)
                all_status[next] = target_node
            # 输入字符不是由 'digit' 'nonzero_digit' 'alphabet' 表示的终结符，即alpha自身是一个终结符
            if alpha not in self.alphabets.keys():
                if next is None:
                    now_node.addEdge(alpha, 'end')
                else:
                    if next in self.alphabets.keys():
                        for val in self.alphabets[next]:
                            now_node.addEdge(alpha, val)
                    else:
                        now_node.addEdge(alpha, next)
            # 输入字符是由 'digit' 'nozero_digit' 'alphabet' 表示的终结符
            else:
                for val in self.alphabets[alpha]:
                    if next is None:
                        now_node.addEdge(val, 'end')
                    else:
                        if next in self.alphabets.keys():
                            for tval in self.alphabets[next]:
                                now_node.addEdge(alpha, tval)
                        else:
                            now_node.addEdge(alpha, next)
                            now_node.addEdge(val, next)
            # 更新NFA中的节点信息
            all_status[now] = now_node

        # NFA的终结符集合
        terminators = list()
        for i in range(ord(' '), ord('~') + 1):#全部合法的文本字符
            terminators.append(chr(i))
        self.NFA = NFA(terminators)
        self.NFA.status = all_status

    def createDFA(self):
        "创建对应的DFA"
        all_status = dict()

        def getDFANode(name, isFinal):
            if name in all_status:
                return all_status[name]
            else:
                node = DFANode(name, isFinal)
            return node

        for node_name in self.NFA.status['start'].edge['$']:
            start_node = getDFANode('start', 0)
            dfa_node = getDFANode(node_name, 0)
            start_node.addEdge('$', node_name)
            all_status['start'] = start_node
            all_status[node_name] = dfa_node

            # 记录DFA节点是否已经访问过
            is_visit = list()
            queue = list()

            # 最初的NFA节点集合，即DFA节点
            nfa_node_set = list()
            if node_name not in nfa_node_set:
                nfa_node_set.append(node_name)
            queue.append((nfa_node_set, node_name))

            # BFS
            while queue:
                node_name = queue.pop(0)
                now_node_set = node_name[0]
                now_node_name = node_name[1]
                now_dfa_node = getDFANode(now_node_name, 0)

                # move(I,alpha)  寻找后续状态
                for alpha in self.NFA.terminators:

                    # next节点的NFA节点集合
                    target_set = list()
                    for nfa_node_name in now_node_set:
                        nfa_node = self.NFA.status[nfa_node_name]

                        # 有出边
                        if alpha in nfa_node.edge.keys():
                            for name in nfa_node.edge[alpha]:
                                if name not in target_set:
                                    target_set.append(name)

                    # 如果target_set为空，则直接返回
                    if not target_set:
                        continue
                    next_node_name = ''
                    isFinal = 0
                    tmp_list = list(target_set)
                    target_list = sorted(tmp_list)
                    for tar in target_list:
                        next_node_name = '%s$%s' % (next_node_name, tar)
                        isFinal += int(self.NFA.status[tar].isFinal)

                    # 如果集合中有一个NFA的终态，该节点就是DFA的终态
                    if isFinal > 0:
                        isFinal = 1
                    next_dfa_node = getDFANode(next_node_name, isFinal)
                    now_dfa_node.addEdge(alpha, next_node_name)
                    all_status[now_node_name] = now_dfa_node
                    all_status[next_node_name] = next_dfa_node

                    # 该状态已经访问过，则继续
                    if next_node_name in is_visit:
                        continue

                    # 该状态未访问过，则放入队列中
                    else:
                        if next_node_name not in is_visit:
                            is_visit.append(next_node_name)
                        queue.append((target_set, next_node_name))

        # DFA的终结符集合
        terminators = list()
        for i in range(ord(' '), ord('~') + 1):#全部合法的文本字符
            terminators.append(chr(i))
        self.DFA = DFA(terminators)
        self.DFA.status = all_status

    def runOnDFA(self, line, pos):
        "开始分析"
        if line[pos] in self.alphabets['alphabet'] or line[pos] == '_':
            final_pos = pos
            final_str = ''
            while final_pos < len(line) and line[final_pos] not in self.alphabets['symbol'] and line[final_pos] != ' ':
                final_str += line[final_pos]
                final_pos += 1

            cur_pos = 0
            token = ''
            now_node = self.DFA.status['identifier']
            while cur_pos < len(final_str) and final_str[cur_pos] in now_node.edge.keys():
                token += final_str[cur_pos]
                now_node = self.DFA.status[list(
                    now_node.edge[final_str[cur_pos]])[0]]
                cur_pos += 1

            if cur_pos >= len(final_str) and now_node.isFinal > 0:

                if token in self.keywords.keys():
                    token_type = self.keywords[token]
                else:
                    token_type = 'identifier'
                return final_pos - 1, token_type, token, 'OK'
            else:
                return final_pos - 1, None, '', '标识符不合法'

        elif line[pos] in self.alphabets['digit']:

            # 判断是否为复数
            final_pos = pos
            final_str = ''
            while final_pos < len(line) and (line[final_pos] not in self.alphabets['symbol'] or line[final_pos] == '+'
                                             or line[final_pos] == '-') and line[final_pos] != ' ':
                final_str += line[final_pos]
                final_pos += 1

            cur_pos = 0
            token = ''
            now_node = self.DFA.status['complex']
            while cur_pos < len(final_str) and final_str[cur_pos] in now_node.edge.keys():
                token += final_str[cur_pos]
                now_node = self.DFA.status[list(
                    now_node.edge[final_str[cur_pos]])[0]]
                cur_pos += 1

            if cur_pos >= len(final_str) and now_node.isFinal > 0:
                token_type = 'number'
                return final_pos - 1, token_type, token, 'OK'

            # 判断是否为科学计数形式常量
            final_pos = pos
            final_str = ''
            while final_pos < len(line) and (line[final_pos] not in self.alphabets['symbol'] or line[final_pos] == '+'
                                             or line[final_pos] == '-') and line[final_pos] != ' ':
                final_str += line[final_pos]
                final_pos += 1

            cur_pos = 0
            token = ''
            now_node = self.DFA.status['scientific']
            while cur_pos < len(final_str) and final_str[cur_pos] in now_node.edge.keys():
                token += final_str[cur_pos]
                now_node = self.DFA.status[list(
                    now_node.edge[final_str[cur_pos]])[0]]
                cur_pos += 1

            if cur_pos >= len(final_str) and now_node.isFinal > 0:
                token_type = 'number'
                return final_pos - 1, token_type, token, 'OK'

            # 判断是否为整型常量
            final_pos = pos
            final_str = ''
            while final_pos < len(line) and line[final_pos] not in self.alphabets['symbol'] and line[final_pos] != ' ':
                final_str += line[final_pos]
                final_pos += 1

            cur_pos = 0
            token = ''
            now_node = self.DFA.status['integer']
            while cur_pos < len(final_str) and final_str[cur_pos] in now_node.edge.keys():
                token += final_str[cur_pos]
                now_node = self.DFA.status[list(
                    now_node.edge[final_str[cur_pos]])[0]]
                cur_pos += 1

            if cur_pos >= len(final_str) and now_node.isFinal > 0:
                token_type = 'number'
                return final_pos - 1, token_type, token, 'OK'

            return final_pos - 1, None, '', '标识符或常量不合法'

        else:
            cur_pos = pos
            token = ''
            token_type = 'limiter'
            now_node = self.DFA.status['limiter']

            # 逐个向后读取字符，并进行状态转移
            while cur_pos < len(line) and line[cur_pos] in now_node.edge.keys():
                token += line[cur_pos]
                now_node = self.DFA.status[list(
                    now_node.edge[line[cur_pos]])[0]]
                cur_pos += 1

            # 如果到达终态，则获得一个单词
            if now_node.isFinal > 0:
                return cur_pos - 1, token_type, token, 'OK'

            cur_pos = pos
            token = ''
            token_type = 'operator'
            now_node = self.DFA.status['operator']

            # 逐个向后读取字符，并进行状态转移
            while cur_pos < len(line) and line[cur_pos] in now_node.edge.keys():
                token += line[cur_pos]
                now_node = self.DFA.status[list(
                    now_node.edge[line[cur_pos]])[0]]
                cur_pos += 1

            # 如果到达终态，则获得一个单词
            if now_node.isFinal > 0:
                return cur_pos - 1, token_type, token, 'OK'

            cur_pos = pos
            while cur_pos < len(line) and line[cur_pos] not in self.alphabets['symbol'] \
                    and line[cur_pos] not in self.alphabets['digit'] and line[cur_pos] not in self.alphabets['alphabet'] \
                    and line[cur_pos] != '_' and line[cur_pos] != ' ':
                cur_pos += 1
            return cur_pos - 1, None, '', '非法文本字符'

    def analyze(self, codelist, TokenTable_path):
        "顶层函数"
        line_num = 0
        lex_error = False
        token_table = list()
        error_message=None
        for line in codelist:
            pos = 0
            line_num += 1
            while pos < len(line):
                # 跳过tab，回车，换行，空格
                while pos < len(line) and line[pos] in ['\t', '\n', ' ', '\r']:
                    pos += 1
                if pos < len(line):
                    pos, token_type, token, message = self.runOnDFA(line, pos)
                    if token_type is None:
                        error_message='Lexical error at line %s, column %s : %s' % (str(line_num), str(pos), message)
                        print(error_message)
                        lex_error = True
                        # break
                    else:
                        token_table.append((line_num, token_type, token))
                    pos += 1

        # 如果未出错，那么写入token表文件
        if not lex_error:
            output = open(TokenTable_path, 'w+')
            for line_num, token_type, token in token_table:
                output.write('%d %s %s\n' % (line_num, token_type, token))
            output.close()
            print("Lex analyze complete!")
            return True,error_message
        else:
            print("Lex analyze failed!")
        return False,error_message

class c_tokenner:
    type = None
    text = None
    # single token
    EOS_TOKEN  = 1             # END of string
    ADD_TOKEN = EOS_TOKEN + 1  # +
    SUB_TOKEN = ADD_TOKEN + 1  # -
    MUL_TOKEN = SUB_TOKEN + 1  # *
    DIV_TOKEN = MUL_TOKEN + 1  # /
    
    LBL_TOKEN = DIV_TOKEN + 1  # (
    LBR_TOKEN = LBL_TOKEN + 1  # )
    SEM_TOKEN = LBR_TOKEN + 1  # ;
    NOT_TOKEN = SEM_TOKEN + 1  # '
    
    EMT_TOKEN = NOT_TOKEN + 1  # space
    NEW_TOKEN = EMT_TOKEN + 1  # new line
    # multi token
    ID_TOKEN    = 50           # letter
    NUM_TOKEN   = ID_TOKEN + 1 # number
    IF_TOKEN    = NUM_TOKEN + 1 # if
    THEN_TOKEN  = IF_TOKEN + 1 # then
    ELSE_TOKEN  = THEN_TOKEN + 1 # else
    WHILE_TOKEN = ELSE_TOKEN + 1 # while
    DO_TOKEN    = WHILE_TOKEN + 1 # do
    
    # operator
    GREA_TOKEN  = DO_TOKEN + 1   # >
    LESS_TOKEN  = GREA_TOKEN + 1 # <
    EVAL_TOKEN  = LESS_TOKEN + 1 # =
    EQUL_TOKEN  = EVAL_TOKEN + 1 # ==
    G_E_TOKEN   = EQUL_TOKEN + 1 # >=61
    L_E_TOKEN   = G_E_TOKEN + 1  # <=
    N_E_TOKEN   = L_E_TOKEN + 1  # != 
    
    INT_TOKEN   = N_E_TOKEN + 1  # int
    FLO_TOKEN   = INT_TOKEN + 1  # float
    ANY_TOKEN   = FLO_TOKEN + 1  # other char

    @classmethod
    def text(self,t):
        if(t.type == c_tokenner.ID_TOKEN):
            return 'id'
        elif(t.type == c_tokenner.NUM_TOKEN):
            return 'int10'
        else:
            return t.text

def insert(table, result):
    # 插入数据
    for index, data in enumerate(result):
        if index != 0:
            table.insert('', END, values=data)  # 添加数据到末尾

class c_symboller:
    symbol_table = {}
    #
    # add token to symbol table
    #
    def add(self, tk):
        buf  = str(tk.type) + tk.text
        hash = hashlib.md5(buf).hexdigest()
        if not self.symbol_table.has_key(hash):
            self.symbol_table.update({hash:[tk.type, tk.text]})
    #
    # show symbol table
    #
    def show(self):
        for key in self.symbol_table:
            print(self.symbol_table[key])
    #
    # write symbol to file
    #
    def write2file(self):
        fhandle = open('symbol_table.txt','w')
        for key in self.symbol_table:
            buf = '%s\t%d\t%s\n'%(key, self.symbol_table[key][0], self.symbol_table[key][1])
            fhandle.write(buf)
        fhandle.close()

    def read2file(self):
        fhandle = open('symbol_table.txt','w')
        for key in self.symbol_table:
            buf = '%s\t%d\t%s\n'%(key, self.symbol_table[key][0], self.symbol_table[key][1])
            print(buf)
        fhandle.close()

if __name__ == '__main__':
    source_path = './sourceprogram.cc'  # 源文件相对路径
    LexGrammar_path = './LexGra.txt'  # 词法规则文件相对路径
    TokenTable_path = './LexAnaResult.txt'  # 存储TOKEN表的相对路径

    lex_ana = LexAnalyze()
    lex_ana.readLexGrammar(LexGrammar_path)
    lex_ana.createNFA()
    lex_ana.createDFA()
    codelist = lex_ana.Preprocessing(open(source_path, "r").read())
    lex_ana.analyze(codelist, TokenTable_path)

    # 第1步，实例化object，建立窗口window
    window = tk.Tk()
    # 第2步，给窗口的可视化起名字
    window.title('My Window') 
    # 第3步，设定窗口的大小(长 * 宽)
    window.geometry('800x600')  # 这里的乘是小x
    # 第4步，在图形界面上设定标签
    var = tk.StringVar()    # 将label标签的内容设置为字符类型，用var来接收hit_me函数的传出内容用以显示在标签上
    l = tk.Label(window, textvariable=var, bg='green', fg='white', font=('Arial', 12), width=30, height=2)
    # 说明： bg为背景，fg为字体颜色，font为字体，width为长，height为高，这里的长和高是字符的长和高，比如height=2,就是标签有2个字符这么高
    #l.pack()

    on_hit = False
    def hit_me():
        global on_hit
        if on_hit == False:
            on_hit = True
            var.set('you hit me')
        else:
            on_hit = False
            var.set('')
 
    # 第5步，在窗口界面设置放置Button按键
    b = tk.Button(window, text='hit me', font=('Arial', 12), width=10, height=1, command=hit_me)

    path = StringVar()

    def selectPath():
        var=StringVar()
        scr = scrolledtext.ScrolledText(window, width=30, height=10, font=("隶书",18), bd =5)
        scr.place(x=0,y=50)

        path_ = askdirectory()
        path.set(path_)
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
    
    label=Label(window,text = "目标路径:")
    label.place(x=0,y=0)
    entry = Entry(window, textvariable = path)
    entry.place(x=200, y=20)
    #Button(window, text = "路径选择", command = selectPath).grid(row = 0, column = 2)
    button1 = tk.Button(window, text = "路径选择", font=('Arial', 12), width=10, height=1, command=selectPath)
    button1.place(x=50, y=10)

    window.mainloop()