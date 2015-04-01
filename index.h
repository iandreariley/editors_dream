/*
 *  Index.h
 *  EditorsDreamTree
 *
 *  Created by Ian Riley on 10/28/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include <sstream>
#include "binaryTree.h"
//#include "Word.h"

using namespace std;

class Index
{
public:
    Index();
    ~Index();
    Index(const Index &other);

    Index& operator = (const Index &other);
    void insert(node<string> w);
    void topTen(string words[], int counts[]);
    int* letterCount();

    friend ostream& operator<<(ostream &outs, Index &ind);

private:
    int size;
    binaryTree< node<string> > orchard[26];
    void nukem();
    void copy(const Index& other);
};

#endif
