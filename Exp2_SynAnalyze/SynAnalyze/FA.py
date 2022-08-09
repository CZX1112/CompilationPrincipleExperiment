class DFANode(object):
    def __init__(self, name=None, isFinal=0):
        super(DFANode, self).__init__()
        self.name = name
        self.isFinal = isFinal
        self.edge = {}
    def addEdge(self, alpha, target):
        if alpha not in self.edge:
            nextNodes = set()
            nextNodes.add(target)
            self.edge[alpha] = nextNodes
        else:
            self.edge[alpha].add(target)

class DFA(object):

    def __init__(self, terminators):
        super(DFA, self).__init__()
        self.status = {}
        self.terminators = terminators

class NFANode(object):
    def __init__(self, name=None, isFinal=0):
        super(NFANode, self).__init__()
        self.name = name
        self.isFinal = isFinal
        self.edge = {}
    def addEdge(self, alpha, target):
        if alpha not in self.edge:
            nextNodes = set()
            nextNodes.add(target)
            self.edge[alpha] = nextNodes
        else:
            self.edge[alpha].add(target)

class NFA(object):

    def __init__(self, terminators=None):
        super(NFA, self).__init__()
        self.terminators = terminators
        self.status = {}

# # LR(1)项目集
# class LRDFANode(object):

#     def __init__(self, id):
#         self.id = id
#         self.itemSet = set()
#         # self.edge = {}

#     def addItemSetBySet(self, itemSet):
#         self.itemSet |= itemSet


# # LR(1)项目集的DFA
# class LRDFA(object):

#     def __init__(self, terminators):
#         super(LRDFA, self).__init__()
#         self.status = {}
#         self.terminators = terminators

class LRDFANode(object):
    def __init__(self, id):
        self.id = id
        self.itemSet = list()
 
    def addItemSetBySet(self, itemSet):
        for i in itemSet:
            if i not in self.itemSet:
                self.itemSet.append(i)