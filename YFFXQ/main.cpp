#include "grammerHeader.h"

/*******************************  项目生成 开始 **************************************/
// 根据->分割字符串
vector<string> splitGrammar(const string &str,const string &pattern)
{
    //const char* 转 to char*
    char* strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str()); //strtok， 分割字符串
    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str()); //strtok， 分割字符串，此时第一个参数需要为NULL
    }
    // 释放空间
    delete[] strc;
    return resultVec;
}

// 读取文法txt
void readGrammer(char* file)
{
    // 初始化grammar数组为空
    for(int i=0; i<MAX_GRAMMER; i++ )
    {
        memset(grammars[i], 0, MAX_GRAMMER*sizeof(char));
        grammarOriginal[i] = "\0";
    }

    // 初始化grammarOriginal数组为空
    ifstream ifile(file);
    string s;
    string splitPattern = "->";
    int index = 0;
    // 按行读取文法，进行解析
    cout<<"***************  从文件中读取文法  ******************"<<endl;
    while(getline(ifile, s))
    {
        cout<<s<<endl;
        grammarOriginal[index] = s;
        vector<string> splitList = splitGrammar(s, splitPattern);
        // rowOffset存在的意义是，iter第一次返回表达式左边的，第二次才返回右边的，因此需要加偏移
        int rowOffset = 0;
        for (vector<string>::const_iterator iter = splitList.begin(); iter != splitList.end(); iter++)
        {
            char* temp = const_cast<char*>((*iter).c_str()); // string -> const char* -> char*
            int len = strlen(temp);
            for(int j=0; j<len; j++)
                grammars[index][rowOffset + j] = *(temp + j);
            delete temp; // 释放指针内存
            rowOffset = len;
        }
        index++; // 文法下移一行
        vector<string>().swap(splitList); // 释放空间
    }

    grammarOriginalLength = index;

    // 打印文法
    int k = 0;
    cout<<"***************  去掉->文法  ******************"<<endl;
    while(grammars[k][0]!='\0')
    {
        int grammarLen = strlen(grammars[k]);
        for(int j=0; j<grammarLen; j++)
            cout<<grammars[k][j];
        cout<<endl;
        k++;
    }
    ifile.close();
    cout<<"*****************************************************"<<endl;
}

// 添加项目 . ， 这里用@表示
void addItemNodeFlag()
{
    int i = 0;
    int nodeId = 0;
    cout<<"***************  显示所有项目及编号  ******************"<<endl;
    cout<<setw(12)<<"itemId"<<setw(20)<<"itemContent"<<setw(18)<<"isAddToClosur"<<endl;

    while(grammars[i][0]!='\0')
    {
        int addPosition = 1; // 由于->左侧的为第一位，因此点从下标为1的位置开始加
        int grammarLen = strlen(grammars[i]);
        for(int k=0; k<grammarLen; k++)
        {
            char tempItem[MAX_GRAMMER];
            memset(tempItem, 0, MAX_GRAMMER);
            int tempItemIndex = 0; // 当前项目数组下标
            for(int j=0; j<=grammarLen; j++)
            {
                if(j<addPosition)
                    tempItem[tempItemIndex] = grammars[i][j];
                else if(j == addPosition && j< grammarLen) // 当@不在最后一位
                {
                    tempItem[tempItemIndex] = '@';
                    tempItem[tempItemIndex+1] = grammars[i][j];
                    tempItemIndex++;
                }
                else if(j < grammarLen)
                    tempItem[tempItemIndex] = grammars[i][j];
                else if(addPosition == grammarLen) // 当@在最后一位
                    tempItem[tempItemIndex] = '@';

                tempItemIndex++;
                tempItem[tempItemIndex] = '\0'; // 初始化下一位
            }

            itemStruct i ;
            i.nodeId = nodeId;
            strcpy(i.content, tempItem);
            i.isAdd = false;
            cout<<setw(12)<<i.nodeId<<setw(20)<<i.content<<setw(18)<<i.isAdd<<endl;
            items.push_back(i); // 向vector添加

            delete[] tempItem; // 清除tempItem 空间

            nodeId++; // 节点编号增加
            addPosition++; // @ 移位
        }

        i++; // 语法移行
    }
    cout<<"*******************************************************"<<endl;
}

/*******************************  项目生成 结束  **************************************/

/*******************************  字符流生成 开始  *************************************/

// 是否是大写字母
bool isCapitalLetter(char ch)
{
    int capitalLetterLen = strlen(capitalLetter);
    for(int i=0; i<capitalLetterLen; i++)
    {
        if(ch == capitalLetter[i])
            return true;
    }
    return false;
}

// 是否是小写字母
bool isLetter(char ch)
{
    int letterLen = strlen(letter);
    for(int i=0; i<letterLen; i++)
    {
        if(ch == letter[i])
            return true;
    }
    return false;
}

// 是否是符号
bool isSymbol(char ch)
{
    int symbolLen = strlen(symbol);
    for(int i=0; i<symbolLen; i++)
    {
        if(ch == symbol[i])
            return true;
    }
    return false;
}

void addLetterToMartix(int type, char ch)
{
    if(1 == type)
    {
        int i = 0;
        while(nonTerminator[i] != '\0')
        {
            if(ch == nonTerminator[i])
                return;
            i++;
        }
        nonTerminator[i] = ch;
        cout<<ch<<" ";
    }
    if(2 == type)
    {
        int i = 0;
        while(terminator[i] != '\0')
        {
            if(ch == terminator[i])
                return;
            i++;
        }
        terminator[i] = ch;
        cout<<ch<<" ";
    }
}

void initLetterMatrix()
{
    memset(terminator, 0, MAX_LETTER);
    memset(nonTerminator, 0, MAX_LETTER);
    terminator[0] = '#';
}

