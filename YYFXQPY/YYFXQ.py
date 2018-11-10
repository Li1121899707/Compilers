import sys
class Logger(object): 
    def __init__(self, fileN="Default.log"):
        self.terminal = sys.stdout
        self.log = open(fileN, "w")
 
    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)
 
    def flush(self):
        pass
sys.stdout = Logger("target_file.txt")

NONTERMINATOR = []
TERMINATOR = []

grammarOriginalListsWithArrow = []
grammarOriginalLists = []
rule = []
grammarWithNode = []
stream = ""
streamValWithSymbol = ""
streamval = []
CLOSURE = []
FIRST = []
FOLLOW = []
isFollowChange = 0
action = []
goto = []
symbolStack = []
stateStack = []
valStack = []
LOG = []
MAPS = {"for":'f', "int":'i', "double":'d', "float":'t', "while":'w'}
grammarResultVal = 0

grammarFile = "grammar.txt"
cffxResultFile = "cffxresult.txt"
ruleFile = "rule.txt"


with open(grammarFile, 'r') as f:
    grammarOriginalStr = f.read()
    grammarOriginalListsWithArrow = grammarOriginalStr.split('\n')
    grammarOriginalStr = grammarOriginalStr.replace('->', '')
    grammarOriginalLists = grammarOriginalStr.split('\n')
    nonTerminatorSet = set()
    terminatorSet = set()
    for i in grammarOriginalLists:
        for j in i:
            if j.isupper():
                nonTerminatorSet.add(j)
            else:
                terminatorSet.add(j)
    nonTerminatorSet.remove(grammarOriginalLists[0][0])
    terminatorSet.add('#')
    NONTERMINATOR = list(nonTerminatorSet)
    TERMINATOR = list(terminatorSet)

print("源文法: ",grammarOriginalListsWithArrow)
print("源文法(去掉->): ",grammarOriginalLists)
print("非终结符集: ", NONTERMINATOR)
print("终结符集: ", TERMINATOR)


with open(cffxResultFile, 'r') as f:
    cffxResult = f.read().split('\n')
    cffxStr = ""
    cffxValWithSymbol = ""
    for i in cffxResult:
        if i[(i.index('(')+1):i.index(',')] == "KEY":
            keyStr = i[(i.index(',')+1):i.index(')')]
            cffxStr = cffxStr + MAPS[keyStr]
            streamval.append('_')
        elif i[(i.index('(')+1):i.index(',')] == "NUM":
            cffxStr = cffxStr + 'g'
            streamval.append(i[(i.index(',')+1):i.index(')')])
            cffxValWithSymbol = cffxValWithSymbol + i[(i.index(',')+1):i.index(')')]
        else:
            cffxStr = cffxStr + i[-2]
            streamval.append('_')
            cffxValWithSymbol = cffxValWithSymbol + i[-2]
    if cffxStr == "":
        print("词法分析结果无法识别！")
        exit()
    stream = cffxStr
    streamValWithSymbol = cffxValWithSymbol

print("输入流: ",stream)
print("输入流值: ",streamval)

with open(ruleFile, 'r') as f:
    ruleStr = f.read()
    rule = ruleStr.split('\n')

print("语义规则: ", rule)

nodeIndex = 1
for i in grammarOriginalLists:
    while(nodeIndex <= len(i)):
        grammarWithNode.append((i[0:nodeIndex] + '@' + i[nodeIndex:]))
        nodeIndex += 1
    nodeIndex = 1

for i in range(0, len(grammarWithNode)):
    print("item(", i, ") = ", grammarWithNode[i])

def searchClosure(c, tempClosure): 
    for i in grammarWithNode:
        if i[0] != c:
            continue
        if i[1] == '@':
            tempClosure.append(i)
            if c != i[2] and i[2] in NONTERMINATOR :
                searchClosure(i[2], tempClosure) 

def newList(size):
    for i in range(0, size):
        FIRST.append([])
    for i in range(0, size):
        FOLLOW.append([])

