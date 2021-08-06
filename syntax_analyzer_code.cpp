#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
static vector<string> v; // lexical analyzer에 의해 만들어진 token sequence들을 모두 저장(순서대로)
static vector<int> stack; //current state를 저장하는 스택 대신에 vector를 사용
static int index; // v에서 현재 검사할 부분을 가리키는 인덱스 

#define SHIFT       1  
#define REDUCE      2  
#define GOTO        3 
#define ACCEPT      4  

typedef struct _PRODUCTION //cfg의 production을 나타내는 구조체 ex) E -> T * T
{
    string from; // reducing 되는 string ex) E
    int toNum; // reducing하는 terminal 또는 non-terminal의 개수 ex) 3 (T, *, T)
}pTable;

typedef struct _slrTABLE //slr 테이블을 저장하기 위한 구조체
{
    int cState; //current State
    string token;//현재 token
    int action; // 다음 action(SHIFT/REDUCE/GOTO/ACCEPT)
    int nState; //next State
} slrTable;

vector<string> t;

pTable proTABLE[43] = // cfg의 production을 하나씩 뜯어내 나타낸 것으로 "CODE->VDECL CODE"인 경우 "{CODE, 2}"로 나타냄
{
    {"S'", 1},
    {"CODE", 2},
    {"CODE", 2},
    {"CODE", 2},
    {"CODE", 0},
    {"VDECL", 3},
    {"VDECL", 3},
    {"ASSIGN", 3},
    {"RHS", 1},
    {"RHS", 1},
    {"RHS", 1},
    {"RHS", 1},
    {"EXPR", 2},
    {"EXPR'", 2},
    {"EXPR'", 0},
    {"TEXPR", 2},
    {"TEXPR'", 2},
    {"TEXPR'", 0},
    {"FEXPR", 3},
    {"FEXPR", 1},
    {"FEXPR", 1},
    {"FDECL", 9},
    {"ARG", 3},
    {"ARG", 0},
    {"MOREARGS", 4},
    {"MOREARGS", 0},
    {"BLOCK", 2},
    {"BLOCK", 0},
    {"STMT", 1},
    {"STMT", 2},
    {"STMT", 8},
    {"STMT", 7},
    {"COND", 2},
    {"COND'", 2},
    {"COND'", 0},
    {"TCOND", 1},
    {"ELSE", 4},
    {"ELSE", 0},
    {"RETURN", 3},
    {"CDECL", 5},
    {"ODECL", 2},
    {"ODECL", 2},
    {"ODECL", 0},
};