// 生成 terminator, nonTerminator
void generateNTerminators()
{

    initLetterMatrix();

    cout<<"文法中所有终结符与非终结符："<<endl;
    for(int i=1; i<grammarOriginalLength; i++)
    {
        int j = 0;
        while(grammars[i][j]!='\0')
        {
            if(isCapitalLetter(grammars[i][j]))
                addLetterToMartix(1, grammars[i][j]);
            else
                addLetterToMartix(2, grammars[i][j]);

            j++;
        }
    }
    cout<<endl;
}

void generateCharStream(char* file, char* fileOut)
{
    ifstream ifile(file);
    ofstream ofile(fileOut);
    string s;
    // 按行读取文法，进行解析
    cout<<"***************  生成输入流  ******************"<<endl;
    while(getline(ifile, s))
    {
        cout<<s<<endl;
        cout<<s.substr(1, 2);
        if(s.substr(1, 2).compare("ID") == 0)
        {
            ofile<<"g";
            cout<<"  ======>  g"<<endl;
        }
        else
        {
            ofile<<s.substr(4, 1);
            cout<<"  ======>   "<<s.substr(4, 1)<<endl;
        }
    }

    ifile.close();
    ofile.close();
}

/*******************************  字符流生成 结束  *************************************/


/****************************  NFA Closure DFA 生成 开始  ********************************/

// 初始化NFA DFA FIRST FOLLOW
void initGraph()
{
    // 初始化grammar数组为空
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(NFA[i], 0, MAX_ITEM_NUM*sizeof(char));
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(DFA[i], 0, MAX_ITEM_NUM*sizeof(char));
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(FIRST[i], 0, MAX_ITEM_NUM*sizeof(char));
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(FOLLOW[i], 0, MAX_ITEM_NUM*sizeof(char));
}

// 判断是否是终结符
bool isTerminator(char ch)
{
    int terminatorLen = strlen(terminator);
    for(int i=0; i<terminatorLen; i++)
    {
        if(ch == terminator[i])
            return true;
    }
    return false;
}

// 返回@的位置
int getNodePosition(char* ch)
{
    int chLen = strlen(ch);
    for(int i=1; i<chLen; i++)
    {
        if(ch[i] == '@')
            return i;
    }
    return 0;
}

// 返回@前一个字符的位置
char getNodePrevCharPosition(char* ch)
{
    int chLen = strlen(ch);
    for(int i=1; i<chLen; i++)
    {
        if(ch[i] == '@')
            return ch[i-1];
    }
    return '$';
}


// 查找第一个字符
char* getFirstChar(char* ch)
{
    char* p = new char[2];
    memset(p, 0, 2);
    *p = *ch;
    return p;
}

// 查找最后一个字符
char* getLastChar(char* ch)
{
    char* p = new char[2];
    memset(p, 0, 2);
    while(*ch)
    {
        *p = *ch;
        ch++;
    }
    return p;
}

// 查找@下一个字符
char* getNextChar(char* ch)
{
    char* p = new char[2];
    memset(p, 0, 2);
    while(*ch)
    {
        if(*ch == '@')
        {
            if(*(ch+1)!= '\0')
            {
                *p = *(ch+1);
                return p;
            }
        }
        ch++;
    }
    *p = '\0';
    return p;
}


// 将加点后移一位
void moveNode(char* chIn, char** chOut, int nodePosition)
{
    char* nodeContent = new char[MAX_GRAMMER];
    memset(nodeContent, 0, MAX_GRAMMER);
    int index = 0;
    while(*chIn)
    {
        if(index <nodePosition)
            nodeContent[index] = (*chIn);
        else if(index == nodePosition) {} // 不能加break或者continue，break会直接停止循环，continue会无限循环
        else if(index - nodePosition == 1) // @与后面的字符调换位置
        {
            nodeContent[index-1] = (*chIn);
            nodeContent[index] = '@';
        }
        else
            nodeContent[index] = (*chIn);

        index++;
        chIn++;
    }

    memcpy(*chOut, nodeContent, strlen(nodeContent));
    delete nodeContent;
}

void printNFA()
{
    cout<<endl<<"*************************************  NFA  *************************************"<<endl;
    int itemVectorLen = items.size();
    cout<<setw(4)<<" ";
    for(int k=0; k<itemVectorLen; k++)
        cout<<setw(4)<<k;
    cout<<endl;
    for(int i=0; i<itemVectorLen; i++)
    {
        cout<<setw(4)<<i;
        for(int j=0; j<itemVectorLen; j++)
            cout<<setw(4)<<NFA[i][j];
        cout<<endl<<endl;
    }
}

// NFA 邻接矩阵生成
void generateNFA()
{
    for (vector<itemStruct>::iterator iter = items.begin(); iter != items.end(); iter++)
    {
        char* tempContent = (*iter).content; // 取出项目的每一项
        int tempNodePosition = getNodePosition(tempContent);
        int row = (*iter).nodeId;
        char positionContent = tempContent[tempNodePosition+1];

        if(isTerminator(positionContent)) // 是终结符，没有空边
        {
            char* moveNodeContent = new char[MAX_GRAMMER];
            memset(moveNodeContent,  0, MAX_GRAMMER);
            moveNode(tempContent, &moveNodeContent, tempNodePosition); // 将点右移一位，与item匹配，匹配成功的加入NFA，即E->@aA _____a____E->a@A,匹配E->a@A
            for(vector<itemStruct>::iterator tempIter = items.begin(); tempIter != items.end(); tempIter++)
            {
                if(0 == strcmp(moveNodeContent, (*tempIter).content))
                {
                    char nextChar = getNodePrevCharPosition(moveNodeContent); // 移动后点在终结符之前，需要取@上一个字符
                    NFA[row][(*tempIter).nodeId] = nextChar;
                }
            }
            delete moveNodeContent;
        }
        else
        {
            char* moveNodeContent = new char[MAX_GRAMMER];
            memset(moveNodeContent,  0, MAX_GRAMMER);
            moveNode(tempContent, &moveNodeContent, tempNodePosition); // 将点右移一位，与item匹配，匹配成功的加入NFA
//            cout<<moveNodeContent<<endl;
            for(vector<itemStruct>::iterator tempIter = items.begin(); tempIter != items.end(); tempIter++)
            {
                int nonTerminateNodePosition = getNodePosition((*tempIter).content); // 查遍历的那个文法的点，也就是空边指向的文法的点，1则为空边
                if(0 == strcmp(moveNodeContent, (*tempIter).content))//S->@E ____E____S->E@,匹配S->E@
                {
                    char nextChar = getNodePrevCharPosition(moveNodeContent);
                    NFA[row][(*tempIter).nodeId] = nextChar;
                }
                // S->@EB ____e____E->@aA,空边需要用特殊符号标记
                else if((0 == strcmp(getNextChar(tempContent), getFirstChar((*tempIter).content))) && (1 == nonTerminateNodePosition))
                    NFA[row][(*tempIter).nodeId] = '~';

            }
            delete moveNodeContent;
        }

        delete tempContent;
    }
    printNFA();
}

