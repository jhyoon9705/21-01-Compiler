#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
using namespace std;
vector <string> t_n, t_v;
string temp;
static int index;

void isID(string st, int& num)
{
    string state = "T0";
    if (st[num] == '_')
    {
        state = "T1";
        temp = st[num];
        num++;
    }
    else if (isalpha(st[num]) == 1 || isalpha(st[num]) == 2)
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
        else if (isalpha(st[num]) == 1 || isalpha(st[num]) == 2)
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
        }
    }
    

}

void isCHARACTER(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '\'')
    {
        state = "T1";
        i++;
    }

    if (state == "T1")
    {
        if (isalpha(st[i]) == 1 || isalpha(st[i]) == 2)
        {
            state = "T2";
            temp += st[i];
            i++;
            
        }

        else if (isdigit(st[i]) != 0)
        {
            state = "T3";
            temp += st[i];
            i++;
            
        }

        else if (st[i] == ' ')
        {
            state = "T4";
            i++;
        }

        else {
            state = "false";

        }
    }

    if (st[i] == '\'')
    {
        t_n.push_back("CHARACTER");
        t_v.push_back(temp);
    }
    else state = "false";

   
}

void isLITERAL(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '\"')
    {
        state = "T1";
        i++;
    }
    else state = "false";

    while (state =="T1" || state == "T7" && st[i] != '\"')
    {
        temp += st[i];
        i++;
        state = "T7";
    }
    if (st[i] == '\"')
    {
        t_n.push_back("LITERAL");
        t_v.push_back(temp);
    }
}

void isCOMPARISON(string st)
{
    string state = "T0";
    int i = 0;

    if (st[i] == '<')
    {
        state = "T1";
        temp += st[i];
        i++;
    }
    else if (st[i] == '>')
    {
        state = "T2";
        temp += st[i];
        i++;
    }
    else if (st[i] == '!')
    {
        state = "T3";
        temp += st[i];
        i++;
    }
    else if (st[i] == '=')
    {
        state = "T4";
        temp += st[i];
        i++;
    }
    else state = "false";

    if (st[i] == '=')
    {
        state = "T5";
        temp += st[i];
    }

    if (state == "T1" || state == "T2" || state == "T5")
    {
        t_n.push_back("COMPARISON");
        t_v.push_back(temp);
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

void isCHAR(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 'c' && st[i + 1] == 'h' && st[i + 2] == 'a' && st[i+3] == 'r')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("CHAR");
        t_v.push_back(" ");
    }
}

void isBOOLEAN(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 'b' && st[i + 1] == 'o' && st[i + 2] == 'o' && st[i + 3] == 'l' && st[i+4]=='e' && st[i+5] == 'a' && st[i+6] == 'n')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("BOOLEAN");
        t_v.push_back(" ");
    }
}

void isSTRING(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 's' && st[i + 1] == 't' && st[i + 2] == 'r' && st[i + 3] == 'i' && st[i+4]=='n' && st[i+5]=='g')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("STRING");
        t_v.push_back(" ");
    }
}

void isBOOLSTRING(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 't' && st[i + 1] == 'r' && st[i + 2] == 'u' && st[i + 3] == 'e')
    {
        state = "T9";
        temp = "true";
    }
    if (st[i] == 'f' && st[i + 1] == 'a' && st[i + 2] == 'l' && st[i + 3] == 's' && st[i + 4] == 'e')
    {
        state = "T8";
        temp = "false";
    }

    if (state == "T8" || state == "T9")
    {
        t_n.push_back("BOOLSTRING");
        t_v.push_back(temp);
    }
    else state = "false";
}

void isLPAREN(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '(')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("LPAREN");
        t_v.push_back(" ");
    }
}

void isRPAREN(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == ')')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RPAREN");
        t_v.push_back(" ");
    }
}

void isLBRACE(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '{')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("LBRACE");
        t_v.push_back(" ");
    }
}

void isRBRACE(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '}')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RBRACE");
        t_v.push_back(" ");
    }
}

void isLBRACKET(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '[')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("LBRACKET");
        t_v.push_back(" ");
    }
}

void isRBRACKET(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == ']')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RBRACKET");
        t_v.push_back(" ");
    }
}

void isSEMI(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == ';')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("SEMI");
        t_v.push_back(" ");
    }
}

void isCOMMA(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == ',')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("COMMA");
        t_v.push_back(" ");
    }
}

void isASSIGN(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '=')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("ASSIGN");
        t_v.push_back(" ");
    }
}

