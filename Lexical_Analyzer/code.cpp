#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;
vector <string> t_n, t_v; //t_n���� token name��, t_v���� token value�� ����
string temp; //token value�� �ӽ÷� �����ϱ� ���� string
static int index;
int stringNum = 0;

void isID(string st, int& num)
{
    string state = "T0";
    if (st[num] == '_')
    {
        state = "T1";
        temp = st[num];
        num++;
    }
    else if (isalpha(st[num]) == 1 || isalpha(st[num]) == 2) //letter�� ���
    {
        state = "T2";
        temp = st[num];
        num++;
    }
    else
    {
        state = "false";
    }
    while (state != "false")
    {
        if (st[num] == '_')
        {
            state = "T3";
            temp += st[num];
            num++;
        }
        else if (isalpha(st[num]) == 1 || isalpha(st[num]) == 2) //digit�� ���
        {
            state = "T4";
            temp += st[num];
            num++;
        }
        else if (isdigit(st[num]) != 0)
        {
            state = "T5";
            temp += st[num];
            num++;
        }
        else
        {
            state = "false";
            t_n.push_back("ID");
            t_v.push_back(temp);
            temp.clear();
        }
    }
    

}

void isCHARACTER(string st, int &num)
{
    string state = "T0";
    if (st[num] == '\'')
    {
        state = "T1";
        num++;
    }
    else state = "false";

    if (state == "T1")
    {
        if (isalpha(st[num]) == 1 || isalpha(st[num]) == 2)
        {
            state = "T2";
            temp += st[num];
            num++;
            
        }

        else if (isdigit(st[num]) != 0)
        {
            state = "T3";
            temp += st[num];
            num++;
            
        }

        else if (st[num] == ' ')
        {
            state = "T4";
            num++;
        }

        else {
            state = "false";

        }
    }

    if (state!="false" && st[num] == '\'')
    {
        t_n.push_back("CHARACTER");
        t_v.push_back(temp);
        temp.clear();
    }
    

   
}

void isLITERAL(string st, int &num)
{
    string state = "T0";
    if (st[num] == '\"')
    {
        state = "T1";
        num++;
    }
    else state = "false";

    while (state =="T1" || state == "T7" && st[num] != '\"')
    {
        temp += st[num];
        num++;
        state = "T7";
    }
    if (state != "false" && st[num] == '\"')
    {
        t_n.push_back("LITERAL");
        t_v.push_back(temp);
        num++;
        temp.clear();
    }
}

void isCOMPARISON(string st, int& num)
{
    string state = "T0";
    if (st[num] == '<')
    {
        state = "T1";
        temp += st[num];
    }
    else if (st[num] == '>')
    {
        state = "T2";
        temp += st[num];
        
    }
    else if (st[num] == '!')
    {
        state = "T3";
        temp += st[num];
       
    }
    else if (st[num] == '=')
    {
        state = "T4";
        temp += st[num];
    }
    else state = "false";

    if ((state == "T1" || state == "T2" || state == "T3" || state == "T4") && st[num+1] == '=')
    {
        state = "T5";
        temp += st[num];
        num++;
    }
    
    if (state == "T1" || state == "T2" || state == "T5")
    {
        t_n.push_back("COMPARISON");
        t_v.push_back(temp);
        temp.clear();
        num++;
    }
    else if (state == "T4")
    {
        temp.clear();
    }
}

void isINT(string st, int &num)
{
    string state = "T0";
    
    if (st[num] == 'i' && st[num + 1] == 'n' && st[num + 2] == 't')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("INT");
        t_v.push_back(" ");
        num += 3;
    }
}

void isCHAR(string st, int &num)
{
    string state = "T0";
    if (st[num + 0] == 'c' && st[num + 1] == 'h' && st[num + 2] == 'a' && st[num + 3] == 'r')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("CHAR");
        t_v.push_back(" ");
        num += 4;
    }
}

void isBOOLEAN(string st, int &num)
{
    string state = "T0";
    if (st[num] == 'b' && st[num + 1] == 'o' && st[num + 2] == 'o' && st[num + 3] == 'l' && st[num + 4]=='e' && st[num + 5] == 'a' && st[num + 6] == 'n')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("BOOLEAN");
        t_v.push_back(" ");
        num += 7;
    }
}

void isSTRING(string st, int &num)
{
    string state = "T0";
    if (st[num] == 's' && st[num + 1] == 't' && st[num + 2] == 'r' && st[num + 3] == 'i' && st[num +4]=='n' && st[num + 5]=='g')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("STRING");
        t_v.push_back(" ");
        num += 6;
    }
}

void isBOOLSTRING(string st, int &num)
{
    string state = "T0";
    if (st[num] == 't' && st[num + 1] == 'r' && st[num + 2] == 'u' && st[num + 3] == 'e')
    {
        state = "T9";
        temp = "true";
        num += 4;
    }
    if (st[num] == 'f' && st[num + 1] == 'a' && st[num + 2] == 'l' && st[num + 3] == 's' && st[num + 4] == 'e')
    {
        state = "T8";
        temp = "false";
        num += 5;
    }

    if (state == "T8" || state == "T9")
    {
        t_n.push_back("BOOLSTRING");
        t_v.push_back(temp);
        temp.clear();
    }
    
}

