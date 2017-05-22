//2017.05
//
//
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#define BUFFER_SIZE 8192
#define SAMPLE_PATH "D:/tmp/purer/"

using namespace std;

inline bool isText(char c);
inline bool isNewList(char c);
// for calculation
const int result = 3;
vector<int> value[3];
int similarity[3];
string TmpStr, ResultStr;

int main()
{
    std::filebuf sample[3];
    std::filebuf output; 
    //output.open(string(SAMPLE_PATH) + string("traning.ar.tok-zh.tok.A4"), ios::out);
    output.open(string("traning.ar.tok-zh.tok.A4"), ios::out);

    std::istream *in[3] = { nullptr, nullptr, nullptr };
    std::ostream *out = new ostream(&output);

    for (auto i = 0; i < 3; ++i)
    { 
        /*if (sample[i].open(string(SAMPLE_PATH) + string("traning.ar.tok-zh.tok.A") + to_string(i + 1), ios::in) == nullptr)
            cout << "open failed:" << i << endl;*/
        if (sample[i].open(string("traning.ar.tok-zh.tok.A") + to_string(i + 1), ios::in) == nullptr)
            cout << "open failed:" << i << endl;
        in[i] = new istream(&sample[i]);
    }
    
    string buffer[3];
    char buffer_c[3][BUFFER_SIZE];
    string trash;
    char newline = '\n';

    out->clear();

    getline(*in[0], trash);
    getline(*in[1], trash);
    getline(*in[2], trash);
    out->write(trash.c_str(), trash.size());
    out->write(&newline, 1);

    getline(*in[0], trash);
    getline(*in[1], trash);
    getline(*in[2], trash);
    out->write(trash.c_str(), trash.size());
    out->write(&newline, 1);

    getline(*in[0], buffer[0]);
    do
    {   
        getline(*in[1], buffer[1]);
        getline(*in[2], buffer[2]);

        if (buffer[0][0] != 'N')
            cout << "fuck";

        for (auto i = 0; i < 3; ++i)
        {
            memset(buffer_c[i], 0, BUFFER_SIZE);
            strcpy(buffer_c[i], buffer[i].c_str());
        }

        int ptr = 0;
        char cur = 0;
        int val;
        int pool[4];
        int log = 0;

        while (true)
        {
            TmpStr.clear();
            ResultStr.clear();
            InitOK:
            cur = buffer_c[0][ptr];
            if (cur == 0)
                break;

            if (isText(cur))
            {
                out->write(&cur, 1);
                //TmpStr += cur;
                ptr++;
                goto InitOK;
            }
            else
            {
                int index = 0;
                for (int i = 0; i < 3; ++i)
                    value[i].clear();

                while (!isNewList(buffer_c[0][ptr]))
                {
                    if (isText(buffer_c[0][ptr]))
                    {
                        //out->write(&buffer_c[0][ptr], 1);
                        TmpStr += buffer_c[0][ptr++];
                        continue;
                    }

                    int tmpptr = 0;
                    int start_ptr = ptr;
                    //because un sync of the reading while the length of number is not stable
                    //so this can not be dealt with loop
                    tmpptr = start_ptr;
                    log = 0;
                    pool[0] = pool[1] = pool[2] = pool[3] = 0;
                    val = 0;
                    pool[log] = buffer_c[0][tmpptr++];
                    while (!isText(buffer_c[0][tmpptr]))
                        pool[++log] = buffer_c[0][tmpptr++];
                    for (auto j = 0; j <= log; ++j)
                        val += (pool[j] - '0') * pow(10, log - j);
                    value[0].push_back(val);

                    ptr = tmpptr;

                    tmpptr = start_ptr;
                    log = 0;
                    pool[0] = pool[1] = pool[2] = pool[3] = 0;
                    val = 0;
                    pool[log] = buffer_c[1][tmpptr++];
                    while (!isText(buffer_c[1][tmpptr]))
                        pool[++log] = buffer_c[1][tmpptr++];
                    for (auto j = 0; j <= log; ++j)
                        val += (pool[j] - '0') * pow(10, log - j);
                    value[1].push_back(val); 
                    tmpptr = start_ptr;

                    log = 0;
                    pool[0] = pool[1] = pool[2] = pool[3] = 0;
                    val = 0;
                    pool[log] = buffer_c[2][tmpptr++];
                    while (!isText(buffer_c[2][tmpptr]))
                        pool[++log] = buffer_c[2][tmpptr++];
                    for (auto j = 0; j <= log; ++j)
                        val += (pool[j] - '0') * pow(10, log - j);
                    value[2].push_back(val);                    

                    index++;
                    char tmpval = '@';
                    //strcat(tmpval, to_string(index).c_str());
                    //out->write(tmpval, index / 10 + 2);
                    TmpStr += tmpval;            
                }
                //similarity[0] : A1 : A2
                //similarity[1] : A1 : A3
                //similarity[2] : A2 : A3
                similarity[0] = similarity[1] = similarity[2] = index;
                for (auto vi = 0; vi < index; ++vi)
                {
                    if (value[0][vi] != value[1][vi])
                        similarity[0]--;
                    if (value[0][vi] != value[2][vi])
                        similarity[1]--;
                    if (value[1][vi] != value[2][vi])
                        similarity[2]--;
                }
                int min_i = 0;
                int min = similarity[min_i];
                if (min > similarity[1])
                    min_i = 1;
                if (min > similarity[2])
                    min_i = 2;


                int r_index = 0;
                for (auto t_len = 0; t_len < TmpStr.length(); )
                {
                    if (TmpStr[t_len] != '@')
                        ResultStr += TmpStr[t_len++];
                    else
                    {
                        ResultStr += to_string(value[2 - min_i][r_index++]); 
                        //t_len += to_string(value[2 - min_i][r_index++]).length();
                        t_len++;
                    }
                }
                out->write(ResultStr.c_str(), ResultStr.length());
            }
        }
        out->write(&newline, 1);

        getline(*in[0], trash);
        getline(*in[1], trash);
        getline(*in[2], trash);
        out->write(trash.c_str(), trash.size());
        out->write(&newline, 1);

        getline(*in[0], trash);
        getline(*in[1], trash);
        getline(*in[2], trash);
        out->write(trash.c_str(), trash.size());
        out->write(&newline, 1);

    } while (getline(*in[0], buffer[0]));

    output.close();
}

inline bool isText(char c)
{
    if (c > '9' || c < '0')
        return true;
    return false;
}

inline bool isNewList(char c)
{
    if (isText(c) && c != '(' && c != ')' && c != '{' && c != '}' && c != ' ' && c != ',')
        return true;
    return false;
}