int isItemInClosure(closureStruct c, int nextItemId)
{
    for(int i=0; i<c.itemIds.size(); i++)
    {
        if(c.itemIds[i] == nextItemId)
            return 1;
    }
    return 0;
}

// 空边递归
void closureNullEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct)
{
    for(int nextItemId = 0; nextItemId < items.size(); nextItemId ++ )
    {
        // 空边合并至closurem。sourceItemId!=nextItemId是防止出现无限递归的情况，因为自己不可能给自己加空边！
        if('~' == NFA[sourceItemId][nextItemId] && sourceItemId!=nextItemId)
        {
            if(!isItemInClosure(c, nextItemId))
            {
                c.itemIds.push_back(nextItemId);
//                cout<<"NULL EDGE: closure:"<<closureVectorIndex<<"source id: "<<sourceItemId<<" closure item id: "<<nextItemId<<endl;
                items[nextItemId].isAdd = true;
                isAddDataToClosureStruct = true;
                items[nextItemId].closureIds.push_back(closureVectorIndex); // 将closureId加入item结构体中
                closureNullEdgeRecursion(c, closureVectorIndex, nextItemId, isAddDataToClosureStruct);
            }
        }
    }
}

// 非空边递归
void closureEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct)
{
    for(int nextItemId = 0; nextItemId < items.size(); nextItemId ++ )
    {
        if ('\0' != NFA[sourceItemId][nextItemId] && '~' != NFA[sourceItemId][nextItemId]) // 只要存在字符串，说明该i状态一定有后继，因此加入closure
        {
            c.itemIds.push_back(sourceItemId);
//            cout<<"EDGE source: closure: "<<closureVectorIndex<<"source id: "<<sourceItemId<<" closure item id: "<<nextItemId<<endl;
            items[sourceItemId].isAdd = true;
            isAddDataToClosureStruct = true;
            items[sourceItemId].closureIds.push_back(closureVectorIndex); // 将closureId加入item结构体中
        }
    }
}

// closure 控制 *****************************************************************************
int closureController(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct)
{
    // 如果该点已经被加入closure，不再以它为源来寻找后继 这句话不对！
    if(items[sourceItemId].isAdd == true)
        return -1;

    closureNullEdgeRecursion(c, closureVectorIndex, sourceItemId, isAddDataToClosureStruct);

    closureEdgeRecursion(c, closureVectorIndex, sourceItemId, isAddDataToClosureStruct);

    return 0;
}


// 生成Closure
void generateClosure()
{
    int closureVectorIndex = 0; // closure vector 下标
    bool isAddDataToClosureStruct = false; // 是否向closure添加数据，如果是，存入vector closure
    for(int i=0; i<items.size(); i++)
    {
        closureStruct c;
        c.closureId = closureVectorIndex;

        int result = closureController(c, closureVectorIndex, i, isAddDataToClosureStruct);

        if(isAddDataToClosureStruct)
        {
            closures.push_back(c); // 加入到closure中。
            closureVectorIndex++;
            isAddDataToClosureStruct = false;
        }
        // 如果没有~或者终结符/非终结符，说明这行为空，则可能为规约状态。
        // result=0为了判断是否是空行，如果不加该变量会出现一个问题，在closureController函数中，被添加到closure的会直接返回
        // 下面的else会认为是空行
        else if(result == 0)
        {
            c.itemIds.push_back(i);
            items[i].isAdd = true;
            items[i].closureIds.push_back(closureVectorIndex);
            closures.push_back(c);
            closureVectorIndex++;
            isAddDataToClosureStruct = false;
        }
    }
}


void printClosure()
{
    cout<<"******************************  closure ******************************"<<endl;

    for(vector<closureStruct>::iterator tempIter = closures.begin(); tempIter != closures.end(); tempIter++)
        cout<<"closure id: "<<(*tempIter).closureId<<", closure content length: "<<(*tempIter).itemIds.size()<<endl;

    cout<<"**********************************************************************"<<endl;

    cout<<endl<<"*************************  closure  **************************"<<endl;
    for(vector<closureStruct>::iterator closureStructIter = closures.begin(); closureStructIter != closures.end(); closureStructIter++)
    {
        cout<<"closure id: "<<(*closureStructIter).closureId<<" size : "<<(*closureStructIter).itemIds.size()<<endl;
        for(vector<int>::iterator intIter = (*closureStructIter).itemIds.begin(); intIter != (*closureStructIter).itemIds.end(); intIter++)
            cout<<"    itemId: "<<(*intIter)<< "  "<<endl;
        cout<<endl;
    }
}

void printClosureMerge()
{
//    cout<<"******************************  closure Merge  ******************************"<<endl;
//
//    for(vector<closureStruct>::iterator tempIter = closuresMerge.begin(); tempIter != closuresMerge.end(); tempIter++)
//        cout<<"closure id: "<<(*tempIter).closureId<<", closure content length: "<<(*tempIter).itemIds.size()<<endl;
//
//    cout<<"**********************************************************************"<<endl;

    cout<<endl<<"*************************   Closure  **************************"<<endl;
    for(vector<closureStruct>::iterator closureStructIter = closuresMerge.begin(); closureStructIter != closuresMerge.end(); closureStructIter++)
    {
        cout<<"closure id: "<<(*closureStructIter).closureId<<" size : "<<(*closureStructIter).itemIds.size()<<endl;
        for(vector<int>::iterator intIter = (*closureStructIter).itemIds.begin(); intIter != (*closureStructIter).itemIds.end(); intIter++)
            cout<<"    itemId: "<<(*intIter)<< "  "<<endl;
        cout<<endl;
    }
}