slrTable lrTABLE[90][11] = //SLR 테이블의 내용을 저장하는 2차원 배열(90*11), 최대로 필요한 열의 개수는 10개이지만 error detect를 위해 열의 개수를 11로 지정함
{
    {{0, "vtype", SHIFT, 5}, {0, "class", SHIFT, 6},  {0, "$", REDUCE, 4}, {0, "CODE", GOTO, 1}, {0, "VDECL", GOTO, 2}, {0, "FDECL", GOTO, 3}, {0, "CDECL", GOTO, 4}},
    {{1, "$", ACCEPT, 0}},
    {{2, "vtype", SHIFT, 5}, {2, "class", SHIFT, 6}, {2, "$", REDUCE, 4}, {2, "CODE", GOTO, 7}, {2, "VDECL", GOTO, 2}, {2, "FDECL", GOTO, 3}, {2, "CDECL", GOTO, 4}},
    {{3, "vtype", SHIFT, 5}, {3, "class", SHIFT, 6}, {3, "$", REDUCE, 4}, {3, "CODE", GOTO, 8}, {3, "VDECL", GOTO, 2}, {3, "FDECL", GOTO, 3}, {3, "CDECL", GOTO, 4}},
    {{4, "vtype", SHIFT, 5}, {3, "class", SHIFT, 6}, {4, "$", REDUCE, 4}, {4, "CODE", GOTO, 9}, {4, "VDECL", GOTO, 2}, {4, "FDECL", GOTO, 3}, {4, "CDECL", GOTO, 4}},
    {{5, "id", SHIFT, 10}, {5, "ASSIGN", GOTO, 11}},
    {{6, "id", SHIFT, 12}},
    {{7, "$", REDUCE, 1}},
    {{8, "$", REDUCE, 2}},
    {{9, "$", REDUCE, 3}},
    {{10, "semi", SHIFT, 13}, {10, "assign", SHIFT, 15}, {10, "lparen", SHIFT, 14}},
    {{11, "semi", SHIFT, 16}},
    {{12, "lbrace", SHIFT, 17}},
    {{13, "vtype", REDUCE, 5}, {13, "id", REDUCE, 5}, {13, "rbrace", REDUCE, 5}, {13, "if", REDUCE, 5}, {13, "while", REDUCE, 5}, {13, "return", REDUCE, 5}, {13, "class", REDUCE, 5}, {13, "$", REDUCE, 5}},
    {{14, "vtype", SHIFT, 19}, {14, "rparen", REDUCE, 22}, {14, "ARG", GOTO, 18}},
    {{15, "id", SHIFT, 28}, {15, "literal", SHIFT, 22}, {15, "character", SHIFT, 23}, {15, "boolstr", SHIFT, 24}, {15, "lparen", SHIFT, 27}, {15, "num", SHIFT, 29}, {15, "RHS", GOTO, 20}, {15, "EXPR", GOTO, 21}, {15, "TEXPR", GOTO, 25}, {15, "FEXPR", GOTO, 26}},
    {{16, "vtype", REDUCE, 6}, {16, "id", REDUCE, 6}, {16, "rbrace", REDUCE, 6}, {16, "if", REDUCE, 6}, {16, "while", REDUCE, 6}, {16, "return", REDUCE, 6}, {16, "class", REDUCE, 6}, {16, "$", REDUCE, 6}},
    {{17, "vtype", SHIFT, 5}, {17, "rbrace", REDUCE, 42}, {17, "VDECL", GOTO, 31}, {17, "FDECL", GOTO, 32}, {17, "ODECL", GOTO, 30}},
    {{18, "rparen", SHIFT, 33}},
    {{19, "id", SHIFT, 34}},
    {{20, "semi", REDUCE, 7}},
    {{21, "semi", REDUCE, 8}},
    {{22, "semi", REDUCE, 9}},
    {{23, "semi", REDUCE, 10}},
    {{24, "semi", REDUCE, 11}},
    {{25, "semi", REDUCE, 14}, {25, "addsub", SHIFT, 36}, {25, "rparen", REDUCE, 14}, {25, "EXPR'", GOTO, 35}},
    {{26, "semi", REDUCE, 17}, {26, "addsub", REDUCE, 17}, {26, "multdiv", SHIFT, 38}, {26, "rparen", REDUCE, 17}, {26, "TEXPR'", GOTO, 37}},
    {{27, "id", SHIFT, 28}, {27, "lparen", SHIFT, 27}, {27, "num", SHIFT, 29}, {27, "RHS", GOTO, 39}, {27, "TEXPR", GOTO, 25}, {27, "FEXPR", GOTO, 26}},
    {{28, "semi", REDUCE, 19}, {28, "addsub", REDUCE, 19}, {28, "multdiv", REDUCE, 19}, {28, "rparen", REDUCE, 19}},
    {{29, "semi", REDUCE, 20}, {29, "addsub", REDUCE, 20}, {29, "multdiv", REDUCE, 20}, {29, "rparen", REDUCE, 20}},
    {{30, "rbrace", SHIFT, 40}},
    {{31, "vtype", SHIFT, 5}, {31, "rbrace", REDUCE, 42}, {31, "VDECL", GOTO, 31}, {31, "FDECL", GOTO, 32}, {31, "ODECL", GOTO, 41}},
    {{32, "vtype", SHIFT, 5}, {32, "rbrace", REDUCE, 42}, {32, "VDECL", GOTO, 31}, {32, "FDECL", GOTO, 32}, {32, "ODECL", GOTO, 42}},
    {{33, "lbrace", SHIFT, 43}},
    {{34, "rparen", REDUCE, 25}, {34, "comma", SHIFT, 45}, {34, "MOREARGS", GOTO, 44}},
    {{35, "semi", REDUCE, 12}, {35, "rparen", REDUCE, 12}},
    {{36, "id", SHIFT, 28}, {36, "lparen", SHIFT, 27}, {36, "num", SHIFT, 29}, {36, "EXPR", GOTO, 46}, {36, "TEXPR", GOTO, 25}, {36, "FEXPR", GOTO, 26}},
    {{37, "semi", REDUCE, 15}, {37, "addsub", REDUCE, 15}, {37, "rparen", REDUCE, 15}},
    {{38, "id", SHIFT, 28}, {38, "lparen", SHIFT, 27}, {38, "num", SHIFT, 29}, {38, "TEXPR", GOTO, 47}, {38, "FEXPR", GOTO, 26}},
    {{39, "rparen", SHIFT, 48}},
    {{40, "vtype", REDUCE, 39}, {40, "class", REDUCE, 39}, {40, "$", REDUCE, 39}},
    {{41, "rbrace", REDUCE, 40}},
    {{42, "rbrace", REDUCE, 41}},
    {{43, "vtype", SHIFT, 55}, {43, "id", SHIFT, 56}, {43, "rbrace", REDUCE, 27}, {43, "if", SHIFT, 53}, {43, "while", SHIFT, 54}, {43, "return", REDUCE, 27}, {43, "VDECL", GOTO, 51}, {43, "ASSIGN", GOTO, 52}, {43, "BLOCK", GOTO, 49}, {43, "STMT", GOTO, 50}},
    {{44, "rparen", REDUCE, 22}},
    {{45, "vtype", SHIFT, 57}},
    {{46, "semi", REDUCE, 13}, {46, "rparen", REDUCE, 13}},
    {{47, "semi", REDUCE, 16}, {47, "addsub", REDUCE, 16}, {47, "rparen", REDUCE, 16}},
    {{48, "semi", REDUCE, 18}, {48, "addsub", REDUCE, 18}, {48, "multdiv", REDUCE, 18}, {48, "rparen", REDUCE, 18}},
    {{49, "return", SHIFT, 59}, {49, "RETURN", GOTO, 58}},
    {{50, "vtype", SHIFT, 55}, {50, "id", SHIFT, 56}, {50, "rbrace", REDUCE, 27}, {50, "if", SHIFT, 53}, {50, "while", SHIFT, 54}, {50, "return", REDUCE, 27}, {50, "VDECL", GOTO, 51}, {50, "ASSIGN", GOTO, 52}, {50, "BLOCK", GOTO, 60}, {50, "STMT", GOTO, 50}},
    {{51, "vtype", REDUCE, 28}, {51, "id", REDUCE, 28}, {51, "rbrace", REDUCE, 28}, {51, "if", REDUCE, 28}, {51, "while", REDUCE, 28}, {51, "return", REDUCE, 28}},
    {{52, "semi", SHIFT, 61}},
    {{53, "lparen", SHIFT, 62}},
    {{54, "lparen", SHIFT, 63}},
    {{55, "id", SHIFT, 64}, {55, "ASSIGN", GOTO, 11}},
    {{56, "assign", SHIFT, 15}},
    {{57, "id", SHIFT, 65}},
    {{58, "rbrace", SHIFT, 66}},
    {{59, "id", SHIFT, 28}, {59, "literal", SHIFT, 22}, {59, "character", SHIFT, 23}, {59, "boolstr", SHIFT, 24}, {59, "lparen", SHIFT, 27}, {59, "num", SHIFT, 29}, {59, "RHS", GOTO, 67}, {59, "EXPR", GOTO, 21}, {59, "TEXPR", GOTO, 25}, {59, "FEXPR", GOTO, 26}},
    {{60, "rbrace", REDUCE, 26}, {60, "return", REDUCE, 26}},
    {{61, "vtype", REDUCE, 29}, {61, "id", REDUCE, 29}, {61, "rbrace", REDUCE, 29}, {61, "if", REDUCE, 29}, {61, "while", REDUCE, 29}, {61, "return", REDUCE, 29}},
    {{62, "boolstr", SHIFT, 70}, {62, "COND", GOTO, 68}, {62, "TCOND", GOTO, 69}},
    {{63, "boolstr", SHIFT, 70}, {63, "COND", GOTO, 71}, {63, "TCOND", GOTO, 69}},
    {{64, "semi", SHIFT, 13}, {64, "assign", SHIFT, 15}},
    {{65, "rparen", REDUCE, 25}, {65, "comma", SHIFT, 45}, {65, "MOREARGS", GOTO, 72}},
    {{66, "vtype", REDUCE, 21}, {66, "rbrace", REDUCE, 21}, {66, "class", REDUCE, 21}, {66, "$", REDUCE, 21}},
    {{67, "semi", SHIFT, 73}},
    {{68, "rparen", SHIFT, 74}},
    {{69, "rparen", REDUCE, 34}, {69, "comp", SHIFT, 76}, {69, "COND'", GOTO, 75}},
    {{70, "rparen", REDUCE, 35}, {70, "comp", REDUCE, 35}},
    {{71, "rparen", SHIFT, 77}},
    {{72, "rparen", REDUCE, 24}},
    {{73, "rbrace", REDUCE, 38}},
    {{74, "lbrace", SHIFT, 78}},
    {{75, "rparen", REDUCE, 32}},
    {{76, "boolstr", SHIFT, 70}, {76, "COND", GOTO, 79}, {76, "TCOND", GOTO, 69}},
    {{77, "lbrace", SHIFT, 80}},
    {{78, "vtype", SHIFT, 55}, {78, "id", SHIFT, 56}, {78, "rbrace", REDUCE, 27}, {78, "if", SHIFT, 53}, {78, "while", SHIFT, 54}, {78, "return", REDUCE, 27}, {78, "VDECL", GOTO, 51}, {78, "ASSIGN", GOTO, 52}, {78, "BLOCK", GOTO, 81}, {78, "STMT", GOTO, 50}},
    {{79, "rparen", REDUCE, 33}},
    {{80, "vtype", SHIFT, 55}, {80, "id", SHIFT, 56}, {80, "rbrace", REDUCE, 27}, {80, "if", SHIFT, 53}, {80, "while", SHIFT, 54}, {80, "return", REDUCE, 27}, {80, "VDECL", GOTO, 51}, {80, "ASSIGN", GOTO, 52}, {80, "BLOCK", GOTO, 82}, {80, "STMT", GOTO, 50}},
    {{81, "rbrace", SHIFT, 83}},
    {{82, "rbrace", SHIFT, 84}},
    {{83, "vtype", REDUCE, 37}, {83, "id", REDUCE, 37}, {83, "rbrace", REDUCE, 37}, {83, "if", REDUCE, 37}, {83, "while", REDUCE, 37}, {83, "else", SHIFT, 86}, {83, "return", REDUCE, 37}, {83, "ELSE", GOTO, 85}},
    {{84, "vtype", REDUCE, 31}, {84, "id", REDUCE, 31}, {84, "rbrace", REDUCE, 31}, {84, "if", REDUCE, 31}, {84, "while", REDUCE, 31}, {84, "return", REDUCE, 31}},
    {{85, "vtype", REDUCE, 30}, {85, "id", REDUCE, 30}, {85, "rbrace", REDUCE, 30}, {85, "if", REDUCE, 30}, {85, "while", REDUCE, 30}, {85, "return", REDUCE, 30}},
    {{86, "lbrace", SHIFT, 87}},
    {{87, "vtype", SHIFT, 55}, {87, "id", SHIFT, 56}, {87, "rbrace", REDUCE, 27}, {87, "if", SHIFT, 53}, {87, "while", SHIFT, 54}, {87, "return", REDUCE, 27}, {87, "VDECL", GOTO, 51}, {87, "ASSIGN", GOTO, 52}, {87, "BLOCK", GOTO, 88}, {87, "STMT", GOTO, 50}},
    {{88, "rbrace", SHIFT, 89}},
    {{89, "vtype", REDUCE, 36}, {89, "id", REDUCE, 36}, {89, "rbrace", REDUCE, 36}, {89, "if", REDUCE, 36}, {89, "while", REDUCE, 36}, {89, "return", REDUCE, 36}}
};