def nodeRightMove(expression):
    nodePosition = expression.index('@')
    if nodePosition == (len(expression) - 2):
            result = expression[0] + expression[(nodePosition+1)] + "@" 
    if nodePosition == (len(expression) - 1):
            result = "传入参数为归约状态，错误！"
    else:
        result = expression[0:(nodePosition)] + expression[(nodePosition+1)] + "@" + expression[(nodePosition+2):] 
    return result

def CLOSUREF(J):
    tempClosure = []
    for i in J:
        nodePosition = i.index('@')
        if (nodePosition == (len(i) - 1)) or ((i[nodePosition + 1]) in TERMINATOR): 
            tempClosure.append(i)
        elif((i[nodePosition + 1]) in NONTERMINATOR): 
            tempClosure.append(i)
            searchClosure(i[nodePosition+1], tempClosure) 
    
    if tempClosure in CLOSURE:
        return CLOSURE[CLOSURE.index(tempClosure)]
    CLOSURE.append(tempClosure)  
    return tempClosure

def GO(I,X):
    J = []
    for i in I:
        nodeIndex = i.index('@')
        if nodeIndex == len(i) - 1:
            continue
        if i[nodeIndex + 1] != X:
            continue
        
        moveRightStr = nodeRightMove(i)
        J.append(moveRightStr)

    return CLOSUREF(J)

def closureController(tempClosure):
    NONTSET = [] 
    for i in tempClosure:
        nodeIndex = i.index('@')
        if nodeIndex == (len(i) - 1):
            continue
        if i[nodeIndex + 1] in NONTSET:
            continue
        NONTSET.append(i[nodeIndex + 1])
    
    for j in NONTSET:
        GO(tempClosure, j)

def generateClosure():
    I = grammarWithNode[0].split(" ")
    tempClosure = CLOSUREF(I)
    closureController(tempClosure)
    j = 0
    for i in CLOSURE: 
        if j == 0:
            j = j+1
            continue
        closureController(i)

    for i in range(len(CLOSURE)):
        print("I", i, "= ", CLOSURE[i])

def findFirstSetChar(ch):
    for i in range(len(grammarOriginalLists)):
        tempCh = grammarOriginalLists[i][0]
        tempNextCh = grammarOriginalLists[i][1]
        if tempCh == ch:
            if tempCh == tempNextCh:
                continue
            if tempNextCh in TERMINATOR:
                if tempNextCh not in FIRST[NONTERMINATOR.index(ch)]:
                    FIRST[NONTERMINATOR.index(ch)].append(tempNextCh)
            else:
                findFirstSetChar(tempNextCh)
                tempNextChInFirstIndex = NONTERMINATOR.index(tempNextCh)
                for j in range(len(FIRST[tempNextChInFirstIndex])):
                    if FIRST[tempNextChInFirstIndex][j] not in FIRST[NONTERMINATOR.index(ch)]:
                        FIRST[NONTERMINATOR.index(ch)].append(FIRST[tempNextChInFirstIndex][j])
                     
def generateFirst():
    newList(len(NONTERMINATOR))
    for i in NONTERMINATOR:
        findFirstSetChar(i)
    for i in range(len(FIRST)):
        print("FIRST(", NONTERMINATOR[i],") = ", FIRST[i])

def addToFollow(nonTerminatorIndex, ch):
    global isFollowChange
    for i in FOLLOW:
        if ch in i:
            return 
    FOLLOW[nonTerminatorIndex].append(ch)
    isFollowChange = 1

