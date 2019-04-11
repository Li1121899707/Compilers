#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#define MAX 256  //�����볤��
using namespace std;

const char* const KEY_WORD[32]= {"char", "double", "enum", "float", "int", "long", "short", "signed", "struct", "unoin", "unsigned",
                                "void", "for", "do", "while", "break", "continue", "if", "else", "goto", "switch", "case", "default",
                                "return", "auto", "extern", "register", "static", "const", "sizeof", "typedef", "volatile"
                               };//�ؼ���
const char OPT_SINGLE[] = {'+', '-', '*', '/', '=', '<', '>', '%', '#', '&'};//��Ŀ�����
const char* OPT_DOUBLE[] = {"<=", ">=", "==", "!=", "+=", "-=", "*=", "/=", "++", "--", "||", "&&"};//˫Ŀ�����
const char END[] = {'(', ')', ';', ',', '.', '[', ']', '"', '{', '}', ':', '\''};//���

// ״̬��ת��
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

// �ж��Ƿ���״̬���ڵ��ַ�
bool isStateMark(char delimiter)
{
    if('/' == delimiter || '*' == delimiter || '"' == delimiter || '\n' == delimiter)
        return true;
    else
        return false;
}

// �ж��Ƿ�������
bool isQuotationMark(char ch)
{
    if(ch=='"')
        return true;
    else
        return false;
}

// �ж��Ƿ��ǿո�Tab�ͻس�
bool isSpaseTab(char ch)
{
    if(ch==' ' || ch=='\t' || ch=='\n')
        return true;
    else
        return false;
}

// �ж��Ƿ������ֻ�С����
bool isDigit(char ch)
{
    if((ch >= '0' && ch<='9') || ch=='.')
        return true;
    else
        return false;
}

// �ж��Ƿ�����ĸ
bool isLetter(char ch)
{
    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
        return true;
    else
        return false;
}

// �ж�����ĸ��������
bool isLetterAndDigit(char ch)
{
    //ASCII || 48:0; 57:9; 65:A; 90:Z; 97:a, 122:z;
    if((ch>= '0' && ch<='9') || (ch>='A' && ch<='Z') || (ch>='a' && ch<='z') || ch=='_' )
        return true;
    else
        return false;
}

// �ж��Ƿ��ǹؼ���
bool isKey(char ch[])
{
    for(int i=0; i<32; i++)
    {
        if(strcmp(ch, KEY_WORD[i]) == 0)
        {
            cout<<"("<<strupr(ch)<<",)"<<"[�ؼ���,"<<ch<<"]"<<endl;
            return true;
        }
    }
    return false;
}

// �ж��Ƿ��Ǳ�ʶ��
bool isIdentifier(char ch[])
{
    cout<<"(ID,"<<ch<<")"<<"[��ʶ��]"<<endl;
    return true;
}

