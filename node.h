#ifndef NODE_H
#define NODE_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "position.h"

using namespace std;

template<typename T>
struct node
{
    T data;
    vector<position> pos;
    int count;

    //constructors
    node();
    node(T d, int p, int s);
    node(const node<T> &other);
    node<T>& operator = (const node<T> &other);
    node<T>& operator+=(const node<T> &RHS);
    node<T>& operator--();

    template<typename U>
    friend
    ostream& operator<<(ostream& out, const node<U> &n);

    template<typename U>
    friend
    istream& operator>>(istream& in, node<U> &n);

    template<typename U>
    friend
    bool operator<(const node<U> &a, const node<U> &b);

    template<typename U>
    friend
    bool operator>(const node<U> &a, const node<U> &b);

    template<typename U>
    friend
    bool operator==(const node<U> &a, const node<U> &b);
};

template<typename T>
node<T>::node()
{
    count = 0;
}

template<typename T>
node<T>::node(T d, int p, int s)
{
    data = d;
    count = 1;
    position poser(p, s);
    pos.push_back(poser);
}

template<typename T>
node<T>::node(const node<T> &other)
{
    data = other.data;
    count = other.count;
    pos = other.pos;
}

template<typename T>
node<T>& node<T>::operator=(const node<T> &other)
{
    if(this != &other)
    {
        count = other.count;
        data = other.data;
        pos = other.pos;
    }
    return *this;
}

template<typename T>
node<T>& node<T>::operator--()
{
    --count;
    pos.pop_back();
}

template<typename T>
node<T>& node<T>::operator+=(const node<T> &RHS)
{
    if(this != &RHS)
    {
        int c = RHS.count;
        count += c;
        for(int i = 0; i < c; i++)
            pos.push_back(RHS.pos[i]);
    }
    return *this;
}

template<typename U>
ostream& operator<<(ostream& out, const node<U> &n)
{
    int c;
    out<<endl<<endl<<n.data<<" "<<endl<<(c = n.count)<<" occurances:"<<endl;
    for(int i = 0; i < c; i++)
    {
        out<<n.pos[i]<<" ";
        if((i+1)%4 == 0 && (i+1) != c)
            out<<endl;
    }
    return out;
}

template<typename U>
istream& operator>>(istream& in, node<U> &n)
{
    position poser;
    stringstream ss;
    string line;
    getline(in, line);
    ss<<line;
    ss>>n.data;
    ss>>n.count;
    while(getline(in, line))
    {
        ss<<line;
        while(ss>>poser)
            n.pos.push_back(poser);
    }
    return in;
}

template<typename U>
bool operator<(const node<U> &a, const node<U> &b)
{
    return a.data < b.data;
}

template<typename U>
bool operator>(const node<U> &a, const node<U> &b)
{
    return a.data > b.data;
}

template<typename U>
bool operator==(const node<U> &a, const node<U> &b)
{
    return a.data == b.data;
}


#endif
