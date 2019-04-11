#include "grammerHeader.h"

/*******************************  ��Ŀ���� ��ʼ **************************************/
// ����->�ָ��ַ���
vector<string> splitGrammar(const string &str,const string &pattern)
{
    //const char* ת to char*
    char* strc = new char[strlen(str.c_str())+1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char* tmpStr = strtok(strc, pattern.c_str()); //strtok�� �ָ��ַ���
    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str()); //strtok�� �ָ��ַ�������ʱ��һ��������ҪΪNULL
    }
    // �ͷſռ�
    delete[] strc;
    return resultVec;
}

// ��ȡ�ķ�txt
void readGrammer(char* file)
{
    // ��ʼ��grammar����Ϊ��
    for(int i=0; i<MAX_GRAMMER; i++ )
    {
        memset(grammars[i], 0, MAX_GRAMMER*sizeof(char));
        grammarOriginal[i] = "\0";
    }

    // ��ʼ��grammarOriginal����Ϊ��
    ifstream ifile(file);
    string s;
    string splitPattern = "->";
    int index = 0;
    // ���ж�ȡ�ķ������н���
    cout<<"***************  ���ļ��ж�ȡ�ķ�  ******************"<<endl;
    while(getline(ifile, s))
    {
        cout<<s<<endl;
        grammarOriginal[index] = s;
        vector<string> splitList = splitGrammar(s, splitPattern);
        // rowOffset���ڵ������ǣ�iter��һ�η��ر��ʽ��ߵģ��ڶ��βŷ����ұߵģ������Ҫ��ƫ��
        int rowOffset = 0;
        for (vector<string>::const_iterator iter = splitList.begin(); iter != splitList.end(); iter++)
        {
            char* temp = const_cast<char*>((*iter).c_str()); // string -> const char* -> char*
            int len = strlen(temp);
            for(int j=0; j<len; j++)
                grammars[index][rowOffset + j] = *(temp + j);
            delete temp; // �ͷ�ָ���ڴ�
            rowOffset = len;
        }
        index++; // �ķ�����һ��
        vector<string>().swap(splitList); // �ͷſռ�
    }

    grammarOriginalLength = index;

    // ��ӡ�ķ�
    int k = 0;
    cout<<"***************  ȥ��->�ķ�  ******************"<<endl;
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

// �����Ŀ . �� ������@��ʾ
void addItemNodeFlag()
{
    int i = 0;
    int nodeId = 0;
    cout<<"***************  ��ʾ������Ŀ�����  ******************"<<endl;
    cout<<setw(12)<<"itemId"<<setw(20)<<"itemContent"<<setw(18)<<"isAddToClosur"<<endl;

    while(grammars[i][0]!='\0')
    {
        int addPosition = 1; // ����->����Ϊ��һλ����˵���±�Ϊ1��λ�ÿ�ʼ��
        int grammarLen = strlen(grammars[i]);
        for(int k=0; k<grammarLen; k++)
        {
            char tempItem[MAX_GRAMMER];
            memset(tempItem, 0, MAX_GRAMMER);
            int tempItemIndex = 0; // ��ǰ��Ŀ�����±�
            for(int j=0; j<=grammarLen; j++)
            {
                if(j<addPosition)
                    tempItem[tempItemIndex] = grammars[i][j];
                else if(j == addPosition && j< grammarLen) // ��@�������һλ
                {
                    tempItem[tempItemIndex] = '@';
                    tempItem[tempItemIndex+1] = grammars[i][j];
                    tempItemIndex++;
                }
                else if(j < grammarLen)
                    tempItem[tempItemIndex] = grammars[i][j];
                else if(addPosition == grammarLen) // ��@�����һλ
                    tempItem[tempItemIndex] = '@';

                tempItemIndex++;
                tempItem[tempItemIndex] = '\0'; // ��ʼ����һλ
            }

            itemStruct i ;
            i.nodeId = nodeId;
            strcpy(i.content, tempItem);
            i.isAdd = false;
            cout<<setw(12)<<i.nodeId<<setw(20)<<i.content<<setw(18)<<i.isAdd<<endl;
            items.push_back(i); // ��vector���

            delete[] tempItem; // ���tempItem �ռ�

            nodeId++; // �ڵ�������
            addPosition++; // @ ��λ
        }

        i++; // �﷨����
    }
    cout<<"*******************************************************"<<endl;
}

/*******************************  ��Ŀ���� ����  **************************************/

/*******************************  �ַ������� ��ʼ  *************************************/

// �Ƿ��Ǵ�д��ĸ
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

// �Ƿ���Сд��ĸ
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

// �Ƿ��Ƿ���
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

// ���� terminator, nonTerminator
void generateNTerminators()
{

    initLetterMatrix();

    cout<<"�ķ��������ս������ս����"<<endl;
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
    // ���ж�ȡ�ķ������н���
    cout<<"***************  ����������  ******************"<<endl;
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

/*******************************  �ַ������� ����  *************************************/


/****************************  NFA Closure DFA ���� ��ʼ  ********************************/

// ��ʼ��NFA DFA FIRST FOLLOW
void initGraph()
{
    // ��ʼ��grammar����Ϊ��
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(NFA[i], 0, MAX_ITEM_NUM*sizeof(char));
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(DFA[i], 0, MAX_ITEM_NUM*sizeof(char));
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(FIRST[i], 0, MAX_ITEM_NUM*sizeof(char));
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(FOLLOW[i], 0, MAX_ITEM_NUM*sizeof(char));
}

// �ж��Ƿ����ս��
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

// ����@��λ��
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

// ����@ǰһ���ַ���λ��
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


// ���ҵ�һ���ַ�
char* getFirstChar(char* ch)
{
    char* p = new char[2];
    memset(p, 0, 2);
    *p = *ch;
    return p;
}

// �������һ���ַ�
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

// ����@��һ���ַ�
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


// ���ӵ����һλ
void moveNode(char* chIn, char** chOut, int nodePosition)
{
    char* nodeContent = new char[MAX_GRAMMER];
    memset(nodeContent, 0, MAX_GRAMMER);
    int index = 0;
    while(*chIn)
    {
        if(index <nodePosition)
            nodeContent[index] = (*chIn);
        else if(index == nodePosition) {} // ���ܼ�break����continue��break��ֱ��ֹͣѭ����continue������ѭ��
        else if(index - nodePosition == 1) // @�������ַ�����λ��
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

// NFA �ڽӾ�������
void generateNFA()
{
    for (vector<itemStruct>::iterator iter = items.begin(); iter != items.end(); iter++)
    {
        char* tempContent = (*iter).content; // ȡ����Ŀ��ÿһ��
        int tempNodePosition = getNodePosition(tempContent);
        int row = (*iter).nodeId;
        char positionContent = tempContent[tempNodePosition+1];

        if(isTerminator(positionContent)) // ���ս����û�пձ�
        {
            char* moveNodeContent = new char[MAX_GRAMMER];
            memset(moveNodeContent,  0, MAX_GRAMMER);
            moveNode(tempContent, &moveNodeContent, tempNodePosition); // ��������һλ����itemƥ�䣬ƥ��ɹ��ļ���NFA����E->@aA _____a____E->a@A,ƥ��E->a@A
            for(vector<itemStruct>::iterator tempIter = items.begin(); tempIter != items.end(); tempIter++)
            {
                if(0 == strcmp(moveNodeContent, (*tempIter).content))
                {
                    char nextChar = getNodePrevCharPosition(moveNodeContent); // �ƶ�������ս��֮ǰ����Ҫȡ@��һ���ַ�
                    NFA[row][(*tempIter).nodeId] = nextChar;
                }
            }
            delete moveNodeContent;
        }
        else
        {
            char* moveNodeContent = new char[MAX_GRAMMER];
            memset(moveNodeContent,  0, MAX_GRAMMER);
            moveNode(tempContent, &moveNodeContent, tempNodePosition); // ��������һλ����itemƥ�䣬ƥ��ɹ��ļ���NFA
//            cout<<moveNodeContent<<endl;
            for(vector<itemStruct>::iterator tempIter = items.begin(); tempIter != items.end(); tempIter++)
            {
                int nonTerminateNodePosition = getNodePosition((*tempIter).content); // ��������Ǹ��ķ��ĵ㣬Ҳ���ǿձ�ָ����ķ��ĵ㣬1��Ϊ�ձ�
                if(0 == strcmp(moveNodeContent, (*tempIter).content))//S->@E ____E____S->E@,ƥ��S->E@
                {
                    char nextChar = getNodePrevCharPosition(moveNodeContent);
                    NFA[row][(*tempIter).nodeId] = nextChar;
                }
                // S->@EB ____e____E->@aA,�ձ���Ҫ��������ű��
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

// �ձߵݹ�
void closureNullEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct)
{
    for(int nextItemId = 0; nextItemId < items.size(); nextItemId ++ )
    {
        // �ձߺϲ���closurem��sourceItemId!=nextItemId�Ƿ�ֹ�������޵ݹ���������Ϊ�Լ������ܸ��Լ��ӿձߣ�
        if('~' == NFA[sourceItemId][nextItemId] && sourceItemId!=nextItemId)
        {
            if(!isItemInClosure(c, nextItemId))
            {
                c.itemIds.push_back(nextItemId);
//                cout<<"NULL EDGE: closure:"<<closureVectorIndex<<"source id: "<<sourceItemId<<" closure item id: "<<nextItemId<<endl;
                items[nextItemId].isAdd = true;
                isAddDataToClosureStruct = true;
                items[nextItemId].closureIds.push_back(closureVectorIndex); // ��closureId����item�ṹ����
                closureNullEdgeRecursion(c, closureVectorIndex, nextItemId, isAddDataToClosureStruct);
            }
        }
    }
}

// �ǿձߵݹ�
void closureEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct)
{
    for(int nextItemId = 0; nextItemId < items.size(); nextItemId ++ )
    {
        if ('\0' != NFA[sourceItemId][nextItemId] && '~' != NFA[sourceItemId][nextItemId]) // ֻҪ�����ַ�����˵����i״̬һ���к�̣���˼���closure
        {
            c.itemIds.push_back(sourceItemId);
//            cout<<"EDGE source: closure: "<<closureVectorIndex<<"source id: "<<sourceItemId<<" closure item id: "<<nextItemId<<endl;
            items[sourceItemId].isAdd = true;
            isAddDataToClosureStruct = true;
            items[sourceItemId].closureIds.push_back(closureVectorIndex); // ��closureId����item�ṹ����
        }
    }
}

// closure ���� *****************************************************************************
int closureController(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct)
{
    // ����õ��Ѿ�������closure����������ΪԴ��Ѱ�Һ�� ��仰���ԣ�
    if(items[sourceItemId].isAdd == true)
        return -1;

    closureNullEdgeRecursion(c, closureVectorIndex, sourceItemId, isAddDataToClosureStruct);

    closureEdgeRecursion(c, closureVectorIndex, sourceItemId, isAddDataToClosureStruct);

    return 0;
}


// ����Closure
void generateClosure()
{
    int closureVectorIndex = 0; // closure vector �±�
    bool isAddDataToClosureStruct = false; // �Ƿ���closure������ݣ�����ǣ�����vector closure
    for(int i=0; i<items.size(); i++)
    {
        closureStruct c;
        c.closureId = closureVectorIndex;

        int result = closureController(c, closureVectorIndex, i, isAddDataToClosureStruct);

        if(isAddDataToClosureStruct)
        {
            closures.push_back(c); // ���뵽closure�С�
            closureVectorIndex++;
            isAddDataToClosureStruct = false;
        }
        // ���û��~�����ս��/���ս����˵������Ϊ�գ������Ϊ��Լ״̬��
        // result=0Ϊ���ж��Ƿ��ǿ��У�������Ӹñ��������һ�����⣬��closureController�����У�����ӵ�closure�Ļ�ֱ�ӷ���
        // �����else����Ϊ�ǿ���
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
    cout<<endl<<"*************************  item's closure {����closure} **************************"<<endl;
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

// ����DFA
void generateDFA()
{
    int itemVectorLen = items.size();
    // closureStruct�ĵ�����
    for(vector<closureStruct>::iterator closureStructIter = closures.begin(); closureStructIter != closures.end(); closureStructIter++)
    {
        // closureStruct vector �ڲ��� itemIds vector ������
        for(vector<int>::iterator closureStructItemIdIter = (*closureStructIter).itemIds.begin(); closureStructItemIdIter != (*closureStructIter).itemIds.end(); closureStructItemIdIter++)
        {
            int closureStructItemStructIndex = *closureStructItemIdIter; // closure�µ�itemId���±�
            for(int j=0; j<itemVectorLen; j++) // ����closureStructItemIdָ���item�ĺ��
            {
                if(NFA[closureStructItemStructIndex][j] != '\0' && NFA[closureStructItemStructIndex][j] != '~') // ��closureStructItemStructIndex = 0��Ŀ�ĺ��
                {
                    int subItemNodeId = j; // ��̵�id
                    int closureIdsSize = items[subItemNodeId].closureIds.size(); // �������Ǻ�̵��±꣬��ʱѰ�Һ�����ڵ�closure
                    if(closureIdsSize != 1)
                        cout<<"��̣�"<<subItemNodeId<<"  "<<*closureStructItemIdIter<<"  "<<closureIdsSize<<"  Can't generate DFA!"<<endl;
                    else
                    {
                        int dfaRow = (*closureStructIter).closureId; // DFA�ڵ�ĺ����꣬��closure vector�������е�closureId
                        int dfaCol = items[subItemNodeId].closureIds[0]; // ��������ӵ���Ŀ���ڵ�closure������item�ṹ��ȡ��closureId
                        int nfaRow = closureStructItemStructIndex; // closure�µ�itemId���±꣬��ǰ��
                        int nfaCol = subItemNodeId; // nfa ��̽ڵ�����
                        DFA[dfaRow][dfaCol] = NFA[nfaRow][nfaCol];
                    }
                }
            }
        }
    }
    printDFA();
}

// �ϲ�closure
void mergeClosure()
{
    cout<<"******************************  �ϲ�����  ******************************"<<endl;
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
    for(int i=0; i<closureLen; i++) // ʱ�临�Ӷ�̫��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
                        cout<<"�ϲ�("<<i<<","<<j<<"); ("<<i<<","<<k<<")"<<endl;
                        closureStruct c;
                        c.itemIds.push_back(closures[j].itemIds[0]); // ��ʱӦ����1��Ԫ�أ�����������ӣ������д���
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
                                    DFA[m][k] = '\0'; // ˫���ڣ�����������������������������������ͼ�����
                                    cout<<"ɾ��("<<m<<","<<j<<"); ("<<m<<","<<k<<")"<<endl;
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
            c.itemIds.push_back((*itemIter).nodeId); // ��ʱӦ����1��Ԫ�أ�����������ӣ������д���
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
    // itemClosureIdIter Ϊ closure id, ��ʱ��ȡ��Item���ڵ�һ��Closure�е�closureId  I1
    for(vector<int>::iterator itemClosureIdIter = items[itemId].closureIds.begin(); itemClosureIdIter != items[itemId].closureIds.end(); itemClosureIdIter++)
    {
        int counts = 0;
        // closureIdIter Ϊ item id, ��ʱ��ȡ��Closure   I1    ��һ��item��id
        for(vector<int>::iterator closureIdIter = closures[(*itemClosureIdIter)].itemIds.begin(); closureIdIter != closures[(*itemClosureIdIter)].itemIds.end(); closureIdIter++)
        {
            // prevClosureIdIter Ϊ item id, ��ʱ��ȡ��ǰһ��Closure��һ��item��id I0
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
            cout<<" ��ǰ��Ŀ��"<<itemId<<" ����Closure��"<<(*itemClosureIdIter)<<endl;
            return (*itemClosureIdIter);
        }
        else
            counts = 0;
    }
    return -1;

}

// ����DFA
void generateDFAMerge()
{
    for(int i=0; i<MAX_ITEM_NUM; i++ )
        memset(DFA[i], 0, MAX_ITEM_NUM*sizeof(char));

    int itemVectorLen = items.size();
    // closureStruct�ĵ�����
    for(vector<closureStruct>::iterator closureStructIter = closuresMerge.begin(); closureStructIter != closuresMerge.end(); closureStructIter++)
    {
        // closureStruct vector �ڲ��� itemIds vector ������
        for(vector<int>::iterator closureStructItemIdIter = (*closureStructIter).itemIds.begin(); closureStructItemIdIter != (*closureStructIter).itemIds.end(); closureStructItemIdIter++)
        {
            int closureStructItemStructIndex = *closureStructItemIdIter; // closure�µ�itemId���±�
            for(int j=0; j<itemVectorLen; j++) // ����closureStructItemId��itemָ��ĺ��
            {
                if(NFA[closureStructItemStructIndex][j] != '\0' && NFA[closureStructItemStructIndex][j] != '~') // ��closureStructItemStructIndex = 0��Ŀ�ĺ��
                {
                    int subItemNodeId = j; // ��̵�id
                    int closureIdsSize = items[subItemNodeId].closureIds.size(); // �������Ǻ�̵��±꣬��ʱѰ�Һ�����ڵ�closure
                    if(closureIdsSize != 1)
                    {
//                        int result = isEdgeInThisClosure(closureStructItemStructIndex, subItemNodeId);
//                        if(result == -1)
//                            cout<<"û���ҵ�Closure�ĺ��"<<endl;
//                        else
//                        {
//                            int dfaRow = (*closureStructIter).closureId; // DFA�ڵ�ĺ����꣬��closure vector�������е�closureId
//                            int dfaCol = result; // ��������ӵ���Ŀ���ڵ�closure������item�ṹ��ȡ��closureId
//                            int nfaRow = closureStructItemStructIndex; // closure�µ�itemId���±꣬��ǰ��
//                            int nfaCol = subItemNodeId; // nfa ��̽ڵ�����
//                            DFA[dfaRow][dfaCol] = NFA[nfaRow][nfaCol];
//                        }

                    }
                    else
                    {
                        int dfaRow = (*closureStructIter).closureId; // DFA�ڵ�ĺ����꣬��closure vector�������е�closureId
                        int dfaCol = items[subItemNodeId].closureIds[0]; // ��������ӵ���Ŀ���ڵ�closure������item�ṹ��ȡ��closureId
                        int nfaRow = closureStructItemStructIndex; // closure�µ�itemId���±꣬��ǰ��
                        int nfaCol = subItemNodeId; // nfa ��̽ڵ�����
                        DFA[dfaRow][dfaCol] = NFA[nfaRow][nfaCol];
                    }
                }
            }
        }
    }
    printMergeDFA();
}

/****************************  NFA Closure DFA ���� ����  ********************************/

/********************************  SLR ������ ���� ��ʼ  **********************************/

void printFirst()
{
    cout<<"*******************  FIRST  *****************"<<endl;
    for(int i=0; i<strlen(nonTerminator); i++)
    {
        char firstChar = getNumInNonTerminator(i);
        if(firstChar != '\0')
            cout<<firstChar<<"��FIRST�� : FIRST={";
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
            cout<<firstChar<<"��FOLLOW�� : FOLLOW={";
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


// ��ӵ�First��
void addToFirst(int nonTerminatorIndex, char ch)
{
    int i = 0; // ѭ�������ǰ���ս����FIRST����С
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

// ����FIRST��
void copyFirstSet(int dest, int source)
{
    int i = 0;
    while(FIRST[source][i] != '\0')
    {
        addToFirst(dest, FIRST[source][i]);
        i++;
    }
}

// Ѱ�ҵ�ǰ���ս����First��
void findFirstSetChar(char ch)
{
    for(int i=0; i<grammarOriginalLength; i++)
    {
        if(grammars[i][0] == ch) // �Ƿ��ǽ�Ҫ����ս��
        {
            if(isTerminator(grammars[i][1])) // S->X x���ս����ֱ�Ӽ���FIRST��
                addToFirst(getCharInNonTerminator(ch), grammars[i][1]); // getCharInTerminator(ch)Ϊ������ս���±꣬grammars[i][1]�ս������
            else if(grammars[i][0] != grammars[i][1])
            {
                findFirstSetChar(grammars[i][1]); // ��Ҫ�ж��Ƿ�Ϊ�Լ��������S->E, E->E+F; �����޵ݹ顣
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

// ��ʼ��Follow ��, ��ʼ���ż��� #
void initFollow()
{
    int index = getCharInNonTerminator(grammars[1][0]);
    FOLLOW[index][0] = '#';
}

// ��ӵ�Follow��
void addToFollow(int nonTerminatorIndex, char ch)
{
    int i = 0; // ѭ�������ǰ���ս����FOLLOW����С
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
    for(int i=1; i<grammarOriginalLength; i++) // ���������ķ����ӵı��S
    {
        int j = 1;
        while(grammars[i][j]!='\0')
        {
            if(!isTerminator(grammars[i][j])) // S->E+F, �е�E (���ս��)
            {
                int sourceFollowIndex = getCharInNonTerminator(grammars[i][j]); // E �±�
                if(isTerminator(grammars[i][j+1])) // S->E+F �е� + (�ս��)
                    addToFollow(sourceFollowIndex, grammars[i][j+1]);
                else if(grammars[i][j+1] != '\0') // S->EF �е�E�� ��F FIRST ��Ԫ���еļ���FOLLOW�� (���ս��)
                {
                    int copyFollowIndex = getCharInNonTerminator(grammars[i][j+1]); // ׼������ F FIRST ��
                    copyFollow(sourceFollowIndex, copyFollowIndex, false);
                }
                else if(grammars[i][j+1] == '\0') // S->E �е� E�� ��S FOLLOW ��Ԫ�ؼ���FOLLOW�� (��)
                {
                    int copyFollowIndex = getCharInNonTerminator(grammars[i][0]); // ׼������ S ��FOLLOW ��
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

// ����һ��,Ѱ��Ӧת�Ƶ�״̬
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

// ���ҷ����ڷ��ս����λ��
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

// ���ҷ������ս����λ��
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

// �����������ս���ķ���
char getNumInNonTerminator(int num)
{
    return nonTerminator[num];
}

// �����������ս���ķ���
char getNumInTerminator(int num)
{
    return terminator[num];
}

// ��ʼ������
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

// ��ӡslr
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

// @��λ���Ƿ������
bool isNodeLast(int id)
{
    int len = strlen(items[id].content);
    if(items[id].content[len-1] == '@')
        return true;
    else
        return false;
}

// ��ȡ��Լ�����ķ����ʽ���**********************important*****************/
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

    cout<<"ȥ��@����->������"<<tempStr<<endl;

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


// �ж��Ƿ��Լ����@����λ���Ƿ������һλ
int isReduction(closureStruct cs)
{
    for(vector<int>::iterator iter = cs.itemIds.begin(); iter != cs.itemIds.end(); iter++)
    {
        if(isNodeLast(*iter))
        {
            int reductionGrammarId = getReductionId(items[*iter].content);
            cout<<"��"<<cs.closureId<<"��closure��Ҫ��Լ, ��ʱ���ַ���Ϊ"<<items[*iter].content<<"����Ҫ��Լ��"<<reductionGrammarId<<"�ţ�"<<endl;
            return reductionGrammarId;
        }
    }
    return -1;
}

// ACTION ����
void setAction(int closurId, char inTChar)// �����ս��
{
    int nextClosurId = getAChar(closurId, inTChar);
    int position = getCharInTerminator(inTChar); // �������ս����λ��
    stringstream tempStr;
    tempStr<<nextClosurId;
    string result = "s" + tempStr.str();
    actionMatrix[closurId][position] = result;
}


// GOTO ����
void setGoto(int closurId, char inNTChar)// ������ս��
{
    int nextClosurId = getAChar(closurId, inNTChar);
    int position = getCharInNonTerminator(inNTChar); // �������ս����λ��
    stringstream tempStr;
    tempStr<<nextClosurId;
    string result = "g" + tempStr.str();
    gotoMatrix[closurId][position] = result;
}

// ��Ҫ��Լ��action
void reductionSetAction(int currentClosureId, int reductionGrammarId) // ��Լ���ķ���
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

// ����SLR������
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
                cout<<"SLR��������!"<<endl;
        }
    }

    int acceptId = items[1].closureIds[0];
    int sharpPosition = getCharInTerminator('#');
    actionMatrix[acceptId][sharpPosition] = "a0";

    printSLR();
}
/********************************  SLR ������ ���� ����  **********************************/

/*************************************  �﷨���� ��ʼ *************************************/

// ��ʼ��ջ
void initStack()
{
    symbolStack.push('#');
    stateStack.push(0);
}

// "s1" �е�s ���
char splitChangeStateSymbol(string source)
{
    char result = source[0];
    return result;
}

// "s10" �е�10 ���
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

// ��ȡջӦ�õ����ĸ���
int getGrammerReductionLen(int index)
{
    return grammarOriginal[index].length() - 3;
}


// �﷨����
void analysisGrammer(char ch)
{
    cout<<setw(10)<<ch;
    int currStateTop = stateStack.top();
    char currSymbolTop = symbolStack.top();

    cout<<setw(10)<<currStateTop<<setw(10)<<currSymbolTop;
    int symbolIndex = 0; // ��ʶ����LR�������е��±�
    string changeStateStr = "\0"; // ת�����state
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

        cout<<setw(20)<<"�ƽ�";
        stateStack.push(splitChangeStateNum(changeStateStr));
        symbolStack.push(ch);
        cout<<setw(30)<<splitChangeStateNum(changeStateStr)<<" �� "<<ch<<" ��ջ�� (Action)"<<endl;
    }

    else if(splitChangeStateSymbol(changeStateStr) == 'r')
    {
        cout<<setw(20)<<"��Լ";
        // POPʱע�����
        int reductionLen = getGrammerReductionLen(splitChangeStateNum(changeStateStr));
        for(int i=0; i<reductionLen; i++)
        {
            if(currStateTop != 0)
                stateStack.pop();
            else
                cout<<endl<<"������"<<endl;
            if(currSymbolTop != '#')
                symbolStack.pop();
            else
                cout<<endl<<"������"<<endl;
        }

        char reductionSymol = getReductionChar(splitChangeStateNum(changeStateStr));
        cout<<setw(30)<<"��Լ����ַ�Ϊ:"<<reductionSymol<<endl;
        analysisGrammer(reductionSymol); // ��Լ��ϣ����������������ķ��Ż�û�н����ƽ����Լ
        analysisGrammer(ch);

    }
    else if(splitChangeStateSymbol(changeStateStr) == 'g')
    {
        cout<<setw(20)<<"�ƽ�";
        stateStack.push(splitChangeStateNum(changeStateStr));
        symbolStack.push(ch);
        cout<<setw(30)<<splitChangeStateNum(changeStateStr)<<" �� "<<ch<<" ��ջ�� (Goto)"<<endl;
    }
    else if(splitChangeStateSymbol(changeStateStr) == 'a')
    {
        cout<<endl<<endl<<"---------------------�ɹ�����---------------------"<<endl;
        success = true;
    }
}

// �﷨����������
void analysisGrammerController(char* fileIn)
{
    initStack();

    char c = '\0';

    cout<<"**************************  �﷨��������  *****************************"<<endl<<endl;

    cout<<setw(10)<<"������"<<setw(10)<<"״̬ջ"<<setw(10)<<"����ջ"<<setw(10)<<"A/G"<<setw(20)<<"ת���ַ���";
    cout<<setw(16)<<"����"<<setw(30)<<"������ɺ�״̬"<<endl<<endl;

    ifstream ifile(fileIn);
    while(ifile.get(c))
        analysisGrammer(c);


    analysisGrammer('#');

    if(!success)
        cout<<endl<<"----δ�ɹ�����----"<<endl;

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
