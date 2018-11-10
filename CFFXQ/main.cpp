#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#define MAX 256  //最大代码长度
using namespace std;

const char* const KEY_WORD[32]= {"char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "unoin", "unsigned",
                                "void", "for", "do", "while", "break", "continue", "if", "else", "goto", "switch", "case", "default",
                                "return", "auto", "extern", "register", "static", "const", "sizeof", "typedef", "volatile"
                               };//关键字
const char OPT_SINGLE[] = {'+', '-', '*', '/', '=', '<', '>', '%', '#', '&'};//单目运算符
const char* OPT_DOUBLE[] = {"<=", ">=", "==", "!=", "+=", "-=", "*=", "/=", "++", "--", "||", "&&"};//双目运算符
const char END[] = {'(', ')', ';', ',', '.', '[', ']', '"', '{', '}', ':', '\''};//界符

// 状态机转换
int stateChange(int state, char delimiter)
{
    if(state == 0 && delimiter == '/')
        return 1;
    if(state == 0 && delimiter == '"')
        return 5;
    if(state == 1 && delimiter == '/')
        return 2;
    if(state == 1 && delimiter == '*')
        return 3;
    if(state == 1 && delimiter == '"')
        return 5;
    if(state == 2 && delimiter == '\n')
        return 6;
    if(state == 3 && delimiter == '*')
        return 4;
    if(state == 4 && delimiter == '/')
        return 6;
    if(state == 5 && delimiter == '"')
        return 0;
    return state;
}

// 判断是否是状态机内的字符
bool isStateMark(char delimiter)
{
    if('/' == delimiter || '*' == delimiter || '"' == delimiter || '\n' == delimiter)
        return true;
    else
        return false;
}

// 判断是否是引号
bool isQuotationMark(char ch)
{
    if(ch=='"')
        return true;
    else
        return false;
}

// 判断是否是空格、Tab和回车
bool isSpaseTab(char ch)
{
    if(ch==' ' || ch=='\t' || ch=='\n')
        return true;
    else
        return false;
}

// 判断是否是数字或小数点
bool isDigit(char ch)
{
    if((ch >= '0' && ch<='9') || ch=='.')
        return true;
    else
        return false;
}

// 判断是否是字母
bool isLetter(char ch)
{
    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
        return true;
    else
        return false;
}

// 判断是字母还是数字
bool isLetterAndDigit(char ch)
{
    //ASCII || 48:0; 57:9; 65:A; 90:Z; 97:a, 122:z;
    if((ch>= '0' && ch<='9') || (ch>='A' && ch<='Z') || (ch>='a' && ch<='z') || ch=='_' )
        return true;
    else
        return false;
}

// 判断是否是关键字
bool isKey(char ch[])
{
    for(int i=0; i<32; i++)
    {
        if(strcmp(ch, KEY_WORD[i]) == 0)
        {
            cout<<"("<<strupr(ch)<<",)"<<"[关键字,"<<ch<<"]"<<endl;
            return true;
        }
    }
    return false;
}

// 判断是否是标识符
bool isIdentifier(char ch[])
{
    cout<<"(ID,"<<ch<<")"<<"[标识符]"<<endl;
    return true;
}

