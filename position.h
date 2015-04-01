#ifndef POSITION_H
#define POSITION_H

#include <iostream>

using namespace std;

struct position
{
    int par;
    int sent;

    position();
    position(int p, int s);
    ~position();
    position(const position &p);
    position& operator=(const position &p);

    friend ostream& operator<<(ostream &outs, const position &p);
    friend istream& operator>>(istream &ins, position &p);

};

#endif