void isLPAREN(string st, int &num)
{
    string state = "T0";
    if (st[num] == '(')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("LPAREN");
        t_v.push_back(" ");
        num++;
    }
}

void isRPAREN(string st, int &num)
{
    string state = "T0";
    if (st[num] == ')')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RPAREN");
        t_v.push_back(" ");
        num++;
    }
}

void isLBRACE(string st, int &num)
{
    string state = "T0";
    if (st[num] == '{')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("LBRACE");
        t_v.push_back(" ");
        num++;
    }
}

void isRBRACE(string st, int &num)
{
    string state = "T0";
    if (st[num] == '}')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RBRACE");
        t_v.push_back(" ");
        num++;
    }
}

void isLBRACKET(string st, int &num)
{
    string state = "T0";
    if (st[num] == '[')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("LBRACKET");
        t_v.push_back(" ");
        num++;
    }
}

void isRBRACKET(string st, int &num)
{
    string state = "T0";
    if (st[num] == ']')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RBRACKET");
        t_v.push_back(" ");
        num++;
    }
}

void isSEMI(string st, int &num)
{
    string state = "T0";
    if (st[num] == ';')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("SEMI");
        t_v.push_back(" ");
        num++;
    }
}

void isCOMMA(string st, int &num)
{
    string state = "T0";
    if (st[num] == ',')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("COMMA");
        t_v.push_back(" ");
        num++;
    }
}

void isASSIGN(string st, int &num)
{
    string state = "T0";
    
    if (st[num] == '=')
    {
        state = "T1";
    }
    else state = "false";

    if (st[num + 1] == '=') // ==�� =�� ����
    {
        state = "false";
    }
    

    if (state == "T1")
    {
        t_n.push_back("ASSIGN");
        t_v.push_back(" ");
        num++;
        temp.clear();
    }
}

void isARITHOPERATOR(string st, int &num)
{
    string state = "T0";
    if (st[num] == '+')
    {
        state = "T1";
        temp += "+";
    }
    else if (st[num] == '-')
    {
        state = "T2";
        temp += "-";
    }
    else if (st[num] == '*')
    {
        state = "T3";
        temp += "*";
    }
    else if (st[num] == '/')
    {
        state = "T4";
        temp += "/";
    }
    else state = "false";

    if (state == "T1" || state == "T2" || state == "T3" || state == "T4")
    {
        temp = st[num];
        t_n.push_back("ARITHOPERATOR");
        t_v.push_back(temp);
        temp.clear();
        num++;
    }
}

void isCLASS(string st, int &num)
{
    string state = "T0";
    if (st[num] == 'c' && st[num + 1] == 'l' && st[num + 2] == 'a' && st[num + 3] == 's' && st[num + 4] == 's')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("CLASS");
        t_v.push_back(" ");
        num += 5;
    }
}

void isRETURN(string st, int &num)
{
    string state = "T0";
    if (st[num] == 'r' && st[num + 1] == 'e' && st[num + 2] == 't' && st[num + 3] == 'u' && st[num + 4] == 'r' && st[num + 5] == 'n')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RETURN");
        t_v.push_back(" ");
        num += 6;
    }
}

void isWHILE(string st, int &num)
{
    string state = "T0";
    if (st[num] == 'w' && st[num + 1] == 'h' && st[num + 2] == 'i' && st[num + 3] == 'l' && st[num + 4] == 'e')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("WHILE");
        t_v.push_back(" ");
        num += 5;
    }
}

void isCONDITION(string st, int &num)
{
    string state = "T0";
    if (st[num] == 'i' && st[num + 1] == 'f' && !(isalpha(st[num+2]) == 1 || isalpha(st[num+2]) == 2) && !(isdigit(st[num+2]) != 0)) //if (if �ٷ� �ڿ� ���ڳ� ���ĺ��� ���� �ʴ� ���)
    {
        state = "T3";
        temp = "if";
        num += 2;
    }
    else if (st[num] == 'e' && st[num + 1] == 'l' && st[num + 2] == 's' && st[num + 3] == 'e') //else
    {
        state = "T6";
        temp = "else";
        if (st[num + 4] == ' ' && st[num + 5] == 'i' && st[num + 6] == 'f') //else if
        {
            state = "T9";
            temp += " if";
            num += 7;
        }
        else num += 4;
    }
    else state = "false";

    if (state == "T3" || state == "T6" || state == "T9")
    {
        t_n.push_back("CONDITION");
        t_v.push_back(temp);
        temp.clear();
    }

}

void isINTEGER(string st, int &num)
{
    string state = "T0";
   
    if (st[num] == '-') 
    {

            state = "T1";
            temp = '-';
            num++;
       
    }
    if (isdigit(st[num]) != 0 && st[num] != '0')
    {
        state = "T2";
        temp += st[num];
        num++;
    }
    
    while (state == "T2" && isdigit(st[num]) != 0)
    {
        temp += st[num];
        num++;
        state = "T2";
    }
   
    if (state == "T2")
    {
        t_n.push_back("INTEGER");
        t_v.push_back(temp);
        temp.clear();
        
    }

    if (st[num] == '0') // 0�� ��� ���� ���
    {
        t_n.push_back("INTEGER");
        t_v.push_back("0");
        num++;
    }

}

