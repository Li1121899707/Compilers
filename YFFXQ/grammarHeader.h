#ifndef GRAMMERHEADER_H_INCLUDED
#define GRAMMERHEADER_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <malloc.h>
#include <iomanip>
#include <map>
#include <stack>
#define MAX_GRAMMER 30 //�ķ���󳤶�
#define MAX_ITEM_NUM 30
#define MAX_CLOSURE 10
#define MAX_ACTION 30
#define MAX_GOTO 30
#define MAX_LETTER 26

using namespace std;

/**********************************************  �ṹ�嶨��  **********************************************/
// ��Ŀ�ṹ��
typedef struct itemStruct
{
    int nodeId; // �ڵ���
    char content[MAX_GRAMMER]; // �ڵ�����
    bool isAdd; //�Ƿ���ӵ�closure��
    vector<int> closureIds;
}itemStruct;

typedef struct closureStruct
{
    int closureId; // �ڵ���
     vector<int> itemIds;// closure �µ���Ŀid
}closureStruct;

typedef struct mergeNodeStruct
{
    int row; // ��
    int col; // ��
    char nonChar; // ���ս��
}mergeNodeStruct;

/**********************************************  ���� ����� ����  **********************************************/

int grammarOriginalLength = 0;
bool success = false;
bool ifFollowChange = false; // �ж�Follow�Ƿ�������

/*************  ӳ��  ************/
map<string, char> nonTerminatorMap;
map<string, char> terminatorMap;
map<string, char> keyWordMap;

/***********  ����  **************/
char capitalLetter[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z','\0'};
char letter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z','\0'};
char symbol[] = {'+', '-', '*', '/', '=', '<', '>', '%', '#', '&', '(', ')', ';', ',', '.', '[', ']', '"', '{', '}', ':', '\'','\0'};
string grammarOriginal[MAX_GRAMMER]; // Դ�ķ�
char nonTerminator[MAX_LETTER];
char terminator[MAX_LETTER];

/********  vector ����  **********/
vector<itemStruct> items; // ��ӵ�����Ŀ����
vector<closureStruct> closures;
vector<closureStruct> closuresMerge;
/**********  ��ά����  ***********/

char grammars[MAX_GRAMMER][MAX_GRAMMER]; // ���ļ��ж�ȡ�ķ���ȥ��->
char NFA[MAX_ITEM_NUM][MAX_ITEM_NUM];
char DFA[MAX_ITEM_NUM][MAX_ITEM_NUM];
string actionMatrix[MAX_ACTION][MAX_ACTION];
string gotoMatrix[MAX_GOTO][MAX_GOTO];

char FIRST[MAX_ITEM_NUM][MAX_ITEM_NUM];
char FOLLOW[MAX_ITEM_NUM][MAX_ITEM_NUM];

/*************  ջ  ************/
stack<char> symbolStack; // ����ջ
stack<char> stateStack; // ״̬ջ

/**********************************************  ��������  **********************************************/

/************* ��Ŀ����  **********/
vector<string> splitGrammar(const string &str,const string &pattern); // ����->�ָ��ַ���
void readGrammer(char* file); // ��ȡ�ķ�txt
void addItemNodeFlag(); // �����Ŀ���� �� ������@��ʾ

/************* NFA ����  **********/
void initGraph();// ��ʼ��NFA DFA FIRST FOLLOW
bool isTerminator(char ch); // �ж��Ƿ����ս��
int getNodePosition(char* ch); // ����@��λ��
char getNodePrevCharPosition(char* ch);// ����@ǰһ���ַ���λ��
char* getFirstChar(char* ch);// ���ҵ�һ���ַ�
char* getLastChar(char* ch);// �������һ���ַ�
char* getNextChar(char* ch);// ����@��һ���ַ�
void moveNode(char* chIn, char** chOut, int nodePosition);// ���ӵ����һλ
void printNFA();
void generateNFA(); // NFA �ڽӾ�������

/************* CLOSURE ����  **********/
int isItemInClosure(closureStruct c, int nextItemId);
void closureNullEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct);// �ձߵݹ�
void closureEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct);// �ǿձߵݹ�
int closureController(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct); // Closure����
void printClosure();
void printItemClosure();
void generateClosure(); // ����Closure

/************* DFA ����  **********/
void printDFA();
void generateDFA();

/************* FIRST FOLLOW �� ����  **********/
void printFirst();
void printFollow();
void addToFirst(int nonTerminatorIndex, char ch);// ��ӵ�First��
void copyFirstSet(int dest, int source);// ����FIRST��
void findFirstSetChar(char ch);// Ѱ�ҵ�ǰ���ս����First��
void generateFirst();
void initFollow();// ��ʼ��Follow ��, ��ʼ���ż��� #
void addToFollow(int nonTerminatorIndex, char ch);// ��ӵ�Follow��
void copyFollow(int sourceFollowIndex, int copyFollowIndex, bool ifCopyFollow);// ����FOLLOW��
void findFollowSetChar(); // Ѱ��FOLLOW ���е�Ԫ��
void generateFollow(); // ѭ����ֱ��FOLLOW��������

/************* SLR ������ ����  **********/
int getAChar(int col, char ch);// ����һ��,Ѱ��Ӧת�Ƶ�״̬
int getCharInNonTerminator(char ch);// ���ҷ����ڷ��ս����λ��
int getCharInTerminator(char ch); // ���ҷ������ս����λ��
char getNumInNonTerminator(int num); // �����������ս���ķ���
char getNumInTerminator(int num);// �����������ս���ķ���
void initMartix();// ��ʼ������action goto
void printSLR();
bool isNodeLast(int id);// @��λ���Ƿ������
int getReductionId(char* ch);//��ȡ��Լ�����ķ����ʽ���
char getReductionChar(int index);
int isReduction(closureStruct cs);// �ж��Ƿ��Լ����@����λ���Ƿ������һλ
void setAction(int closurId, char inTChar);// ACTION ���� �����ս��
void setGoto(int closurId, char inNTChar);// GOTO ����������ս��
void reductionSetAction(int currentClosureId, int reductionGrammarId); // ��Ҫ��Լ��action
void generateSLR();

/************* �﷨����  **********/
void initStack();// ��ʼ��ջ
char splitChangeStateSymbol(string source);// "s1" �е�s ���
int splitChangeStateNum(string source);// "s1" �е�1 ���
int getGrammerReductionLen(int index);// ��ȡջӦ�õ����ĸ���
void analysisGrammer(char ch);// �﷨����
void analysisGrammerController(char* fileIn);// �﷨����������


#endif // GRAMMERHEADER_H_INCLUDED
