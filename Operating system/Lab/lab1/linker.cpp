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

ifstream a_file;

int base;
int linenum;
int lineoffset;
int num_instr;
int module_num;
string str;
char *cstr;
string token;
unordered_map<string, int> symbolList;  // used for recording symbol table
unordered_map<string, int> moduleList;  // used for recording module of symbol
vector<string> defineOrder;             // order of the unique symbol
unordered_map<string, bool> dep_symbol; // if symbol is unique
vector<string> insOrder;                // instructions
vector<string> useOrder;                // order of use list
unordered_map<string, bool> useList;    // use for check if the symbol in the use list is used
unordered_map<string, bool> use_symbol; // if symbol is used
vector<char *> tokenList;               // store token in line
vector<string> notUsed;                 // in uselist but not use
unordered_map<string, int> notUsedList; // in uselist but not use, module number
istringstream iss;
int linebase;
int finalOffset;

enum PARSE_ERROR
{
    NUM_EXPECTED,
    SYM_EXPECTED,
    ADDR_EXPECTED,
    SYM_TOO_LONG,
    TOO_MANY_DEF_IN_MODULE,
    TOO_MANY_USE_IN_MODULE,
    TOO_MANY_INSTR
};

void _perseerror(int errcode)
{
    static const char *errstr[] = {
        "NUM_EXPECTED",           // Number expect, anything >= 2^30 is not a number either
        "SYM_EXPECTED",           // Symbol Expected
        "ADDR_EXPECTED",          // Addressing Expected which is A/E/I/R
        "SYM_TOO_LONG",           // Symbol Name is too long
        "TOO_MANY_DEF_IN_MODULE", //>16
        "TOO_MANY_USE_IN_MODULE", // > 16
        "TOO_MANY_INSTR",         // total num_instr exceeds memory size (512)
    };
    printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]);
}

bool nextline()
{
    if (getline(a_file, str))
    {
        finalOffset = (int)str.size() + 1;
        linenum++;
        iss.clear();
        iss.str(str);
        return true;
    }
    return false;
}

bool getToken()
{
    while (!(iss >> token))
    {
        linebase = 0;
        lineoffset = 0;

        if (!nextline())
        {
            lineoffset = finalOffset;
            return false;
        }
    }

    int sub = str.find(token);
    lineoffset = linebase + sub + 1;
    linebase += sub + token.length();
    str = str.substr(sub + token.length());
    return true;
}

bool isalnums(string token)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (!isalnum(token[i]))
        {
            return false;
        }
    }
    return true;
}

bool isdigits(string token)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (!isdigit(token[i]))
        {
            return false;
        }
    }
    return true;
}

int readInt()
{
    bool res = getToken();

    if (isdigits(token) && res)
    {
        return stoi(token);
    }
    else
        throw PARSE_ERROR::NUM_EXPECTED;
}

bool isSym(string token)
{
    return isalpha(token[0]) && isalnums(token.substr(1));
}

char readIEAR()
{

    if (getToken() && (token == "R" || token == "E" || token == "I" || token == "A"))
    {
        return token[0];
    }
    else
        throw PARSE_ERROR::ADDR_EXPECTED;
}

template <class T>
bool isInMap(string symbol, unordered_map<string, T> &map)
{

    return map.find(symbol) != map.end();
}

void createSymbol(string sym, int val)
{
    if (isInMap(sym, symbolList))
    {
        dep_symbol[sym] = true;
        printf("Warning: Module %d: %s redefined and ignored\n", module_num, sym.c_str());
    }
    else
    {
        defineOrder.push_back(sym);
        symbolList[sym] = base + val;
        moduleList[sym] = module_num;
        dep_symbol[sym];
        use_symbol[sym];
    }
}

string readSym()
{

    if (getToken() && isSym(token))
    {
        if (token.size() > 16)
        {
            throw PARSE_ERROR::SYM_TOO_LONG;
        }

        return token;
    }
    else
        throw PARSE_ERROR::SYM_EXPECTED;
}

void print_symbol_table()
{
    for (int i = 0; i < defineOrder.size(); i++)
    {
        string key = defineOrder.at(i);
        string err;
        if (dep_symbol[key] == true)
        {
            err = "Error: This variable is multiple times defined; first value used";
        }
        cout << key << "=" << symbolList[key] << " " << err << endl;
    }
    cout << endl;
}