void shift(int n) // "shift n"을 만났을 때 실행되는 함수
{
    index++; // index 하나 올리고,
    stack.push_back(n); // 스택(vector)에 n을 저장
}

void reduce(int n) //"reduce (n)"을 만났을 때 실행되는 함수
{
    vector<string>::iterator iter = v.begin(); //erase()와 insert()를 사용하기 위한 iterator 초기화
    iter += index; // iter를 현재 index로 만듦
    int popNum = proTABLE[n].toNum; // proTABLE[n].toNum은 production의 rhs의 개수를 나타내므로 이것은 스택에서 pop 해야 할 원소의 개수와 같음 
    string reduceStr = proTABLE[n].from; // proTABLE[n].from은 production의 lhs

    if (popNum != 0) // production의 rhs의 개수가 0이면 erase 할 것이 없음
    {
        v.erase(iter - popNum, iter);
    }
    index -=  popNum; // erase된 개수만큼 index 감산
    iter = v.begin(); // iter는 다시 초기화
    iter += index;
    v.insert(iter, reduceStr); // index가 가리키는 부분에 production의 lhs insert
    //index = index - popNum + 1;
    while (popNum--) //production의 rhs의 개수만큼 스택에서 pop
    {
        stack.pop_back();
    }
 }

void goTo(int n) //"GOTO n"을 만났을 때 실행되는 함수
{
    index++; // index 하나 올리고,
    stack.push_back(n); // 스택(vector)에 n을 저장
}