// �ж������
// ��������break �������forѭ�����޷���ȡ��ǰ�ķ���ֵ
// �ֱ��ж�˫Ŀ���������Ŀ����������������ֵ�ֱ���2��1��1����chIndex����ƫ��
int isOperator(char ch[])
{
    for(int i=0; i<12; i++)
    {
        if(strcmp(ch, OPT_DOUBLE[i]) == 0)
        {
            switch(i)
            {
            case 0 :
                cout<<"(LE��)"<<"[�����,С�ڵ���]"<<endl;
                break;
            case 1 :
                cout<<"(GE, )"<<"[�����,���ڵ���]"<<endl;
                break;
            case 2 :
                cout<<"(IE, )"<<"[�����,���ڵ���]"<<endl;
                break;
            case 3 :
                cout<<"(NE, )"<<"[�����,������]"<<endl;
                break;
            case 4 :
                cout<<"(PE, )"<<"[�����,�ӵ���]"<<endl;
                break;
            case 5 :
                cout<<"(SE, )"<<"[�����,�����ڡ�]"<<endl;
                break;
            case 6 :
                cout<<"(ME, )"<<"[�����,�˵���]"<<endl;
                break;
            case 7 :
                cout<<"(DE, )"<<"[�����,������]"<<endl;
                break;
            case 8 :
                cout<<"(PO, )"<<"[�����,�Ӽ�]"<<endl;
                break;
            case 9 :
                cout<<"(SO, )"<<"[�����,������]"<<endl;
                break;
            case 10 :
                cout<<"(OR, )"<<"[�����,��]"<<endl;
                break;
            case 11 :
                cout<<"(AND, )"<<"[�����,��]"<<endl;
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
                cout<<"(PLUS��)"<<"[�����,��]"<<endl;
                break;
            case 1 :
                cout<<"(SUB, )"<<"[�����,��]"<<endl;
                break;
            case 2 :
                cout<<"(MULTI, )"<<"[�����,��]"<<endl;
                break;
            case 3 :
                cout<<"(DIV, )"<<"[�����,��]"<<endl;
                break;
            case 4 :
                cout<<"(EQ, )"<<"[�����,����]"<<endl;
                break;
            case 5 :
                cout<<"(LT, )"<<"[�����,С��]"<<endl;
                break;
            case 6 :
                cout<<"(GT, )"<<"[�����,����]"<<endl;
                break;
            case 7 :
                cout<<"(CR, )"<<"[�����,����]"<<endl;
                break;
            case 8 :
                cout<<"(SHARP, )"<<"[�����,#]"<<endl;
                break;
            case 9 :
                cout<<"(GA, )"<<"[�����,ȡ��ַ��]"<<endl;
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
                cout<<"(LR_BRAC, )"<<"[�����,��Բ����]"<<endl;
                break;
            case 1 :
                cout<<"(RR_BRAC, )"<<"[�����,��Բ����]"<<endl;
                break;
            case 2 :
                cout<<"(SEMIC, )"<<"[�����,�ֺ�]"<<endl;
                break;
            case 3 :
                cout<<"(COMMA, )"<<"[�����,����]"<<endl;
                break;
            case 4 :
                cout<<"(DOT, )"<<"[�����,��]"<<endl;
                break;
            case 5 :
                cout<<"(LS_BRAC, )"<<"[�����,������]"<<endl;
                break;
            case 6 :
                cout<<"(RS_BRAC, )"<<"[�����,�ҷ�����]"<<endl;
                break;
            case 7 :
                cout<<"(DQU_MARK, )"<<"[�����,˫����]"<<endl;
                break;
            case 8 :
                cout<<"(LC_BRAC, )"<<"[�����,������]"<<endl;
                break;
            case 9 :
                cout<<"(RC_BRAC, )"<<"[�����,�һ�����]"<<endl;
                break;
            case 10 :
                cout<<"(COLON, )"<<"[�����,ð��]"<<endl;
                break;
            case 11 :
                cout<<"(SQU_MARK, )"<<"[�����,������]"<<endl;
                break;
            }
            return 1;
        }
    }

    // ����Ϊ���������ַ���˵���ǷǷ��ַ�
    cout<<"�Ƿ��ַ���"<<ch<<endl;
    return 1;
}

// Ԥ����
bool pretreatment(char* fileIn, char* fileOut)
{
    ifstream ifile(fileIn);
    ofstream ofile(fileOut);

    int state = 0;// ״̬
    string s = ""; // ƴ���ַ����Ľ��
    char c; // ���ļ������ζ�ȡ�����ַ���temp�洢λ��

    // Ԥ�����������ַ���ȡ�ķ�ʽ����Ϊ����Ҫ��ǰ�����ͻ���
    // �����ڴʷ�����ʹ�ð��ж�ȡ����˲������з�ȥ��
    while(ifile.get(c))
    {
        // ״̬Ϊ��ʼֵ��ֱ�����
        if(state == 0)
        {
            if(isLetterAndDigit(c))// �������ĸ�������֣�ֱ��д���ļ�
                ofile<<c;
            else if(!isStateMark(c))// ����״̬���ڵķ��ţ�ֱ��д��
                ofile<<c;
            else// ��״̬���ڵģ�����״̬ת��
            {
                state = stateChange(state, c);//״̬ת��
                switch(state)
                {
                case 0:// Ϊ0��ֱ�����
                    ofile<<c;
                    break;//!!!!!!!!!!!!!ʮ����Ҫ����Ϊ����break���ں���������ǳ��ֶ���Ŀ��У�ȴ�鲻�����⣡
                case 1:
                case 5:// �����ˡ�����/��Ҫ�ȴ�֮������룬����ȴ���string������������ƴ�ӣ��������ע�Ϳ���ֱ��д���ļ�
                    s += c;
                    break;
                }
            }
        }
        else    // �ж����ȼ�����ĸ����======>״̬�����ַ���������Ͳ��ֽ����======>״̬�����ַ�������״̬ת����
        {
            // �ж�state=1��ԭ�������state=1��˵�������ע�͵Ļ�����ʱcӦ���� /
            // ���state=1����c����ĸ�����֣�����ȷ�������/���ĺ����ǳ��ţ���ʱӦ�����ļ�ֱ����������ҽ�״̬ת��Ϊ0
            // ������������ڵ�//����ʱ����else�ڵģ�����state=1״̬
            if(isLetterAndDigit(c) && state != 1) // �������ĸ�������֣�ֱ��ƴ���ַ���
                s+=c;
            else if(isLetterAndDigit(c) && state == 1)
            {
                state = 0;
                s += c;
                ofile<<s;
                s = "";
            }
            // �������״̬���ڵ��ַ�������ȷ����������򲿷ֽ��������ƴ���ַ���
            else if(!isStateMark(c))
                s+=c;
            // �����״̬���ڵ��ַ�
            else
            {
                // ����״̬
                state = stateChange(state, c);
                switch(state)
                {
                case 0: // ״̬ת����ֹ��������ļ�
                    s += c;
                    ofile<<s;
                    s = "";
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    s += c; // ״̬ת�������У�����ƴ���ַ���
                    break;
                case 6: // ״̬������ע�ͣ���Ҫֱ��ɾ��
                    state = 0;// ע�ͽ�����״̬ת��Ϊ0
                    ofile<<'\n';
                    s = "";
                    break;
                }
            }
        }
    }

    if(state == 3) // stateΪ3����ʾ��ǰ���� /* ״̬�����ļ���ȡ�Ѿ����������ע��δ�պ�
    {
        cout<<"����ע��δ�պϣ��������"<<endl;
        return false;
    }
    if(state == 5) // stateΪ3����ʾ��ǰ���� " ״̬�����ļ���ȡ�Ѿ��������������δ�պ�
    {
        cout<<"����δ�պϣ��������"<<endl;
        return false;
    }

    ifile.close();
    ofile.close();
    return true;
}