void printItemClosure()
{
    cout<<endl<<"*************************  item's closure {所属closure} **************************"<<endl;
    for(vector<itemStruct>::iterator itemStructIter = items.begin(); itemStructIter != items.end(); itemStructIter++)
    {
        cout<<"item id: "<<(*itemStructIter).nodeId<<" size : "<<(*itemStructIter).closureIds.size()<<endl;
        for(vector<int>::iterator intIter = (*itemStructIter).closureIds.begin(); intIter != (*itemStructIter).closureIds.end(); intIter++)
            cout<<"    closureId: "<<(*intIter)<< "  "<<endl ;
        cout<<endl;
    }
}

void printDFA()
{
    cout<<endl<<"*************************  DFA  **************************"<<endl;
    int closuresLen = closures.size();
    cout<<setw(4)<<" ";
    for(int k=0; k<closuresLen; k++)
        cout<<setw(4)<<k;
    cout<<endl;
    for(int i=0; i<closuresLen; i++)
    {
        cout<<setw(4)<<i;
        for(int j=0; j<closuresLen; j++)
            cout<<setw(4)<<DFA[i][j];
        cout<<endl<<endl;
    }
}

void printMergeDFA()
{
    cout<<endl<<"*************************  DFA Merge  **************************"<<endl;
    int closuresLen = closuresMerge.size();
    cout<<setw(4)<<" ";
    for(int k=0; k<closuresLen; k++)
        cout<<setw(4)<<k;
    cout<<endl;
    for(int i=0; i<closuresLen; i++)
    {
        cout<<setw(4)<<i;
        for(int j=0; j<closuresLen; j++)
            cout<<setw(4)<<DFA[i][j];
        cout<<endl<<endl;
    }
}

// 生成DFA
void generateDFA()
{
    int itemVectorLen = items.size();
    // closureStruct的迭代器
    for(vector<closureStruct>::iterator closureStructIter = closures.begin(); closureStructIter != closures.end(); closureStructIter++)
    {
        // closureStruct vector 内部的 itemIds vector 迭代器
        for(vector<int>::iterator closureStructItemIdIter = (*closureStructIter).itemIds.begin(); closureStructItemIdIter != (*closureStructIter).itemIds.end(); closureStructItemIdIter++)
        {
            int closureStructItemStructIndex = *closureStructItemIdIter; // closure下的itemId项下标
            for(int j=0; j<itemVectorLen; j++) // 遍历closureStructItemId指向的item的后继
            {
                if(NFA[closureStructItemStructIndex][j] != '\0' && NFA[closureStructItemStructIndex][j] != '~') // 找closureStructItemStructIndex = 0项目的后继
                {
                    int subItemNodeId = j; // 后继的id
                    int closureIdsSize = items[subItemNodeId].closureIds.size(); // 纵坐标是后继的下标，此时寻找后继所在的closure
                    if(closureIdsSize != 1)
                        cout<<"后继："<<subItemNodeId<<"  "<<*closureStructItemIdIter<<"  "<<closureIdsSize<<"  Can't generate DFA!"<<endl;
                    else
                    {
                        int dfaRow = (*closureStructIter).closureId; // DFA节点的横坐标，是closure vector迭代器中的closureId
                        int dfaCol = items[subItemNodeId].closureIds[0]; // 有向边连接的项目所在的closure，即从item结构体取出closureId
                        int nfaRow = closureStructItemStructIndex; // closure下的itemId项下标，即前驱
                        int nfaCol = subItemNodeId; // nfa 后继节点坐标
                        DFA[dfaRow][dfaCol] = NFA[nfaRow][nfaCol];
                    }
                }
            }
        }
    }
    printDFA();
}