def generateFollow():
    global isFollowChange
    isFollowChange = 1
    followLen = 0
    FOLLOW[NONTERMINATOR.index(grammarOriginalLists[1][0])].append('#')
    while(isFollowChange):
        for i in range(1, len(grammarOriginalLists)):
            for j in range(1, len(grammarOriginalLists[i])): 
                tempCh = grammarOriginalLists[i][j]
                if tempCh in TERMINATOR: 
                    continue
                if j == len(grammarOriginalLists[i])-1: # S->E,E依赖S
                    mergeSet = set(FOLLOW[NONTERMINATOR.index(tempCh)]) | set(FOLLOW[NONTERMINATOR.index(grammarOriginalLists[i][0])])
                    FOLLOW[NONTERMINATOR.index(tempCh)] = list(mergeSet)
                elif grammarOriginalLists[i][j+1] in TERMINATOR:
                    tempSet = set(FOLLOW[NONTERMINATOR.index(tempCh)])
                    tempSet.add(grammarOriginalLists[i][j+1])
                    FOLLOW[NONTERMINATOR.index(tempCh)] = list(tempSet)
                else:
                    tempMergeSet = set(FOLLOW[NONTERMINATOR.index(tempCh)]) | set(FIRST[NONTERMINATOR.index(grammarOriginalLists[i][j+1])])
                    FOLLOW[NONTERMINATOR.index(tempCh)] = list(tempMergeSet)
        followLen = 0
        for i in FOLLOW:
            followLen = followLen + len(i)
        if followLen == isFollowChange:
            isFollowChange = 0
        else:
            isFollowChange = followLen

    for i in range(len(FOLLOW)):
        print("FOLLOW(", NONTERMINATOR[i],") = ", FOLLOW[i])

def generateSLR():
    global action, goto
    action = [[' ' for col in range(len(TERMINATOR))] for row in range(len(CLOSURE))]
    goto = [[' ' for col in range(len(NONTERMINATOR))] for row in range(len(CLOSURE))]
    for k in range(0, len(CLOSURE)):
        for a in CLOSURE[k]: # Closure内各表达式
            nodeIndex = a.index('@')
            if nodeIndex == len(a) - 1:
                grammarIndex = grammarOriginalLists.index(a[0:len(a)-1])
                for ter in TERMINATOR:
                    if a[0] == grammarOriginalLists[0][0]:
                        action[k][TERMINATOR.index('#')] = "a0"
                    elif ter in FOLLOW[NONTERMINATOR.index(a[0])]:
                        action[k][TERMINATOR.index(ter)] = "r" + str(grammarIndex)
            elif a[nodeIndex+1] in TERMINATOR:
                resultClosure = GO(CLOSURE[k], a[nodeIndex+1])
                aIndex = CLOSURE.index(resultClosure)
                action[k][TERMINATOR.index(a[nodeIndex+1])] = "S" + str(aIndex)
            elif a[nodeIndex+1] in NONTERMINATOR:
                resultClosure = GO(CLOSURE[k], a[nodeIndex+1])
                aIndex = CLOSURE.index(resultClosure)
                goto[k][NONTERMINATOR.index(a[nodeIndex+1])] = "G" + str(aIndex)

def printSLR():
    TABLEHEADER = "ID".split(" ") + TERMINATOR.copy() + NONTERMINATOR.copy()
    from prettytable import PrettyTable
    table = PrettyTable(TABLEHEADER)

    for i in range(0, len(CLOSURE)):
        table.add_row(str(i).split(" ") + action[i] + goto[i])
    print(table)

def calculateRecuctionVal(reductionVals, reductionIndex):# 要归约的所有符号值，以及归约的文法下标
    valsDict = {}
    i = -1
    while grammarOriginalListsWithArrow[reductionIndex][i] != '>':
        if reductionVals[i] != '_':
            valsDict[grammarOriginalListsWithArrow[reductionIndex][i]] = reductionVals[i]
        i = i-1
    resultVal = 0
    if '*' in rule[reductionIndex]:
        charIndex = rule[reductionIndex].index('*')
        resultVal = int(valsDict[rule[reductionIndex][charIndex-1]]) * int(valsDict[rule[reductionIndex][charIndex+1]])
    elif '+' in rule[reductionIndex]:
        charIndex = rule[reductionIndex].index('+')
        resultVal = int(valsDict[rule[reductionIndex][charIndex-1]]) + int(valsDict[rule[reductionIndex][charIndex+1]])
    else:
        resultVal = int(valsDict[rule[reductionIndex][-1]])
    print(valsDict)
    return resultVal