void isDOT(string st, int &num)
{
    string state = "T0";
    if (st[num] == '.')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("DOT");
        t_v.push_back(" ");
        num++;
    }
}

void isWHITESPACE(string st, int &num)
{
    string state = "T0";
    
    if (st[num] == ' ')
    {
        state = "T1";
    }
    else if (st[num] == '\\')
    {
        state = "T2";
        num++;
    }

    if (state == "T2" && st[num] == 'n')
    {
        state = "T3";
        temp = "\n";
    }
    else if (state == "T2" && st[num] == 't')
    {
        state = "T4";
        temp = "\t";
    }

    if (state == "T3" || state == "T4")
    {
        t_n.push_back("WHITESPACE");
        t_v.push_back(temp);
        temp.clear();
        num += 2;
    }
    else if (state == "T1")
    {
        t_n.push_back("WHITESPACE");
        t_v.push_back(" ");
        num++;
    }
    

}


void classify(string st) //��ū���� �����س��� ���� ��ū �Լ��� ��� �ϳ��� ȣ���Ͽ� �˻�
{
    int checker=0;
    while (index < (int) st.size()) //input ���Ϸκ��� �о�� �� ���� ���� ������ �ݺ�
    // �� �Լ����� ������ ��, �ش� token���� �νĵǸ� �νĵ� token ���� �κ����� index�� �̵��ϰ� ��� ����
    {
        checker = index;
        isCOMPARISON(st, index);
        isASSIGN(st, index);
        isLPAREN(st, index);
        isRPAREN(st, index);
        isLBRACE(st, index);
        isRBRACE(st, index);
        isLBRACKET(st, index);
        isRBRACKET(st, index);
        isDOT(st, index);
        isWHILE(st, index);
        isCLASS(st, index);
        isRETURN(st, index);
        isCONDITION(st, index);
        isSEMI(st, index);
        isCOMMA(st, index);
        isCHARACTER(st, index);
        isLITERAL(st, index);
        isINT(st, index);
        isCHAR(st, index);
        isBOOLEAN(st, index);
        isBOOLSTRING(st, index);
        isSTRING(st, index);
        isARITHOPERATOR(st, index);
        isINTEGER(st, index);
        isWHITESPACE(st, index);

        if(checker == index) //ID�� ���� token�鿡 ��� ������ ���� ��� �з�->Identifier������ �ٸ� �͵鿡 ��� �ش� �ȵǴ� ��츸 Ȯ��
        isID(st, index);

        if (checker == index) //ID�� ��ġ�� index���� ������ �ʴ´ٴ� ���� �� � ��ū���� ������ �ʴ´ٴ� ���� �ǹ�-> ���� �߻�
            throw stringNum;
        
    }
}
    

int main()
{
    ifstream inFile;
    ofstream outFile;
    string str;
    string startText, inputFileName;

    cin >> startText >> inputFileName;
    if (startText == "lexical_analyzer") // �Է��� lexical_analyzer inputfilename ������ ��츸 lexical analyzer ����
    {
        inFile.open(inputFileName);
        outFile.open(inputFileName+ "_output.txt");
    }
    else //lexical_analyzer�� ����� �Է����� ���� ��� ����
    {
        cout << "<ERROR>: Input should start with \"lexical_analyzer\"" << endl; 
        return 0;
    }

        if (inFile.is_open())
        {
            while (!inFile.eof())
            {
                index = 0; //index �ʱ�ȭ, index�� input file���� �ҷ��� �� �ٿ��� lexical analyzer�� ���� �з� �Ϸ�� ������ ����Ŵ
                getline(inFile, str); //input���Ͽ��� �� ���� �о�ͼ� str�� ����
                stringNum++; //���� �о�� �� ��ȣ, error report�ÿ� ���

                try { classify(str); } //str�� ��ū ���� �з� ����
                catch (int stringNum) //� ��ū���� ������ �ʴ� ������ �߻��� ��, ������ �߻��� �� ��ȣ�� ����
                {
                    cout << "<ERROR>: Line " << stringNum << endl;
                    goto FILECLOSE;
                }
                
                for (int j = 0; j < (int)t_n.size(); j++) //��º�
                {
                    while (t_n[j] == "WHITESPACE") j++; //WHITESPACE�� symbol table�� ���Խ�Ű�� ����

                    outFile << "<";
                    outFile << t_n[j];
                    if (t_v[j] != " ")
                    {
                        outFile << ", ";
                        outFile << t_v[j];
                    }
                    outFile << ">" << endl;
                }
                t_v.clear();
                t_n.clear();
            }
            cout << "Finished!" << endl;

            FILECLOSE:
            inFile.close();
            outFile.close();
        }
        else //input ������ �� �� ���� ��� ����
        {
            cout << "<ERROR>: Cannot open an input file." << endl;
            outFile.close();
        }
        
    
    return 0;

}