// 合并closure
void mergeClosure()
{
    cout<<"******************************  合并过程  ******************************"<<endl;
    int closureLen = closures.size();
    int closureIndex = 0;

    for(vector<itemStruct>::iterator itemClearIter = items.begin(); itemClearIter != items.end(); itemClearIter++)
    {
        (*itemClearIter).closureIds.clear();
        (*itemClearIter).isAdd = false;
    }

    for(vector<closureStruct>::iterator iter = closures.begin(); iter != closures.end(); iter++)
    {
        if((*iter).itemIds.size()!=1)
        {
            (*iter).closureId = closureIndex;
            closuresMerge.push_back(*iter);

            for(vector<int>::iterator intIter = (*iter).itemIds.begin(); intIter != (*iter).itemIds.end(); intIter++)
            {
                items[(*intIter)].closureIds.push_back((*iter).closureId);
                items[(*intIter)].isAdd = true;
            }
            closureIndex++;
        }
    }

    int countIndex = 0;
    for(int i=0; i<closureLen; i++) // 时间复杂度太大!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        for(int j=0; j<closureLen; j++)
        {
            if(j == countIndex)
            {
                countIndex = 0;
                continue;
            }

            if(!isTerminator(DFA[i][j]))
            {
                for(int k=0; k<closureLen; k++)
                {
                    if(DFA[i][k] == DFA[i][j] && j != k && DFA[i][k]!='\0')
                    {
                        cout<<"合并("<<i<<","<<j<<"); ("<<i<<","<<k<<")"<<endl;
                        closureStruct c;
                        c.itemIds.push_back(closures[j].itemIds[0]); // 此时应该是1个元素，（如果照例子，可能有错误）
                        c.itemIds.push_back(closures[k].itemIds[0]);
                        c.closureId = closureIndex;
                        closuresMerge.push_back(c);

                        items[closures[j].itemIds[0]].closureIds.push_back(closureIndex);
                        items[closures[j].itemIds[0]].isAdd = true;
                        items[closures[k].itemIds[0]].closureIds.push_back(closureIndex);
                        items[closures[k].itemIds[0]].isAdd = true;

                        closureIndex++;
                        countIndex = k;

                        for(int m=i+1; m<closureLen; m++)
                        {
                            if(DFA[m][j] == DFA[i][j])
                            {
                                if(DFA[m][k] == DFA[i][k])
                                {
                                    DFA[m][j] = '\0';
                                    DFA[m][k] = '\0'; // 双等于？？？？？？？？？？？？？？？？极其低级错误！
                                    cout<<"删除("<<m<<","<<j<<"); ("<<m<<","<<k<<")"<<endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(vector<itemStruct>::iterator itemIter = items.begin(); itemIter != items.end(); itemIter++)
    {
        if((*itemIter).isAdd == false)
        {
            closureStruct c;
            c.itemIds.push_back((*itemIter).nodeId); // 此时应该是1个元素，（如果照例子，可能有错误）
            c.closureId = closureIndex;
            closuresMerge.push_back(c);

            (*itemIter).closureIds.push_back(closureIndex);
            (*itemIter).isAdd = true;

            closureIndex++;
        }
    }

    printClosureMerge();
}

int isEdgeInThisClosure(int prevClosure, int itemId)
{
    // itemClosureIdIter 为 closure id, 此时获取了Item所在的一个Closure中的closureId  I1
    for(vector<int>::iterator itemClosureIdIter = items[itemId].closureIds.begin(); itemClosureIdIter != items[itemId].closureIds.end(); itemClosureIdIter++)
    {
        int counts = 0;
        // closureIdIter 为 item id, 此时获取了Closure   I1    里一个item的id
        for(vector<int>::iterator closureIdIter = closures[(*itemClosureIdIter)].itemIds.begin(); closureIdIter != closures[(*itemClosureIdIter)].itemIds.end(); closureIdIter++)
        {
            // prevClosureIdIter 为 item id, 此时获取了前一个Closure里一个item的id I0
            for(vector<int>::iterator prevClosureIdIter = closures[prevClosure].itemIds.begin(); prevClosureIdIter != closures[prevClosure].itemIds.end(); prevClosureIdIter++)
            {
                if(itemId == (*closureIdIter))
                    break;
                if(NFA[(*prevClosureIdIter)][(*closureIdIter)] == '\0' || NFA[(*prevClosureIdIter)][(*closureIdIter)] == '~')
                    continue;
                else if(isTerminator(NFA[(*prevClosureIdIter)][(*closureIdIter)]))
                    continue;
                else if(!isTerminator(NFA[(*prevClosureIdIter)][(*closureIdIter)]))
                {
                    counts++;
                    break;
                }
            }
        }
        if(counts == (closures[(*itemClosureIdIter)].itemIds.size() - 1)) // I1 - 1 = counts
        {
            cout<<" 当前项目："<<itemId<<" 所属Closure："<<(*itemClosureIdIter)<<endl;
            return (*itemClosureIdIter);
        }
        else
            counts = 0;
    }
    return -1;

}

// 生成DFA
void generateDFAMerge()
{
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(DFA[i], 0, MAX_ITEM_NUM*sizeof(char));

    int itemVectorLen = items.size();
    // closureStruct的迭代器
    for(vector<closureStruct>::iterator closureStructIter = closuresMerge.begin(); closureStructIter != closuresMerge.end(); closureStructIter++)
    {
        // closureStruct vector 内部的 itemIds vector 迭代器
        for(vector<int>::iterator closureStructItemIdIter = (*closureStructIter).itemIds.begin(); closureStructItemIdIter != (*closureStructIter).itemIds.end(); closureStructItemIdIter++)
        {
            int closureStructItemStructIndex = *closureStructItemIdIter; // closure下的itemId项下标
            for(int j=0; j<itemVectorLen; j++) // 遍历closureStructItemId找item指向的后继
            {
                if(NFA[closureStructItemStructIndex][j] != '\0' && NFA[closureStructItemStructIndex][j] != '~') // 找closureStructItemStructIndex = 0项目的后继
                {
                    int subItemNodeId = j; // 后继的id
                    int closureIdsSize = items[subItemNodeId].closureIds.size(); // 纵坐标是后继的下标，此时寻找后继所在的closure
                    if(closureIdsSize != 1)
                    {
//                        int result = isEdgeInThisClosure(closureStructItemStructIndex, subItemNodeId);
//                        if(result == -1)
//                            cout<<"没有找到Closure的后继"<<endl;
//                        else
//                        {
//                            int dfaRow = (*closureStructIter).closureId; // DFA节点的横坐标，是closure vector迭代器中的closureId
//                            int dfaCol = result; // 有向边连接的项目所在的closure，即从item结构体取出closureId
//                            int nfaRow = closureStructItemStructIndex; // closure下的itemId项下标，即前驱
//                            int nfaCol = subItemNodeId; // nfa 后继节点坐标
//                            DFA[dfaRow][dfaCol] = NFA[nfaRow][nfaCol];
//                        }

                    }
                    else
                    {
                        int dfaRow = (*closureStructIter).closureId; // DFA节点的横坐标，是closure vector迭代器中的closureId
                        int dfaCol = items[subItemNodeId].closureIds[0]; // 有向边连接的项目所在的closure，即从item结构体取出closureId
                        int nfaRow = closureStructItemStructIndex; // closure下的itemId项下标，即前驱
                        int nfaCol = subItemNodeId; // nfa 后继节点坐标
                        DFA[dfaRow][dfaCol] = NFA[nfaRow][nfaCol];
                    }
                }
            }
        }
    }
    printMergeDFA();
}

/****************************  NFA Closure DFA 生成 结束  ********************************/

/********************************  SLR 分析表 生成 开始  **********************************/

void printFirst()
{
    cout<<"*******************  FIRST  *****************"<<endl;
    for(int i=0; i<strlen(nonTerminator); i++)
    {
        char firstChar = getNumInNonTerminator(i);
        if(firstChar != '\0')
            cout<<firstChar<<"的FIRST集 : FIRST={";
        for(int j=0; i<MAX_ITEM_NUM; j++)
        {
            if(FIRST[i][j] == '\0')
                break;
            cout<<setw(3)<<FIRST[i][j];
        }
        cout<<"}; "<<endl;
    }
    cout<<"*******************************************"<<endl;
}

void printFollow()
{
    cout<<"*******************  FOLLOW  *****************"<<endl;
    for(int i=0; i<strlen(nonTerminator); i++)
    {
        char firstChar = getNumInNonTerminator(i);
        if(firstChar != '\0')
            cout<<firstChar<<"的FOLLOW集 : FOLLOW={";
        for(int j=0; i<MAX_ITEM_NUM; j++)
        {
            if(FOLLOW[i][j] == '\0')
                break;
            cout<<setw(3)<<FOLLOW[i][j];
        }
        cout<<"}; "<<endl;
    }
    cout<<"*******************************************"<<endl;
}


// 添加到First集
void addToFirst(int nonTerminatorIndex, char ch)
{
    int i = 0; // 循环求出当前非终结符的FIRST集大小
    bool isAddToFirst = false;
    while(FIRST[nonTerminatorIndex][i] != '\0')
    {
        if(FIRST[nonTerminatorIndex][i] == ch)
        {
            isAddToFirst = true;
            break;
        }
        i++;
    }
    if(!isAddToFirst)
        FIRST[nonTerminatorIndex][i] = ch;

}

// 复制FIRST集
void copyFirstSet(int dest, int source)
{
    int i = 0;
    while(FIRST[source][i] != '\0')
    {
        addToFirst(dest, FIRST[source][i]);
        i++;
    }
}

// 寻找当前非终结符的First集
void findFirstSetChar(char ch)
{
    for(int i=0; i<grammarOriginalLength; i++)
    {
        if(grammars[i][0] == ch) // 是否是将要求的终结符
        {
            if(isTerminator(grammars[i][1])) // S->X x是终结符，直接加入FIRST集
                addToFirst(getCharInNonTerminator(ch), grammars[i][1]); // getCharInTerminator(ch)为所求非终结符下标，grammars[i][1]终结符符号
            else if(grammars[i][0] != grammars[i][1])
            {
                findFirstSetChar(grammars[i][1]); // 需要判断是否为自己，如果是S->E, E->E+F; 会无限递归。
                copyFirstSet(getCharInNonTerminator(ch), getCharInNonTerminator(grammars[i][1]));
            }
        }
    }
}

void generateFirst()
{
    for(int i=0; i<strlen(nonTerminator); i++)
        findFirstSetChar(nonTerminator[i]);

    printFirst();
}

// 初始化Follow 集, 起始符号加入 #
void initFollow()
{
    int index = getCharInNonTerminator(grammars[1][0]);
    FOLLOW[index][0] = '#';
}

// 添加到Follow集
void addToFollow(int nonTerminatorIndex, char ch)
{
    int i = 0; // 循环求出当前非终结符的FOLLOW集大小
    bool isAddToFirst = false;
    while(FOLLOW[nonTerminatorIndex][i] != '\0')
    {
        if(FOLLOW[nonTerminatorIndex][i] == ch)
        {
            isAddToFirst = true;
            break;
        }
        i++;
    }
    if(!isAddToFirst)
    {
        FOLLOW[nonTerminatorIndex][i] = ch;
        ifFollowChange = true;
    }
}

void copyFollow(int sourceFollowIndex, int copyFollowIndex, bool ifCopyFollow)
{
    int k=0;
    if(ifCopyFollow)
    {
        while(FOLLOW[copyFollowIndex][k] != '\0')
        {
            addToFollow(sourceFollowIndex, FOLLOW[copyFollowIndex][k]);
            k++;
        }
    }
    else
    {
        while(FIRST[copyFollowIndex][k] != '\0')
        {
            addToFollow(sourceFollowIndex, FIRST[copyFollowIndex][k]);
            k++;
        }
    }
}

void findFollowSetChar()
{
    for(int i=1; i<grammarOriginalLength; i++) // 忽略增广文法增加的标记S
    {
        int j = 1;
        while(grammars[i][j]!='\0')
        {
            if(!isTerminator(grammars[i][j])) // S->E+F, 中的E (非终结符)
            {
                int sourceFollowIndex = getCharInNonTerminator(grammars[i][j]); // E 下标
                if(isTerminator(grammars[i][j+1])) // S->E+F 中的 + (终结符)
                    addToFollow(sourceFollowIndex, grammars[i][j+1]);
                else if(grammars[i][j+1] != '\0') // S->EF 中的E， 把F FIRST 中元素中的加入FOLLOW集 (非终结符)
                {
                    int copyFollowIndex = getCharInNonTerminator(grammars[i][j+1]); // 准备复制 F FIRST 集
                    copyFollow(sourceFollowIndex, copyFollowIndex, false);
                }
                else if(grammars[i][j+1] == '\0') // S->E 中的 E， 把S FOLLOW 中元素加入FOLLOW集 (空)
                {
                    int copyFollowIndex = getCharInNonTerminator(grammars[i][0]); // 准备复制 S 的FOLLOW 集
                    copyFollow(sourceFollowIndex, copyFollowIndex, true);
                }
            }
            j++;
        }
    }
}

void generateFollow()
{
    initFollow();
    do
    {
        ifFollowChange = false;
        findFollowSetChar();
    }
    while(ifFollowChange);

    printFollow();
}

// 遍历一行,寻找应转移的状态
int getAChar(int col, char ch)
{
    int closuresLen = closures.size();
    for(int i=0; i<closuresLen; i++)
    {
        if(ch == DFA[col][i])
            return i;
    }
    return -1;
}

// 查找符号在非终结符的位置
int getCharInNonTerminator(char ch)
{
    int len = strlen(nonTerminator);
    for(int i=0; i<len; i++)
    {
        if(ch == nonTerminator[i])
            return i;
    }
    return -1;
}

// 查找符号在终结符的位置
int getCharInTerminator(char ch)
{
    int len = strlen(terminator);
    for(int i=0; i<len; i++)
    {
        if(ch == terminator[i])
            return i;
    }
    return -1;
}

// 查找数字在终结符的符号
char getNumInNonTerminator(int num)
{
    return nonTerminator[num];
}

// 查找数字在终结符的符号
char getNumInTerminator(int num)
{
    return terminator[num];
}

// 初始化矩阵
void initMartix()
{
    int closuresLen = closures.size();
    for(int i=0; i<closuresLen; i++)
    {
        for(int j=0; j<=strlen(terminator); j++)
            actionMatrix[i][j] = "\0";
    }
    for(int i=0; i<closuresLen; i++)
    {
        for(int j=0; j<=strlen(nonTerminator); j++)
            gotoMatrix[i][j] = "\0";
    }
}

// 打印slr
void printSLR()
{
    cout<<endl<<"*************************  SLR  **************************"<<endl;
    int closuresLen = closures.size();
    cout<<setw(6)<<" ";
    for(int m=0; m<strlen(terminator); m++)
        cout<<setw(6)<<terminator[m];
    cout<<setw(6)<<" ";
    for(int m=0; m<strlen(nonTerminator); m++)
        cout<<setw(6)<<nonTerminator[m];
    cout<<endl;

    for(int i=0; i<closuresLen; i++)
    {
        cout<<setw(6)<<i;
        for(int j=0; j<=strlen(terminator); j++)
            cout<<setw(6)<<actionMatrix[i][j];
        for(int k=0; k<=strlen(nonTerminator); k++)
            cout<<setw(6)<<gotoMatrix[i][k];
        cout<<endl;
    }
    cout<<"********************  action  ********************"<<endl;
    cout<<setw(6)<<" ";
    for(int m=0; m<strlen(terminator); m++)
        cout<<setw(6)<<terminator[m];
    cout<<setw(6)<<" ";
    cout<<endl;
    for(int i=0; i<closuresLen; i++)
    {
        cout<<setw(6)<<i;
        for(int j=0; j<=strlen(terminator); j++)
            cout<<setw(6)<<actionMatrix[i][j];
        cout<<endl;
    }
    cout<<"********************  goto  ********************"<<endl;
    cout<<setw(6)<<" ";
    for(int m=0; m<strlen(nonTerminator); m++)
        cout<<setw(6)<<nonTerminator[m];
    cout<<endl;
    for(int i=0; i<closuresLen; i++)
    {
        cout<<setw(6)<<i;
        for(int k=0; k<=strlen(nonTerminator); k++)
            cout<<setw(6)<<gotoMatrix[i][k];
        cout<<endl;
    }
}

// @的位置是否是最后
bool isNodeLast(int id)
{
    int len = strlen(items[id].content);
    if(items[id].content[len-1] == '@')
        return true;
    else
        return false;
}

// 获取归约到的文法表达式序号**********************important*****************/
int getReductionId(char* ch)
{
    int chLen = strlen(ch);
    char tempStr[chLen];
    memset(tempStr, 0, chLen + 3);
    int tempIndex = 0;
    int chIndex = 0;
    while(ch[chIndex]!='@')
    {
        if(chIndex == 0)
            tempStr[tempIndex] = ch[chIndex];
        if(chIndex == 1)
        {
            tempStr[tempIndex] = '-';
            tempStr[tempIndex + 1] = '>';
            tempStr[tempIndex + 2] = ch[chIndex];
            tempIndex += 2;
        }
        else
            tempStr[tempIndex] = ch[chIndex];

        tempIndex++;
        chIndex++;
    }

    cout<<"去掉@加入->后结果："<<tempStr<<endl;

    for(int i=0; i < MAX_GRAMMER; i++)
    {
        if(strcmp(tempStr, grammarOriginal[i].c_str()) == 0)
            return i;
    }
    return -1;
}

char getReductionChar(int index)
{
    return grammars[index][0];
}


// 判断是否归约，即@所在位置是否是最后一位
int isReduction(closureStruct cs)
{
    for(vector<int>::iterator iter = cs.itemIds.begin(); iter != cs.itemIds.end(); iter++)
    {
        if(isNodeLast(*iter))
        {
            int reductionGrammarId = getReductionId(items[*iter].content);
            cout<<"第"<<cs.closureId<<"个closure需要归约, 此时的字符串为"<<items[*iter].content<<"，需要归约到"<<reductionGrammarId<<"号！"<<endl;
            return reductionGrammarId;
        }
    }
    return -1;
}

// ACTION 函数
void setAction(int closurId, char inTChar)// 输入终结符
{
    int nextClosurId = getAChar(closurId, inTChar);
    int position = getCharInTerminator(inTChar); // 查找在终结符的位置
    stringstream tempStr;
    tempStr<<nextClosurId;
    string result = "s" + tempStr.str();
    actionMatrix[closurId][position] = result;
}


// GOTO 函数
void setGoto(int closurId, char inNTChar)// 输入非终结符
{
    int nextClosurId = getAChar(closurId, inNTChar);
    int position = getCharInNonTerminator(inNTChar); // 查找在终结符的位置
    stringstream tempStr;
    tempStr<<nextClosurId;
    string result = "g" + tempStr.str();
    gotoMatrix[closurId][position] = result;
}

// 需要归约的action
void reductionSetAction(int currentClosureId, int reductionGrammarId) // 归约到的符号
{

    stringstream tempStr;
    tempStr<<reductionGrammarId;
    string result = "r" + tempStr.str();
    int i=0;
    int reductionGrammarFirstCharId = getCharInNonTerminator(grammars[reductionGrammarId][0]);
    while(FOLLOW[reductionGrammarFirstCharId][i] != '\0')
    {
        for(int k=0; k<strlen(terminator); k++)
        {
            if(terminator[k] == FOLLOW[reductionGrammarFirstCharId][i])
                actionMatrix[currentClosureId][k] = result;
        }
        i++;
    }


}

// 生成SLR分析表
void generateSLR()
{
    initMartix();
    generateFirst();
    generateFollow();

    int closuresLen = closuresMerge.size();
    for(int i=0; i<closuresLen; i++)
    {
        int reductionGrammarId = isReduction(closuresMerge[i]);
        if(reductionGrammarId != -1)
            reductionSetAction(i, reductionGrammarId);

        for(int j=0; j<closuresLen; j++)
        {
            if(DFA[i][j] == '\0')
                continue;
            else if(getCharInNonTerminator(DFA[i][j]) != -1)
                setGoto(i, DFA[i][j]);
            else if(getCharInTerminator(DFA[i][j]) != -1)
                setAction(i, DFA[i][j]);
            else
                cout<<"SLR表构建错误!"<<endl;
        }
    }

    int acceptId = items[1].closureIds[0];
    int sharpPosition = getCharInTerminator('#');
    actionMatrix[acceptId][sharpPosition] = "a0";

    printSLR();
}
/********************************  SLR 分析表 生成 结束  **********************************/

/*************************************  语法分析 开始 *************************************/

// 初始化栈
void initStack()
{
    symbolStack.push('#');
    stateStack.push(0);
}

// "s1" 中的s 拆分
char splitChangeStateSymbol(string source)
{
    char result = source[0];
    return result;
}

// "s10" 中的10 拆分
int splitChangeStateNum(string source)
{
    int len = source.length();
    string resultStr = source.substr(1,len-1);
    stringstream tempStr;
    tempStr<<resultStr;
    int result = 0;
    tempStr>>result;
    return result;
}

// 获取栈应该弹出的个数
int getGrammerReductionLen(int index)
{
    return grammarOriginal[index].length() - 3;
}


// 语法分析
void analysisGrammer(char ch)
{
    cout<<setw(10)<<ch;
    int currStateTop = stateStack.top();
    char currSymbolTop = symbolStack.top();

    cout<<setw(10)<<currStateTop<<setw(10)<<currSymbolTop;
    int symbolIndex = 0; // 标识符在LR分析表中的下标
    string changeStateStr = "\0"; // 转换后的state
    if(isTerminator(ch))
    {
        symbolIndex = getCharInTerminator(ch);
        cout<<setw(10)<<"A ("<<currStateTop<<","<<symbolIndex<<");";
        changeStateStr = actionMatrix[currStateTop][symbolIndex];
        cout<<setw(10)<<changeStateStr;
    }

    else
    {
        symbolIndex = getCharInNonTerminator(ch);
        cout<<setw(10)<<"G ("<<currStateTop<<","<<symbolIndex<<");";
        changeStateStr = gotoMatrix[currStateTop][symbolIndex];
        cout<<setw(10)<<changeStateStr;
    }

    if(changeStateStr == "\0" )
        symbolStack.push(ch);
    else if(splitChangeStateSymbol(changeStateStr) == 's')
    {

        cout<<setw(20)<<"移进";
        stateStack.push(splitChangeStateNum(changeStateStr));
        symbolStack.push(ch);
        cout<<setw(30)<<splitChangeStateNum(changeStateStr)<<" 和 "<<ch<<" 在栈中 (Action)"<<endl;
    }

    else if(splitChangeStateSymbol(changeStateStr) == 'r')
    {
        cout<<setw(20)<<"归约";
        // POP时注意个数
        int reductionLen = getGrammerReductionLen(splitChangeStateNum(changeStateStr));
        for(int i=0; i<reductionLen; i++)
        {
            if(currStateTop != 0)
                stateStack.pop();
            else
                cout<<endl<<"到底了"<<endl;
            if(currSymbolTop != '#')
                symbolStack.pop();
            else
                cout<<endl<<"到底了"<<endl;
        }

        char reductionSymol = getReductionChar(splitChangeStateNum(changeStateStr));
        cout<<setw(30)<<"归约后的字符为:"<<reductionSymol<<endl;
        analysisGrammer(reductionSymol); // 归约完毕，但是输入流传来的符号还没有进行移进或归约
        analysisGrammer(ch);

    }
    else if(splitChangeStateSymbol(changeStateStr) == 'g')
    {
        cout<<setw(20)<<"移进";
        stateStack.push(splitChangeStateNum(changeStateStr));
        symbolStack.push(ch);
        cout<<setw(30)<<splitChangeStateNum(changeStateStr)<<" 和 "<<ch<<" 在栈中 (Goto)"<<endl;
    }
    else if(splitChangeStateSymbol(changeStateStr) == 'a')
    {
        cout<<endl<<endl<<"---------------------成功接收---------------------"<<endl;
        success = true;
    }
}

// 语法分析控制器
void analysisGrammerController(char* fileIn)
{
    initStack();

    char c = '\0';

    cout<<"**************************  语法分析过程  *****************************"<<endl<<endl;

    cout<<setw(10)<<"输入流"<<setw(10)<<"状态栈"<<setw(10)<<"符号栈"<<setw(10)<<"A/G"<<setw(20)<<"转移字符串";
    cout<<setw(16)<<"动作"<<setw(30)<<"动作完成后状态"<<endl<<endl;

    ifstream ifile(fileIn);
    while(ifile.get(c))
        analysisGrammer(c);


    analysisGrammer('#');

    if(!success)
        cout<<endl<<"----未成功接收----"<<endl;

    ifile.close();
}

/******************************************************************************************/

int main()
{
    char* fileIn = "grammar4.txt";
    char* charStreamfileIn = "result.txt";
    char* charStreamfileOut = "stream.txt";
    readGrammer(fileIn);
    addItemNodeFlag();
    generateNTerminators();
    generateCharStream(charStreamfileIn, charStreamfileOut);
    initGraph();
    generateNFA();
    generateClosure();
    generateDFA();
    mergeClosure();
    generateDFAMerge();
    generateSLR();
    analysisGrammerController(charStreamfileOut);
    return 0;
}