// 判断运算符
// 问题解决：break 后会跳出for循环，无法获取当前的返回值
// 分别判断双目运算符、单目运算符、界符，返回值分别是2，1，1，供chIndex进行偏移
int isOperator(char ch[])
{
    for(int i=0; i<12; i++)
    {
        if(strcmp(ch, OPT_DOUBLE[i]) == 0)
        {
            switch(i)
            {
            case 0 :
                cout<<"(LE，)"<<"[运算符,小于等于]"<<endl;
                break;
            case 1 :
                cout<<"(GE, )"<<"[运算符,大于等于]"<<endl;
                break;
            case 2 :
                cout<<"(IE, )"<<"[运算符,等于等于]"<<endl;
                break;
            case 3 :
                cout<<"(NE, )"<<"[运算符,不等于]"<<endl;
                break;
            case 4 :
                cout<<"(PE, )"<<"[运算符,加等于]"<<endl;
                break;
            case 5 :
                cout<<"(SE, )"<<"[运算符,减等于—]"<<endl;
                break;
            case 6 :
                cout<<"(ME, )"<<"[运算符,乘等于]"<<endl;
                break;
            case 7 :
                cout<<"(DE, )"<<"[运算符,除等于]"<<endl;
                break;
            case 8 :
                cout<<"(PO, )"<<"[运算符,加加]"<<endl;
                break;
            case 9 :
                cout<<"(SO, )"<<"[运算符,减减—]"<<endl;
                break;
            case 10 :
                cout<<"(OR, )"<<"[运算符,或]"<<endl;
                break;
            case 11 :
                cout<<"(AND, )"<<"[运算符,与]"<<endl;
                break;
            }
            return 2;
        }
    }

    for(int j=0; j<10; j++)
    {
        if(ch[0]==OPT_SINGLE[j])
        {
            switch(j)
            {
            case 0 :
                cout<<"(PLUS，)"<<"[运算符,加]"<<endl;
                break;
            case 1 :
                cout<<"(SUB, )"<<"[运算符,减]"<<endl;
                break;
            case 2 :
                cout<<"(MULTI, )"<<"[运算符,乘]"<<endl;
                break;
            case 3 :
                cout<<"(DIV, )"<<"[运算符,除]"<<endl;
                break;
            case 4 :
                cout<<"(EQ, )"<<"[运算符,等于]"<<endl;
                break;
            case 5 :
                cout<<"(LT, )"<<"[运算符,小于]"<<endl;
                break;
            case 6 :
                cout<<"(GT, )"<<"[运算符,大于]"<<endl;
                break;
            case 7 :
                cout<<"(CR, )"<<"[运算符,求余]"<<endl;
                break;
            case 8 :
                cout<<"(SHARP, )"<<"[运算符,#]"<<endl;
                break;
            case 9 :
                cout<<"(GA, )"<<"[运算符,取地址符]"<<endl;
                break;
            }
            return 1;
        }
    }

    for(int k = 0; k<12; k++)
    {
        if(ch[0]==END[k])
        {
            switch(k)
            {
            case 0 :
                cout<<"(LR_BRAC, )"<<"[运算符,左圆括号]"<<endl;
                break;
            case 1 :
                cout<<"(RR_BRAC, )"<<"[运算符,右圆括号]"<<endl;
                break;
            case 2 :
                cout<<"(SEMIC, )"<<"[运算符,分号]"<<endl;
                break;
            case 3 :
                cout<<"(COMMA, )"<<"[运算符,逗号]"<<endl;
                break;
            case 4 :
                cout<<"(DOT, )"<<"[运算符,点]"<<endl;
                break;
            case 5 :
                cout<<"(LS_BRAC, )"<<"[运算符,左方括号]"<<endl;
                break;
            case 6 :
                cout<<"(RS_BRAC, )"<<"[运算符,右方括号]"<<endl;
                break;
            case 7 :
                cout<<"(DQU_MARK, )"<<"[运算符,双引号]"<<endl;
                break;
            case 8 :
                cout<<"(LC_BRAC, )"<<"[运算符,左花括号]"<<endl;
                break;
            case 9 :
                cout<<"(RC_BRAC, )"<<"[运算符,右花括号]"<<endl;
                break;
            case 10 :
                cout<<"(COLON, )"<<"[运算符,冒号]"<<endl;
                break;
            case 11 :
                cout<<"(SQU_MARK, )"<<"[运算符,单引号]"<<endl;
                break;
            }
            return 1;
        }
    }

    // 以上为搜索到该字符，说明是非法字符
    cout<<"非法字符："<<ch<<endl;
    return 1;
}

// 预处理
bool pretreatment(char* fileIn, char* fileOut)
{
    ifstream ifile(fileIn);
    ofstream ofile(fileOut);

    int state = 0;// 状态
    string s = ""; // 拼接字符串的结果
    char c; // 从文件中依次读取各个字符的temp存储位置

    // 预处理采用逐个字符读取的方式，因为不需要超前搜索和回退
    // 由于在词法分析使用按行读取，因此不将换行符去掉
    while(ifile.get(c))
    {
        // 状态为初始值，直接输出
        if(state == 0)
        {
            if(isLetterAndDigit(c))// 如果是字母或者数字，直接写入文件
                ofile<<c;
            else if(!isStateMark(c))// 不是状态机内的符号，直接写入
                ofile<<c;
            else// 是状态机内的，进行状态转换
            {
                state = stateChange(state, c);//状态转换
                switch(state)
                {
                case 0:// 为0，直接输出
                    ofile<<c;
                    break;//!!!!!!!!!!!!!十分重要，因为少了break，在后续输出总是出现多余的空行，却查不出问题！
                case 1:
                case 5:// 输入了“或者/需要等待之后的输入，因此先存入string，将后续输入拼接，如果不是注释可以直接写入文件
                    s += c;
                    break;
                }
            }
        }
        else    // 判断优先级：字母数字======>状态机外字符（运算符和部分界符）======>状态机内字符（进行状态转换）
        {
            // 判断state=1的原因：如果是state=1，说明如果是注释的话，此时c应该是 /
            // 如果state=1并且c是字母或数字，可以确定这个“/”的含义是除号！此时应该向文件直接输出，并且将状态转换为0
            // 如果是在引号内的//，此时是在else内的，不在state=1状态
            if(isLetterAndDigit(c) && state != 1) // 如果是字母或者数字，直接拼接字符串
                s+=c;
            else if(isLetterAndDigit(c) && state == 1)
            {
                state = 0;
                s += c;
                ofile<<s;
                s = "";
            }
            // 如果不是状态机内的字符，可以确定是运算符或部分界符，可以拼接字符串
            else if(!isStateMark(c))
                s+=c;
            // 如果是状态机内的字符
            else
            {
                // 更换状态
                state = stateChange(state, c);
                switch(state)
                {
                case 0: // 状态转换中止，输出到文件
                    s += c;
                    ofile<<s;
                    s = "";
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    s += c; // 状态转换进行中，继续拼接字符串
                    break;
                case 6: // 状态表明是注释，需要直接删除
                    state = 0;// 注释结束，状态转换为0
                    ofile<<'\n';
                    s = "";
                    break;
                }
            }
        }
    }

    if(state == 3) // state为3，表示当前处于 /* 状态，而文件读取已经结束，因此注释未闭合
    {
        cout<<"多行注释未闭合，请检查错误！"<<endl;
        return false;
    }
    if(state == 5) // state为3，表示当前处于 " 状态，而文件读取已经结束，因此引号未闭合
    {
        cout<<"引号未闭合，请检查错误！"<<endl;
        return false;
    }

    ifile.close();
    ofile.close();
    return true;
}

