/*
 *  Index.cpp
 *  EditorsDreamTree
 *
 *  Created by Ian Riley on 10/28/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Index.h"

Index::Index()
{size = 0;}

Index::~Index()
{
    nukem();
}

Index::Index(const Index &other)
{
    copy(other);
}

Index& Index::operator=(const Index &other)
{
    if(this != &other)
        copy(other);
    return *this;
}

void Index::insert(node<string> theword)
{
    ++size;
    orchard[theword.data[0] - 65]<<theword;
}

void Index::nukem()
{
    for(int i = 0; i < 26; i++)
        orchard[i].clear();
}

void Index::copy(const Index &other)
{
    for(int i = 0; i < 26; i++)
        orchard[i] = other.orchard[i];
}

void Index::topTen(string words[], int counts[])
{
    string line = "";
    int count;
    for(int i = 0; i < 10; i++)
    {
        words[i] = line;
        counts[i] = 0;
    }
    for(int i = 0; i < 26; i++)
    {
        stringstream ss;
        ss<<orchard[i];
        while(getline(ss, line))
        {
            while(line == "" && getline(ss, line));
            ss>>count;
            for(int i = 0; i < 10; i++)
            {
                if(count > counts[i])
                {
                    for(int j = 9; j > i; j--)
                    {
                        words[j] = words[j-1];
                        counts[j] = counts[j-1];
                    }
                    words[i] = line;
                    counts[i] = count;
                    break;
                }
            }
            while(line != "" && getline(ss, line));
        }
    }
}

int* Index::letterCount()
{
    int* counts = new int[26];
    for(int i = 0; i < 26; i++)
        counts[i] = orchard[i].getSize();
    return counts;
}

ostream& operator<<(ostream &outs, Index &ind)
{
    for(int i = 0; i < 26; i++)
        outs<<(ind.orchard[i]);
    return outs;
}
