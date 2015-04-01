#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <sstream>
#include "node.h"

using namespace std;

enum ORDER {IN_ORDER, POST_ORDER, PRE_ORDER};

template<typename T>
class binaryTree
{
public:
    binaryTree();
    ~binaryTree();
    binaryTree(const binaryTree& other);
    binaryTree<T>& operator = (const binaryTree& other);

    int depth();
    void insert(T d);
    bool remove(T d);
    int find(T d);
    ORDER getTraversal();
    void setTraversal(ORDER traversal);
    int depth(int root);
    int totalNodes();
    int totalDataItems();
    void clear();
    void print();
    void balance();
    int getSize();
    binaryTree<T>& operator<<(T d);




    template<typename U>
    friend
    ostream& operator<<(ostream& out, binaryTree<U> &n);

    template<typename U>
    friend
    istream& operator>>(istream& in, binaryTree<U> &n);


private:
    vector<T*> tree;
    int size, cap;
    ORDER traverse;
    //PRIVATE!!
    void balance(int r);
    void print(int r, int depth);
    void display(int r, ostream &out);
    void init(int begin, int end);
    void insert(T* d, int root);
    void insert(T d, int root);
    void upsize(int d);
    int totalNodes(int r);
    void rotateRight(int r);
    void rotateLeft(int r);
    void moveTreeDownLeft(int source, int dest);
    void moveTreeUpLeft(int source, int dest);
    void moveTreeDownRight(int source, int dest);
    void moveTreeUpRight(int source, int dest);
    int find(T d, int r);
    int findLeftMost(int r);
    int findRightMost(int r);
    void nukem();
    void copy(const binaryTree &other);
    int right(int index);
    int left(int index);
    int parent(int index);
    int log2floor(int);
    int Cap();
};

template<typename T>
binaryTree<T>::binaryTree()
{
    size = 0;
    traverse = IN_ORDER;
    tree.reserve(3);
    for(int i = 0; i < 3; i++)
        tree.push_back(NULL);
    cap = 3;
}

template<typename T>
binaryTree<T>::~binaryTree()
{
    nukem();
}

template<typename T>
binaryTree<T>::binaryTree(const binaryTree& other)
{
    copy(other);
}

template<typename T>
binaryTree<T>& binaryTree<T>::operator = (const binaryTree& other)
{
    if(this != &other)
        copy(other);
    return *this;
}

template<typename T>
int binaryTree<T>::depth()
{
    return depth(0);
}

template<typename T>
int binaryTree<T>::depth(int root)
{
    if(tree[root])
        return 1 + max(depth(right(root)), depth(left(root)));
    return -1;
}

template<typename T>
void binaryTree<T>::insert(T* d, int root)
{
    if(tree[root])
    {
        if(d->data == tree[root]->data)
        {
            *tree[root] += *d;
            delete d;
        }
        else if(d->data > tree[root]->data)
            insert(d, right(root));
        else
            insert(d, left(root));
    }
    else
    {
        tree[root] = d;
        if(log2floor(root+1) >= log2floor(cap));
            upsize();
    }
}

template<typename T>
void binaryTree<T>::insert(T d, int root)
{
    if(tree[root])
    {
        if(d == *tree[root])
            *tree[root] += d;
        else if(d > *tree[root])
            insert(d, right(root));
        else
            insert(d, left(root));
    }
    else
    {
        tree[root] = new T(d);
        if(log2floor(root+1) >= log2floor(cap))
            upsize(log2floor(cap));
    }
}

template<typename T>
void binaryTree<T>::insert(T d)
{
    size++;
    insert(d, 0);
    balance();
}

template<typename T>
bool binaryTree<T>::remove(T d)
{
    int get = find(d, 0);
    if(get >= 0)
    {
        if(tree[get]->count > 1)
            --(*tree[get]);
        else
        {
            delete tree[get];
            if(tree[left(get)])
                moveTreeUpRight(left(get), get);
            else
                moveTreeUpLeft(right(get), get);
        }
        --size;
        balance();
        return true;
    }
    return false;
}

