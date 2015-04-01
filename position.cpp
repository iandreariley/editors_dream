/*
 *  position.cpp
 *
 *
 *  Created by Ian Riley on 10/31/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "position.h"

position::position()
{
    par = 0;
    sent = 0;
}

position::position(int p, int s)
{
    par = p;
    sent = s;
}

position::~position()
{}

position::position(const position &p)
{
    par = p.par;
    sent = p.sent;
}

position& position::operator=(const position &p)
{
    if(this != &p)
    {
        par = p.par;
        sent = p.sent;
    }
    return *this;
}

ostream& operator<<(ostream &outs, const position &pos)
{
    outs<<"("<<pos.par<<", "<<pos.sent<<")";
    return outs;
}

istream& operator>>(istream &ins, position &pos)
{
    char junk;
    int par;
    int sent;
    if(ins>>junk>>par>>junk>>sent>>junk)
    {
        pos.par = par;
        pos.sent = sent;
    }
    return ins;
}