void isARITHOPERATOR(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '+')
    {
        state = "T1";
        
    }
    else if (st[i] == '-')
    {
        state = "T2";
    }
    else if (st[i] == '*')
    {
        state = "T3";
    }
    else if (st[i] == '/')
    {
        state = "T4";
    }
    else state = "false";

    if (state == "T1" || state == "T2" || state == "T3" || state == "T4")
    {
        temp = st[i];
        t_n.push_back("ARITHOPERATOR");
        t_v.push_back(temp);
    }
}

void isCLASS(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 'c' && st[i + 1] == 'l' && st[i + 2] == 'a' && st[i + 3] == 's' && st[i + 4] == 's')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("CLASS");
        t_v.push_back(" ");
    }
}

void isRETURN(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 'r' && st[i + 1] == 'e' && st[i + 2] == 't' && st[i + 3] == 'u' && st[i + 4] == 'r' && st[i + 5] == 'n')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("RETURN");
        t_v.push_back(" ");
    }
}

void isWHILE(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 'w' && st[i + 1] == 'h' && st[i + 2] == 'i' && st[i + 3] == 'l' && st[i + 4] == 'e')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("WHILE");
        t_v.push_back(" ");
    }
}

void isCONDITION(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == 'i' && st[i + 1] == 'f')
    {
        state = "T3";
        temp = "if";
    }
    else if (st[i] == 'e' && st[i + 1] == 'l' && st[i + 2] == 's' && st[i + 3] == 'e')
    {
        state = "T6";
        temp = "else";
    }
    else state = "false";

    if (state == "T6" && st[i + 4] == ' ' && st[i + 5] == 'i' && st[i + 6] == 'f')
    {
        state = "T9";
        temp += " if";
    }

    if (state == "T3" || state == "T6" || state == "T9")
    {
        t_n.push_back("CONDITION");
        t_v.push_back(temp);
    }

}

void isINTEGER(string st)
{
    string state = "T0";
    int i = 0;

    if (st[i] == '-')
    {
        state = "T1";
        temp = '-';
        i++;
    }
    if (isdigit(st[i]) != 0 && st[i] != '0')
    {
        state = "T2";
        temp += st[i];
        i++;
    }
    
    while (state == "T2" || state =="T3" && isdigit(st[i]) != 0)
    {
        temp += st[i];
        i++;
        state = "T3";
    }
   
    if (state == "T2" || state == "T3")
    {
        t_n.push_back("INTEGER");
        t_v.push_back(temp);
    }

}

void isZERO(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '0')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("ZERO");
        t_v.push_back(" ");
    }
}

void isDOT(string st)
{
    string state = "T0";
    int i = 0;
    if (st[i] == '.')
    {
        state = "T1";
    }
    else state = "false";

    if (state == "T1")
    {
        t_n.push_back("DOT");
        t_v.push_back(" ");
    }
}

void isWHITESPACE(string st, int &num)
{
    string state = "T0";
    
    if (st[num] == ' ')
    {
        state = "T1";
    }
    if (st[num] == '\\')
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

    if (state == "T1" || state == "T3" || state == "T4")
    {
        t_n.push_back("WHITESPACE");
        t_v.push_back(temp);
        num++;
    }
    else state = "false";

}

    

    /*if (isalpha(st[i]) == 1 || isalpha(st[i]) == 2)
    {
        state = "T2";
        temp += st[i];
        i++;
    }
    else if (isdigit(st[i]) != 0)
    {
        state = "T3";
        temp += st[i];
        i++;
    }
    else if (st[i] == ' ')
    {
        state = "T4";
        temp += st[i];
        i++;
    }
    else {
        state = "false";
    }

    for (i; i < (int)st.size(); i++)
    {
        if (isalpha(st[i]) == 1 || isalpha(st[i]) == 2)
        {
            state = "T5";

        }

    }*/





int main() {

    int index = 0;
    string str = "int a";
   
    //isID(str);
    //isCHARACTER(str);
    isINT(str, index);
    isWHITESPACE(str, index);
    isID(str, index);
    for (int j = 0; j < (int)t_n.size(); j++)
    {
        cout << t_n[j] << " " ;
        cout << t_v[j] << endl;
    }
   cout << index << endl;
   

    return 0;
}

/*int main()
{
    ifstream readFile;
    readFile.open("input.txt");    //파일 열기

    if (readFile.is_open())
    {
        while (!readFile.eof())
        {
           

            //2. std::getline.
            string str;
            getline(readFile, str); //input파일에서 한 줄을 읽어와서 str에 저장

            
            cout << str << endl;  



        }
        readFile.close();    //파일 닫아줍니다.
    }
    return 0;

}*/