template<typename T>
int binaryTree<T>::find(T d, int root)
{
    int flse = -1;
    if(tree[root])
    {
        if(*tree[root] == d)
            return root;
        else
            return d < *tree[root] ? find(d, left(root)) : find(d, right(root));
    }
    return flse;
}

template<typename T>
int binaryTree<T>::find(T d)
{
    return find(d, 0);
}

template<typename T>
ORDER binaryTree<T>::getTraversal()
{
    return traverse;
}

template<typename T>
void binaryTree<T>::setTraversal(ORDER traversal)
{
    traverse = traversal;
}

template<typename T>
int binaryTree<T>::totalNodes()
{
    return totalNodes(0);
}

template<typename T>
int binaryTree<T>::getSize()
{
    return size;
}

template<typename T>
int binaryTree<T>::totalNodes(int r)
{
    int count = 0, theSize = tree.capacity();
    for(int i = 0; i < theSize; i++)
        if(tree[i])
            ++count;
    return count;

}

template<typename T>
int binaryTree<T>::totalDataItems()
{
    return size;
}

template<typename T>
void binaryTree<T>::clear()
{
    nukem();
}

template<typename T>
void binaryTree<T>::display(int r, ostream& out)
{
    if(traverse == PRE_ORDER)
        out<<*tree[r];
    if(tree[left(r)])
        display(left(r), out);
    if(traverse == IN_ORDER)
        out<<*tree[r];
    if(tree[right(r)])
        display(right(r), out);
    if(traverse == POST_ORDER)
        out<<*tree[r];
}

template<typename T>
void binaryTree<T>::print()
{
    print(0, depth());
}

template<typename T>
void binaryTree<T>::balance()
{
    balance(0);
}

template<typename T>
void binaryTree<T>::balance(int r)
{
    if(tree[r])
    {
        balance(left(r));
        balance(right(r));
        int diff= depth(left(r)) - depth(right(r));
        while(diff < -1 || diff > 1)
        {
            if(diff > 0)
                rotateRight(r);
            else
                rotateLeft(r);
            diff = depth(left(r)) - depth(right(r));
        }
    }
}

template<typename T>
binaryTree<T>& binaryTree<T>::operator<<(T d)
{
    insert(d, 0);
    ++size;
    return *this;
}

template<typename T>
void binaryTree<T>::upsize(int d)
{
    int newCap = ((int)pow(2.0,(d+2)) - 1);
    if(tree.capacity() >= newCap)
    {
        for(int i = cap; i < newCap; i++)
            tree[i] = NULL;
    }
    else
    {
        for(int i = 0; i < (cap + 1); i++)
            tree.push_back(NULL);
    }
    cap = newCap;
}

template<typename T>
void binaryTree<T>::init(int begin, int end)
{
    for(int i = begin; i < end; i++)
        tree[i] = NULL;
}

template<typename T>
void binaryTree<T>::rotateRight(int r)
{
    if(tree[r])
    {
        //hold old root value
        T* anchor = tree[r];

        //detach to make two subtrees that can be moved separately by setting root to NULL
        tree[r] = NULL;

        //move righ subtree down one to the right
        moveTreeDownRight(right(r), right(right(r)));
        tree[right(r)] = NULL;

        if(tree[right(left(r))])
        {
            int pivot = right(left(r));
            tree[r] = tree[pivot];
            tree[pivot] = NULL;

            moveTreeUpRight(right(pivot), left(right(r)));
            moveTreeUpRight(left(pivot), pivot);
        }
        else
        {
            //attach any subtrees to the left of the new root to the right of the old root
            //set that subtree's old root to NULL so it is not moved with movetree(right(r), r);
            moveTreeUpRight(right(left(r)), left(right(r)));
            tree[right(left(r))] = NULL;

            //move right subtree and add old root value as left child
            moveTreeUpRight(left(r), r);
        }

        tree[right(r)] = anchor;
    }
}

