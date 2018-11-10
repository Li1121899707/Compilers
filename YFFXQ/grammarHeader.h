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
#define MAX_GRAMMER 30 //文法最大长度
#define MAX_ITEM_NUM 30
#define MAX_CLOSURE 10
#define MAX_ACTION 30
#define MAX_GOTO 30
#define MAX_LETTER 26

using namespace std;

/**********************************************  结构体定义  **********************************************/
// 项目结构体
typedef struct itemStruct
{
    int nodeId; // 节点编号
    char content[MAX_GRAMMER]; // 节点内容
    bool isAdd; //是否添加到closure中
    vector<int> closureIds;
}itemStruct;

typedef struct closureStruct
{
    int closureId; // 节点编号
     vector<int> itemIds;// closure 下的项目id
}closureStruct;

typedef struct mergeNodeStruct
{
    int row; // 行
    int col; // 列
    char nonChar; // 非终结符
}mergeNodeStruct;

/**********************************************  变量 数组等 定义  **********************************************/

int grammarOriginalLength = 0;
bool success = false;
bool ifFollowChange = false; // 判断Follow是否再增大

/*************  映射  ************/
map<string, char> nonTerminatorMap;
map<string, char> terminatorMap;
map<string, char> keyWordMap;

/***********  数组  **************/
char capitalLetter[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z','\0'};
char letter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z','\0'};
char symbol[] = {'+', '-', '*', '/', '=', '<', '>', '%', '#', '&', '(', ')', ';', ',', '.', '[', ']', '"', '{', '}', ':', '\'','\0'};
string grammarOriginal[MAX_GRAMMER]; // 源文法
char nonTerminator[MAX_LETTER];
char terminator[MAX_LETTER];

/********  vector 数组  **********/
vector<itemStruct> items; // 添加点后的项目数组
vector<closureStruct> closures;
vector<closureStruct> closuresMerge;
/**********  二维数组  ***********/

char grammars[MAX_GRAMMER][MAX_GRAMMER]; // 从文件中读取文法并去掉->
char NFA[MAX_ITEM_NUM][MAX_ITEM_NUM];
char DFA[MAX_ITEM_NUM][MAX_ITEM_NUM];
string actionMatrix[MAX_ACTION][MAX_ACTION];
string gotoMatrix[MAX_GOTO][MAX_GOTO];

char FIRST[MAX_ITEM_NUM][MAX_ITEM_NUM];
char FOLLOW[MAX_ITEM_NUM][MAX_ITEM_NUM];

/*************  栈  ************/
stack<char> symbolStack; // 符号栈
stack<char> stateStack; // 状态栈

/**********************************************  函数定义  **********************************************/

/************* 项目生成  **********/
vector<string> splitGrammar(const string &str,const string &pattern); // 根据->分割字符串
void readGrammer(char* file); // 读取文法txt
void addItemNodeFlag(); // 添加项目点标记 ， 这里用@表示

/************* NFA 生成  **********/
void initGraph();// 初始化NFA DFA FIRST FOLLOW
bool isTerminator(char ch); // 判断是否是终结符
int getNodePosition(char* ch); // 返回@的位置
char getNodePrevCharPosition(char* ch);// 返回@前一个字符的位置
char* getFirstChar(char* ch);// 查找第一个字符
char* getLastChar(char* ch);// 查找最后一个字符
char* getNextChar(char* ch);// 查找@下一个字符
void moveNode(char* chIn, char** chOut, int nodePosition);// 将加点后移一位
void printNFA();
void generateNFA(); // NFA 邻接矩阵生成

/************* CLOSURE 生成  **********/
int isItemInClosure(closureStruct c, int nextItemId);
void closureNullEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct);// 空边递归
void closureEdgeRecursion(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct);// 非空边递归
int closureController(closureStruct& c, int closureVectorIndex, int sourceItemId, bool& isAddDataToClosureStruct); // Closure控制
void printClosure();
void printItemClosure();
void generateClosure(); // 生成Closure

/************* DFA 生成  **********/
void printDFA();
void generateDFA();

/************* FIRST FOLLOW 集 生成  **********/
void printFirst();
void printFollow();
void addToFirst(int nonTerminatorIndex, char ch);// 添加到First集
void copyFirstSet(int dest, int source);// 复制FIRST集
void findFirstSetChar(char ch);// 寻找当前非终结符的First集
void generateFirst();
void initFollow();// 初始化Follow 集, 起始符号加入 #
void addToFollow(int nonTerminatorIndex, char ch);// 添加到Follow集
void copyFollow(int sourceFollowIndex, int copyFollowIndex, bool ifCopyFollow);// 复制FOLLOW集
void findFollowSetChar(); // 寻找FOLLOW 集中的元素
void generateFollow(); // 循环，直到FOLLOW不再增大

/************* SLR 分析表 生成  **********/
int getAChar(int col, char ch);// 遍历一行,寻找应转移的状态
int getCharInNonTerminator(char ch);// 查找符号在非终结符的位置
int getCharInTerminator(char ch); // 查找符号在终结符的位置
char getNumInNonTerminator(int num); // 查找数字在终结符的符号
char getNumInTerminator(int num);// 查找数字在终结符的符号
void initMartix();// 初始化矩阵action goto
void printSLR();
bool isNodeLast(int id);// @的位置是否是最后
int getReductionId(char* ch);//获取归约到的文法表达式序号
char getReductionChar(int index);
int isReduction(closureStruct cs);// 判断是否归约，即@所在位置是否是最后一位
void setAction(int closurId, char inTChar);// ACTION 函数 输入终结符
void setGoto(int closurId, char inNTChar);// GOTO 函数输入非终结符
void reductionSetAction(int currentClosureId, int reductionGrammarId); // 需要归约的action
void generateSLR();

/************* 语法分析  **********/
void initStack();// 初始化栈
char splitChangeStateSymbol(string source);// "s1" 中的s 拆分
int splitChangeStateNum(string source);// "s1" 中的1 拆分
int getGrammerReductionLen(int index);// 获取栈应该弹出的个数
void analysisGrammer(char ch);// 语法分析
void analysisGrammerController(char* fileIn);// 语法分析控制器


#endif // GRAMMERHEADER_H_INCLUDED