def analysisGrammar(ch, val):
    global grammarResultVal
    tempLog = []
    tempLog.append(ch)
    currentStateTop = stateStack[-1]
    currentSymbolTop = symbolStack[-1]
    currentValTop = valStack[-1]
    stateStackNew = [str(x) for x in stateStack] # state都是数字，因此需要这一步
    valStackNew = [str(x) for x in valStack]
    tempLog.append(" ".join(stateStackNew))
    tempLog.append(' '.join(symbolStack))
    tempLog.append(' '.join(valStackNew))
    changeStateStr = ' '
    if ch in TERMINATOR:
        changeStateStr = action[currentStateTop][TERMINATOR.index(ch)]
        tempLog.append("A("+str(currentStateTop)+","+ch+")")
    else:
        changeStateStr = goto[currentStateTop][NONTERMINATOR.index(ch)]
        tempLog.append("G("+str(currentStateTop)+","+ch+")")
    tempLog.append(changeStateStr)

    if changeStateStr == ' ':
        symbolStack.append(ch)
        tempLog.append(" ")
        tempLog.append(" ")
        LOG.append(tempLog)
        print("---------------------------出现错误,无法归约为开始字符---------------------------")
        sys.exit()
    elif changeStateStr[0] == 'S':
        tempLog.append("移进")
        stateStack.append(int(changeStateStr[1:]))
        symbolStack.append(ch)
        valStack.append(val)
        tempLog.append(changeStateStr[1:]+"与"+ch+"在栈中")
        LOG.append(tempLog)
    elif changeStateStr[0] == 'r':
        tempLog.append("归约")
        reductionStr = grammarOriginalLists[int(changeStateStr[1:])]
        reductionLen = len(reductionStr) - 1
        reductionVals = []
        for i in range(0, reductionLen):
            if currentStateTop != 0:
                stateStack.pop()
            else:
                print("状态栈到底了")
            if currentSymbolTop != '#':
                symbolStack.pop()
            else:
                print("符号栈到底了")
            if currentValTop != '#':
                reductionVals.append(valStack[-1])
                valStack.pop()
            else:
                print("值栈到底了")
        resultVal = calculateRecuctionVal(reductionVals, int(changeStateStr[1:]))
        grammarResultVal = resultVal

        reductionSymbol = grammarOriginalLists[int(changeStateStr[1:])][0]
        tempLog.append("归约后字符为："+reductionSymbol)
        LOG.append(tempLog)
        analysisGrammar(reductionSymbol, resultVal)
        analysisGrammar(ch, val) # 归约完毕，但是输入流传来的符号还没有进行移进或归约
    elif changeStateStr[0] == 'G':
        tempLog.append("移进")
        stateStack.append(int(changeStateStr[1:]))
        symbolStack.append(ch)
        valStack.append(val)
        tempLog.append(changeStateStr[1:]+"与"+ch+"在栈中")
        LOG.append(tempLog)
    elif changeStateStr[0] == 'a':
        tempLog.append("成功接收")
        tempLog.append("成功接收")
        LOG.append(tempLog)

def analysisGrammarController():
    global grammarResultVal
    symbolStack.append('#')
    stateStack.append(0)
    valStack.append('#')
    for i in range(0,len(stream)):
        analysisGrammar(stream[i], streamval[i])
    analysisGrammar('#','_')
    
    TABLEHEADER = ["输入", "状态栈", "符号栈", "值栈",  "A/G", "转移字符串", "动作", "动作完成后状态"]
    from prettytable import PrettyTable
    table = PrettyTable(TABLEHEADER)

    for i in LOG:
        table.add_row(i)
    print(table)
    print("语法制导翻译 " + streamValWithSymbol + " 结果为: ", grammarResultVal)

generateClosure()
generateFirst()
generateFollow()
generateSLR()
printSLR()
analysisGrammarController()