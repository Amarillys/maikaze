//2017.05
//
//
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#define BUFFER_SIZE 8192

using namespace std;

inline bool isText(char c);
inline bool isNewList(char c);
// for calculation
const int result = 3;
vector<int> value[3];
int similarity[3];
string TmpStr, ResultStr;

int main(int argc, char** argv)
{
    std::filebuf sample[3];
    std::filebuf output;
    if (argc < 5)
    {
        cout << "usage: purer [standard_A1] [Input_A2] [Input_A3] [Output_A4]";
        return -1;
    }
    output.open(argv[4], ios::out);
    //output.open(string("traning.ar.tok-zh.tok.A4"), ios::out);

    std::istream *in[3] = { nullptr, nullptr, nullptr };
    std::ostream *out = new ostream(&output);

    for (auto i = 0; i < 3; ++i)
    {   
        if (sample[i].open(argv[i+1], ios::in) == nullptr)
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
    //loop a line in the file
    do
    {   
        getline(*in[1], buffer[1]);
        getline(*in[2], buffer[2]);

        //detect whether there has a value line start without a "NULL"
        if (buffer[0][0] != 'N')
            cout << "fuck";

        //copy the line to the buffer to be deal
        for (auto i = 0; i < 3; ++i)
        {
            memset(buffer_c[i], 0, BUFFER_SIZE);
            strcpy(buffer_c[i], buffer[i].c_str());
        }

        //the pointer of to three file
        int ptr[3] = { 0, 0, 0 };
        char cur = 0;
        int val;
        int pool[4];
        int log = 0;

        bool isEnd = false;

        //initilize the vector
        int index = 0;

        //detect if it is the first file(standard file)
        bool isSTD = true;
    Loop:
        if (isEnd)
            goto LoopEnd;
        isSTD = true;
        index = 0;
        TmpStr.clear();
        ResultStr.clear();

        //initilize the vector
        for (auto i = 0; i < 3; ++i)
        {
            value[i].clear();
            //detect whether this line is end
            cur = buffer_c[0][ptr[i]];

            while (isText(cur))
            {
                if (isSTD)
                    out->write(&cur, 1);
                ptr[i]++;
                cur = buffer_c[i][ptr[i]];
                if (cur == 0)
                {
                    isEnd = true;
                    goto Loop;
                }
            }

            //loop all the number in a list
            while (!isNewList(buffer_c[i][ptr[i]]))
            {
                if (isText(buffer_c[i][ptr[i]]))
                {
                    if (isSTD)
                        TmpStr += buffer_c[i][ptr[i]];
                    ptr[i]++;
                    continue;
                }

                int tmpptr = 0;
                int start_ptr = ptr[i];
                //because un sync of the reading while the length of number is not stable
                //so this can not be dealt with loop
                tmpptr = start_ptr;
                log = 0;
                pool[0] = pool[1] = pool[2] = pool[3] = 0;
                val = 0;
                pool[log] = buffer_c[i][tmpptr++];
                while (!isText(buffer_c[i][tmpptr]))
                    pool[++log] = buffer_c[i][tmpptr++];
                for (auto j = 0; j <= log; ++j)
                    val += (pool[j] - '0') * pow(10, log - j);
                value[i].push_back(val);
                char tmpval = '@';
                if (isSTD)
                {
                    TmpStr += tmpval;
                    index++;
                }
                ptr[i] = tmpptr;
            }
            isSTD = false;
        }

        //similarity[0] : A1 : A2
        //similarity[1] : A1 : A3
        //similarity[2] : A2 : A3
        similarity[0] = similarity[1] = similarity[2] = index;
        int min_i = 2;
        if (value[1].size() != value[0].size() || value[2].size() != value[0].size())
        {
            min_i = 2;
        }
        else
        {
            for (auto vi = 0; vi < index; ++vi)
            {
                if (value[0][vi] != value[1][vi])
                    similarity[0]--;
                if (value[0][vi] != value[2][vi])
                    similarity[1]--;
                if (value[1][vi] != value[2][vi])
                    similarity[2]--;
            }
            int min = similarity[min_i];
            if (min > similarity[1])
                min_i = 1;
            min = similarity[min_i];
            if (min > similarity[0])
                min_i = 0;
        }


        int r_index = 0;
        for (auto t_len = 0; t_len < TmpStr.length(); )
        {
            if (TmpStr[t_len] != '@')
                ResultStr += TmpStr[t_len++];
            else
            {
                ResultStr += to_string(value[2 - min_i][r_index++]); 
                t_len++;
            }
        }
        out->write(ResultStr.c_str(), ResultStr.length());
        goto Loop;

    LoopEnd:
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
    if (isText(c) && c != '(' && c != ')' && c != '{' && c != '}' && c != ' ' && c != ',' && c != 0)
        return true;
    return false;
}
