/*
 *  summary.h
 *  EDTree
 *
 *  Created by Ian Riley on 11/3/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SUMMARY_H
#define SUMMARY_H

#include <iostream>
#include <sstream>
#include "node.h"

using namespace std;

struct summary
{
    int syllables, words, sentences, paragraphs;
    double readable, seconds;

    summary();
    summary(int syl, int w, int sent, int pa, double read, double sec);
    ~summary();
    summary(const summary &other);

    summary& operator=(const summary &other);

    friend ostream& operator<<(ostream &outs, const summary &sum);
};

summary::summary()
{
    syllables = words = sentences = paragraphs = readable = seconds = 0;
}
summary::summary(int syl, int w, int sent, int pa, double read, double sec)
{
    syllables = syl;
    words = w;
    sentences = sent;
    paragraphs = pa;
    readable = read;
    seconds = sec;
}
summary::~summary()
{}

summary::summary(const summary &other)
{
    syllables = other.syllables;
    words = other.words;
    sentences = other.sentences;
    paragraphs = other.paragraphs;
    readable = other.readable;
    seconds = other.seconds;
}

summary& summary::operator=(const summary &other)
{
    if(this != &other)
    {
        syllables = other.syllables;
        words = other.words;
        sentences = other.sentences;
        paragraphs = other.paragraphs;
        readable = other.readable;
        seconds = other.seconds;
    }
    return *this;
}

ostream& operator<<(ostream &outs, const summary &sum)
{
    outs<<"syllables: "<<sum.syllables<<endl<<"words: "<<sum.words<<endl;
    outs<<"sentences: "<<sum.sentences<<endl<<"paragraphs: "<<sum.paragraphs;
    outs<<"readability (Flesch-Kincaid): "<<sum.readable<<endl;
    outs<<"time to push: "<<sum.seconds;
    outs<<endl<<"Ten most frequent words:";
}

#endif