void print_memory_map()
{
    for (int i = 0; i < insOrder.size(); i++)
    {
        string row(3 - to_string(i).size(), '0');
        printf("%s%d: %s\n", row.c_str(), i, insOrder.at(i).c_str());
    }
    cout << endl;
}
void transAbs(char addressmode, int instruction, int instcount)
{
    string address;
    int operand = instruction % 1000;
    int opcode = instruction / 1000;
    string err;

    if (addressmode == 'I')
    {
        if (instruction >= 10000)
        {
            err = "Error: Illegal immediate value; treated as 9999";
            address = "9999";
        }
        else
        {
            address = to_string(instruction);
        }
    }
    else if (opcode >= 10)
    {
        err = "Error: Illegal opcode; treated as 9999";
        address = "9999";
    }
    else if (addressmode == 'A')
    {
        if (operand > 512)
        {
            err = "Error: Absolute address exceeds machine size; zero used";
            address = to_string(1000 * opcode);
        }
        else
        {
            address = to_string(instruction);
        }
    }
    else if (addressmode == 'R')
    {
        if (operand > instcount)
        {
            err = "Error: Relative address exceeds module size; zero used";
            operand = 0;
        }
        address = to_string(1000 * opcode + operand + base);
    }
    else if (addressmode == 'E')
    {
        if (operand < useOrder.size())
        {
            string key = useOrder.at(operand);
            int add = 0;
            if (isInMap(key, useList))
            {
                useList[key] = true;
            }
            if (isInMap(key, symbolList) && isInMap(key, useList))
            {
                add = symbolList[key];
                use_symbol[key] = true;
            }
            else
            {
                err = "Error: " + key + " is not defined; zero used";
            }
            address = to_string(1000 * opcode + add);
        }
        else
        {
            address = to_string(instruction);
            err = "Error: External address exceeds length of uselist; treated as immediate";
        }
    }
    if (address.size() < 4)
    {
        address = string(4 - address.size(), '0') + address;
    }

    insOrder.push_back(address + " " + err);
}

bool scanEnd()
{
    int dis = a_file.tellg();
    if (a_file >> token)
    {
        a_file.seekg(dis);
        return false;
    }
    return true;
}

bool Pass1()
{
    try
    {
        while (!a_file.eof() && !scanEnd())
        {
            vector<string> defineList;
            module_num++;
            int defcount = readInt();
            if (defcount > 16)
            {
                throw PARSE_ERROR::TOO_MANY_DEF_IN_MODULE;
            }
            for (int i = 0; i < defcount; i++)
            {
                string sym = readSym();
                int val = readInt();
                defineList.push_back(sym);
                createSymbol(sym, val);
            }
            int usecount = readInt();
            if (usecount > 16)
            {
                throw PARSE_ERROR::TOO_MANY_USE_IN_MODULE;
            }
            for (int i = 0; i < usecount; i++)
            {
                string sym = readSym();
            }
            int instcount = readInt();
            num_instr += instcount;
            if (num_instr > 512)
            {
                throw PARSE_ERROR::TOO_MANY_INSTR;
            }
            for (int i = 0; i < instcount; i++)
            {
                char addressmode = readIEAR();
                int instruction = readInt();
            }
            for (int i = 0; i < defineList.size(); i++)
            {
                string key = defineList.at(i);
                if (symbolList[key] - base >= instcount)
                {

                    printf("Warning: Module %d: %s too big %d (max=%d) assume zero relative\n", module_num, key.c_str(), symbolList[key] - base, instcount - 1);
                    symbolList[key] = base;
                }
            }

            base += instcount;
        }
        linenum = 0;
        printf("Symbol Table\n");
        print_symbol_table();
    }
    catch (PARSE_ERROR errorcode)
    {
        _perseerror(errorcode);
        return false;
    }
    return true;
}

void Pass2()
{
    base = 0;
    module_num = 0;

    while (!a_file.eof() && !scanEnd())
    {

        module_num++;
        int defcount = readInt();
        for (int i = 0; i < defcount; i++)
        {
            string sym = readSym();
            int val = readInt();
        }
        int usecount = readInt();
        for (int i = 0; i < usecount; i++)
        {
            string sym = readSym();
            useOrder.push_back(sym);
            useList[sym] = false;
        }
        int instcount = readInt();
        for (int i = 0; i < instcount; i++)
        {
            char addressmode = readIEAR();
            int instruction = readInt();
            transAbs(addressmode, instruction, instcount);
        }
        for (int i = 0; i < useOrder.size(); i++)
        {
            string key = useOrder.at(i);
            if (!useList[key])
            {
                notUsed.push_back(key);
                notUsedList[key] = module_num;
            }
        }
        base += instcount;
        useOrder.clear();
    }

    cout << endl;
    printf("Memory Map\n");
    print_memory_map();

    for (int i = 0; i < notUsed.size(); i++)
    {
        string key = notUsed.at(i);
        printf("Warning: Module %d: %s appeared in the uselist but was not actually used\n", notUsedList[key], key.c_str());
    }

    for (int i = 0; i < defineOrder.size(); i++)
    {
        string key = defineOrder.at(i);
        if (!use_symbol[key])
        {
            printf("Warning: Module %d: %s was defined but never used\n", moduleList[key], key.c_str());
        }
    }
}

int main(int argc, char **argv)
{

    a_file.open(argv[1]);
    if (Pass1())
    {
        a_file.close();
        a_file.open(argv[1]);
        Pass2();
    }

    return 0;
}