// 判断控制器
void analyzeController(char ch[])
{
    int chLen = strlen(ch);
    int chIndex = 0;
    int quotationMark = 0;// 引号标记
    char strings[MAX] = "";
    int stringIndex = 0;

    while(chIndex < chLen)// 循环边界
    {
        // 是否是字符串内文字
        // 如果是第一个引号，那么会让引号标记变为单数
        // 直到第二个引号进入，才会输出字符串
        if(isQuotationMark(ch[chIndex]))
        {
            if(quotationMark)
            {
                cout<<"(STRING,"<<strings<<")[字符串常量,"<<strings<<"]"<<endl;
            }

            else
                cout<<"(DQU_MARK, )[运算符,双引号]"<<endl;

            quotationMark = (quotationMark+1)%2; // 模二求余判断引号单双数
        }

        // 如果引号为单数，将阻塞后续字符的判断，因为代表这是在一个字符串内， 不需要判断是否为标识符、关键字等
        // 直到第二个引号进入，才停止阻塞
        if(quotationMark)
        {
            strings[stringIndex] = ch[chIndex];
            strings[stringIndex+1] = '"';
            stringIndex++;
            chIndex++;
            continue;
        }

        // 去掉空格、Tab和回车
        if(isSpaseTab(ch[chIndex]))
        {
            chIndex++;
            continue;
        }

        // 由字母或下划线开头,关键字或标识符
        if(isLetter(ch[chIndex]))
        {
            char word[MAX]="";
            int wordlen = 0;
            while(isLetterAndDigit(ch[chIndex])) // 循环终止：非字母或者数字或者下划线
            {
                word[wordlen] = ch[chIndex];
                wordlen++;
                chIndex++;
            }

            if(isKey(word)) // 判断是否是关键字
                continue;
            if(isIdentifier(word)) // 判断是否是标识符
                continue;
        }

        // 由数字或小数点开头，常数
        else if(isDigit(ch[chIndex]))
        {
            char wordNum[MAX]="";
            int wordNumLen = 0;
            int ifFloat = 0;
            while(isDigit(ch[chIndex]))
            {
                if(ch[chIndex]=='.') // 判断是浮点数
                    ifFloat++;

                wordNum[wordNumLen]=ch[chIndex];
                wordNumLen++;
                chIndex++;
            }

            if(ifFloat == 1)
                cout<<"(FLOAT,"<<wordNum<<")[浮点数,"<<wordNum<<"]"<<endl;
            else if(ifFloat > 1)
                cout<<"浮点数格式错误！"<<endl;
            else
                cout<<"(INT,"<<wordNum<<")[整数,"<<wordNum<<"]"<<endl;
        }

        // 运算符
        else
        {
            // 超前搜索，判断双目运算符||单目运算符||界符
            // isOperator() 返回值为chIndex（数组下标）需要偏移的位数
            char word[3]="";
            word[0]=ch[chIndex];
            word[1]=ch[chIndex+1];
            chIndex += isOperator(word);
        }

    }
}

// 词法分析 文件操作
void analysis(char* file)
{
    ifstream ifile(file);
    string s;
    // 词法分析采用按行读取，因为可能要进行超前搜索和回退的操作
    // 使用字符串数组代替string可以使操作更加简便
    while(getline(ifile, s))
    {
        char ch[MAX];
        strcpy(ch, s.c_str());
        analyzeController(ch);
    }
    ifile.close();
}

int main()
{
    // 文件读写
    char* fileIn = "CFFXQ3.txt";
    char* fileOut = "CFFXQ2.txt";

    if(pretreatment(fileIn, fileOut))
        analysis(fileOut);
    else
        cout<<"词法分析预处理出现错误！"<<endl;

    return 0;
}
