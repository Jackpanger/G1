#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;

ifstream a_file("input");
int base;
int linenum;
int lineoffset;
int num_instr;
int module_num;
string str;
char *cstr;
string token;
bool ifEmpty;
bool end;
unordered_map<string, int> symbolList;  // used for recording symbol table
vector<string> defineOrder;             // order of the unique symbol
unordered_map<string, bool> dep_symbol; // if symbol is unique
vector<string> insOrder;                // instructions
vector<string> useOrder;                // order of use list
unordered_map<string, bool> useList;    // use for check if the symbol in the use list is used
unordered_map<string, bool> use_symbol; // if symbol is used
vector<char *> tokenList;               // store token in line
int tokenpos;
istringstream iss;
int linebase;

bool nextline()
{
    if (getline(a_file, str))
    {
        linenum++;
        iss.clear();
        iss.str(str);
        // printf("next line %s\n", token.c_str());

        return true;
    }
    return false;
}

bool getToken()
{
    while (!(iss >> token))
    {
        linebase = 0;

        if (!nextline())
        {
            return false;
        }
    }

    int sub = str.find(token);
    lineoffset = linebase + sub + 1;
    linebase += sub + token.length();
    str = str.substr(sub + token.length());
    printf("get u %s\n", token.c_str());

    // printf("tellg %d\n",lineoffset);
    return true;
}
int main(int argc, char **argv)
{

    while (getToken())
    {
        /* code */
    }

    return 0;
}