int main(int argc, char* argv[])
{
    ifstream inFile;
    string str;
    string inputFileName = argv[1];
    string nToken;

    inFile.open(inputFileName); // lexical_analyzer의 결과 파일을 받음

    if (inFile.is_open()) //파일이 정상적으로 열리면 실행
    {
        while (!inFile.eof())
        {
            getline(inFile, str); //input파일에서 한 줄을 읽어와서 str에 저장
            //if (str.empty() != true && !inFile.eof()) str.pop_back(); //리눅스에서는 개행할 때 "Wr"이 추가되므로 삭제를 위한 코드(빈 줄이 아니고, EOF가 아닌 줄에서만 적용)

            stringstream ss(str);
            string token;

            getline(ss, token, ',');
            
            //lexical analyzer에서 만들어진 token name이 모두 대문자이고, cfg의 lhs, rhs로 사용되는 terminal, nonterminal의 형식이 조금씩 다르므로 적당히 바꿔주는 과정
            // ex) lexical_analyzer: (ARITHOPERATOR,+) -> syntax_analyzer: (addsub) 
                if (token == "INT" || token == "CHAR" || token == "BOOLEAN" || token == "STRING")
                {
                    v.push_back("vtype");
                }
                else if (token == "COMPARISON")
                {
                    v.push_back("comp");
                }
                else if (token == "BOOLSTRING")
                {
                    v.push_back("boolstr");
                }
                else if (token == "INTEGER")
                {
                    v.push_back("num");
                }
                else if (token == "ARITHOPERATOR")
                {
                    getline(ss, token, ',');
                    if(token == "+" || token == "-")
                        v.push_back("addsub");
                    else
                        v.push_back("multidiv");
                }
                else if (token == "CONDITION")
                {
                    getline(ss, token, ',');
                    if (token == "if")
                        v.push_back("if");
                    else
                        v.push_back("else");
                }
                else
                {
                    string temp;
                    for (string::size_type i = 0; i < token.length(); ++i)
                    {
                        temp += (tolower(token[i]));
                    }
                    v.push_back(temp);
                }
          
        }
        while (v.at(v.size() - 1) == "") { v.pop_back(); } // 마지막줄에 빈 줄이 들어가는 경우 삭제
        v.push_back("$"); // v 마지막에 $ 삽입
    }
    else // 파일이 정상적으로 열리지 않은 경우
    {
        cout << "File cannot open!" << endl;
        return 0;
    }

    index = 0; // index 초기화
    stack.push_back(0); // 스택에 초기값으로 0 push
    
        while (index < v.size()) // index가 v의 원소 개수를 넘어가기 전까지 반복
        {
            nToken = v.at(index); //검사할 token

            int row = stack.at(stack.size() - 1); // state는 stack의 가장 맨 위에 저장되는 원소
            int i = 0;
            while ((nToken != lrTABLE[row][i].token) && i < 11) // slrTABLE에서 token<->state가 서로 부합하는 열 번호를 찾음
            {
                i++;
            }
            if (i == 11) i--; //i가 11까지 갔다는 것은 slrTABLE에 존재하지 않는 부분임을 뜻함
            if (lrTABLE[row][i].action == 1) //SHIFT
            {
                shift(lrTABLE[row][i].nState);
            }
            else if (lrTABLE[row][i].action == 2) //REDUCE
            {
                reduce(lrTABLE[row][i].nState);
            }
            else if (lrTABLE[row][i].action == 3) //GOTO
            {
                goTo(lrTABLE[row][i].nState);
            }
            else if (lrTABLE[row][i].action == 4) //ACCEPT
            {
                cout << "Accepted!"<<endl;
                break;
            }
            else //중간에 error가 발생할 경우(accept가 되지 않을 경우) error report 출력
            {
                cout << "Error!" << endl;
                cout << "[ERROR REPORT]" << endl;
                cout << "current state in stack: " << lrTABLE[row][0].cState << endl; //스택의 맨 위에 저장되어있는 state number
                cout << "error part of token sequences: " << index+1 << endl; // 도중에 에러가 발생한 부분의 index number(첫번째 원소가 1(zero index 아님))
                cout << "final state of Reducing: "; // 에러가 발생하기 직전까지 reducing된 결과
                for (vector<int>::size_type i = 0; i < v.size(); i++) {
                    cout << v[i] << " ";
                }
                cout << endl;
                break;
            }
            
        }

    
    inFile.close(); // 파일 닫기
    return 0;

}