// �жϿ�����
void analyzeController(char ch[])
{
    int chLen = strlen(ch);
    int chIndex = 0;
    int quotationMark = 0;// ���ű��
    char strings[MAX] = "";
    int stringIndex = 0;

    while(chIndex < chLen)// ѭ���߽�
    {
        // �Ƿ����ַ���������
        // ����ǵ�һ�����ţ���ô�������ű�Ǳ�Ϊ����
        // ֱ���ڶ������Ž��룬�Ż�����ַ���
        if(isQuotationMark(ch[chIndex]))
        {
            if(quotationMark)
            {
                cout<<"(STRING,"<<strings<<")[�ַ�������,"<<strings<<"]"<<endl;
            }

            else
                cout<<"(DQU_MARK, )[�����,˫����]"<<endl;

            quotationMark = (quotationMark+1)%2; // ģ�������ж����ŵ�˫��
        }

        // �������Ϊ�����������������ַ����жϣ���Ϊ����������һ���ַ����ڣ� ����Ҫ�ж��Ƿ�Ϊ��ʶ�����ؼ��ֵ�
        // ֱ���ڶ������Ž��룬��ֹͣ����
        if(quotationMark)
        {
            strings[stringIndex] = ch[chIndex];
            strings[stringIndex+1] = '"';
            stringIndex++;
            chIndex++;
            continue;
        }

        // ȥ���ո�Tab�ͻس�
        if(isSpaseTab(ch[chIndex]))
        {
            chIndex++;
            continue;
        }

        // ����ĸ���»��߿�ͷ,�ؼ��ֻ��ʶ��
        if(isLetter(ch[chIndex]))
        {
            char word[MAX]="";
            int wordlen = 0;
            while(isLetterAndDigit(ch[chIndex])) // ѭ����ֹ������ĸ�������ֻ����»���
            {
                word[wordlen] = ch[chIndex];
                wordlen++;
                chIndex++;
            }

            if(isKey(word)) // �ж��Ƿ��ǹؼ���
                continue;
            if(isIdentifier(word)) // �ж��Ƿ��Ǳ�ʶ��
                continue;
        }

        // �����ֻ�С���㿪ͷ������
        else if(isDigit(ch[chIndex]))
        {
            char wordNum[MAX]="";
            int wordNumLen = 0;
            int ifFloat = 0;
            while(isDigit(ch[chIndex]))
            {
                if(ch[chIndex]=='.') // �ж��Ǹ�����
                    ifFloat++;

                wordNum[wordNumLen]=ch[chIndex];
                wordNumLen++;
                chIndex++;
            }

            if(ifFloat == 1)
                cout<<"(FLOAT,"<<wordNum<<")[������,"<<wordNum<<"]"<<endl;
            else if(ifFloat > 1)
                cout<<"��������ʽ����"<<endl;
            else
                cout<<"(INT,"<<wordNum<<")[����,"<<wordNum<<"]"<<endl;
        }

        // �����
        else
        {
            // ��ǰ�������ж�˫Ŀ�����||��Ŀ�����||���
            // isOperator() ����ֵΪchIndex�������±꣩��Ҫƫ�Ƶ�λ��
            char word[3]="";
            word[0]=ch[chIndex];
            word[1]=ch[chIndex+1];
            chIndex += isOperator(word);
        }

    }
}

// �ʷ����� �ļ�����
void analysis(char* file)
{
    ifstream ifile(file);
    string s;
    // �ʷ��������ð��ж�ȡ����Ϊ����Ҫ���г�ǰ�����ͻ��˵Ĳ���
    // ʹ���ַ����������string����ʹ�������Ӽ��
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
    // �ļ���д
    char* fileIn = "CFFXQ3.txt";
    char* fileOut = "CFFXQ2.txt";

    if(pretreatment(fileIn, fileOut))
        analysis(fileOut);
    else
        cout<<"�ʷ�����Ԥ������ִ���"<<endl;

    return 0;
}