template<typename T>
void binaryTree<T>::rotateLeft(int r)
{
    if(tree[r])
    {
        //hold old root value
        T* anchor = tree[r];

        //detach to make two subtrees that can be moved separately by setting root to NULL
        tree[r] = NULL;

        //move left subtree down one to the left
        moveTreeDownLeft(left(r), left(left(r)));
        tree[left(r)] = NULL;

        if(tree[left(right(r))])
        {
            int pivot = left(right(r));
            tree[r] = tree[pivot];
            tree[pivot] = NULL;

            moveTreeUpLeft(left(pivot), right(left(r)));
            moveTreeUpLeft(right(pivot), pivot);
        }
        else
        {
            //attach any subtrees to the left of the new root to the right of the old root
            //set that subtree's old root to NULL so it is not moved with movetree(right(r), r);
            moveTreeUpLeft(left(right(r)), right(left(r)));
            tree[left(right(r))] = NULL;


            //move right subtree and add old root value as left child
            moveTreeUpLeft(right(r), r);
        }
        tree[left(r)] = anchor;
    }
}

template<typename T>
void binaryTree<T>::moveTreeDownRight(int source, int dest)
{
    if(tree[source])
    {
        if(tree[right(source)])
            moveTreeDownRight(right(source), right(dest));
        if(tree[left(source)])
            moveTreeDownRight(left(source), left(dest));
        tree[dest] = tree[source];
        tree[source] = NULL;
    }
}

template<typename T>
void binaryTree<T>::moveTreeUpRight(int source, int dest)
{
    if(tree[source])
    {
        tree[dest] = tree[source];
        tree[source] = NULL;
        if(tree[right(source)])
            moveTreeUpRight(right(source), right(dest));
        if(tree[left(source)])
            moveTreeUpRight(left(source), left(dest));
    }
}

template<typename T>
void binaryTree<T>::moveTreeDownLeft(int source, int dest)
{
    if(tree[source])
    {
        if(tree[left(source)])
            moveTreeDownLeft(left(source), left(dest));
        if(tree[right(source)])
            moveTreeDownLeft(right(source), right(dest));
        tree[dest] = tree[source];
        tree[source] = NULL;
    }
}

template<typename T>
void binaryTree<T>::moveTreeUpLeft(int source, int dest)
{
    if(tree[source])
    {
        tree[dest] = tree[source];
        tree[source] = NULL;
        if(tree[left(source)])
            moveTreeUpLeft(left(source), left(dest));
        if(tree[right(source)])
            moveTreeUpLeft(right(source), right(dest));
    }
}

template<typename T>
int binaryTree<T>::findLeftMost(int r)
{
    if(!tree[left(r)])
        return r;
    return findLeftMost(left(r));
}

template<typename T>
int binaryTree<T>::findRightMost(int r)
{
    if(!tree[right(r)])
        return r;
    return findRightMost(right(r));
}

template<typename T>
void binaryTree<T>::nukem()
{
    tree.clear();
    size = 0;
    cap = 0;
}

template<typename T>
void binaryTree<T>::copy(const binaryTree &other)
{
    int theSize;
    if(theSize = other.tree.capacity())
        tree.reserve(theSize);
    for(int i = 0; i < theSize; i++)
    {
        if(other.tree[i])
            tree[i] = new T(*other.tree[i]);
        else
            tree[i] = NULL;

    }
}

template<typename T>
int binaryTree<T>::right(int index)
{
    return 2 * index + 2;
}

template<typename T>
int binaryTree<T>::left(int index)
{
    return 2 * index + 1;
}

template<typename T>
int binaryTree<T>::parent(int index)
{
    //returns -1 if 'index' is root (index 0)
    if(index%2 == 0)
        return (index-2)/2;
    return (index-1)/2;
}

template<typename T>
int binaryTree<T>::log2floor(int arg)
{
    return (int)log2(arg);
}
template<typename T>
int binaryTree<T>::Cap()
{
    return tree.capacity();
}

template<typename U>
ostream& operator<<(ostream& out, binaryTree<U> &n)
{
    if(n.size != 0)
        n.display(0, out);
    return out;
}

template<typename U>
istream& operator>>(istream& in, binaryTree<U> &n)
{
    n.nukem();
    int capacity;
    string line;
    U next;
    stringstream ss;
    getline(in, line);
    ss<<line.substr(line.find(':') + 1);
    ss>>capacity;
    cout<<endl<<"capacity: "<<capacity;
    n.tree.reserve(capacity);
    while(in>>next)
        n.insert(next, 0);
    return in;
}

#endif
