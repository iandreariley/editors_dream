#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <string>
#include "summary.h"
#include "Index.h"

using namespace std;

const char WORDCHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'- ";
const char ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char PUNC[] = ",!@#$%^&*()_-+={}]\[]\"<>?'./";
const char PUNCSPACE[] = " !@#$%^&*()_-+={}]\[]\"'<>?,./";
const string CONTRACTIONS[] =
{"i'm", "i'll", "i'd", "i've",
    "you're", "you'll", "you'd", "you've",
    "he's", "he'll", "he'd",
    "she's", "she'll", "she'd",
    "it's", "'tis", "it'll", "it'd",
    "we're", "we'll", "we'd", "we've",
    "they're", "they'll", "they'd", "they've",
    "that's", "that'll", "that'd",
    "who's", "who'll", "who'd",
    "what's", "what're", "what'll", "what'd",
    "where's", "where'll", "where'd",
    "when's", "when'll", "when'd",
    "why's", "why'll", "why'd",
    "how's", "how'll", "how'd",
    "isn't", "aren't", "wasn't", "weren't",
    "haven't", "hasn't", "hadn't", "won't",
    "wouldn't", "don't", "doesn't", "didn't",
    "can't", "couldn't", "shouldn't", "mightn't",
    "mustn't", "would've", "should've", "couldn've",
    "might've", "must've", "o'", "o'clock",
    "ma'am", "ne'er-do-well", "cat-o'-nine-tails",
    "jack-o'-lantern", "will-o'-the-wisp", "twas"};

bool again();
float readability(double syl, int words, int sent);
bool isTerminus(string &line, int pos);
void cleanWord(string &word);
void easyParse(string &line, Index &words, double &sylc, int &wc, int &sc, int pc);
void getString(string &thestring);
void clearBuf();
void save(Index &words, summary &sums, int* letters, string topTen[], int tencount[]);
void sumOut(ostream& outs, summary& sums, int* letters, string topTen[], int tencount[]);

int main (int argc, char * const argv[])
{
    node<string> theword;
    string line, otherline, filename;
    int wordcount = 0, sencount = 0, parcount = 0;
    double sylcount = 0;
    char ans;
    stringstream ss;

    do
    {
        int* letters;
        string theten[10];
        int tencount[10];
        ifstream text;
        Index words;
        clearBuf();
        cout<<": ";
        getString(filename);
        text.open(filename.c_str());
        if(text.fail())
        {
            cout<<endl<<"file opening failed";
            exit(1);
        }

        clock_t end, start = clock();
        while(getline(text, line))
        {
            ++parcount;
            easyParse(line, words, sylcount, wordcount, sencount, parcount);
        }
        end = clock();
        double time1 = (end - start)/(1.*CLOCKS_PER_SEC);
        summary sum(sylcount, wordcount, sencount, parcount,
                    readability(sylcount, wordcount, sencount), time1);
        words.topTen(theten, tencount);
        letters = words.letterCount();
        sumOut(cout, sum, letters, theten, tencount);
        cout<<endl<<"would you like to see the words in order?";
        cin>>ans;
        if(ans == 'y')
        {
            ss<<words;
            while(getline(ss, line))
                cout<<endl<<line;
        }

        save(words, sum, letters, theten, tencount);
        cout<<endl<<"again?";
        cin>>ans;
    }while(ans != 'n');
}

void easyParse(string &line, Index &words, double &sylc, int &wc, int &sc, int pc)
{
    node<string>* theword;
    stringstream ss;
    string first, rest;
    int pos;
    ss<<line;
    while(ss>>line)
    {
        while(line != "")
        {
            rest = "";  //set rest to NULL string for while-loop condition
            pos = line.find_first_not_of(WORDCHARS);
            if(pos != line.npos)  //if punctuation is found
            {
                rest = line.substr(pos + 1);
                if(isTerminus(line, pos))
                    ++sc;
            }
            if(pos)  //if the punctuation isn't in the first space
            {
                line = line.substr(0, pos);
                cleanWord(line);
                sylc += line.size() / 3.0;
                ++wc;
                if(line != "")
                {
                    line[0] = toupper(line[0]);
                    theword = new node<string>(line, pc, sc);
                    words.insert(*theword);
                    delete theword;
                }
            }
            line = rest;
        }
    }
}

void cleanWord(string &word)
{
    int pos;
    string first = word.substr(0, 1);
    //while the first character is not a letter:
    do
    {
//		cout<<"first"<<"["<<first<<"]"<<endl;
        //while there's a hyphen, and it is either in the first or last position of the string
        while(((pos = word.find('-')) != word.npos) && ((!pos) || (pos == word.size() - 1)))
            word.erase(pos, 1);
        word[0] = tolower(word[0]);
        if((pos = word.find("'")) != word.npos)  //handle apostrophe
        {
            if(!pos)  //if the apostrophe is the first char
                word.erase(pos, 1);
            else
            {
                word[0]=tolower(word[0]);
                bool found = false;
                for(int i = 0; !found && (i < 78); i++)
                    if(CONTRACTIONS[i] == word)
                        found = true;
                if(!found)
                    word.erase(pos);
            }
        }
        first = word.substr(0, 1);
    }while(first != "" && first.find_first_of(ALPHA) == first.npos);
}

void clearBuf()
{
    char junk;
    while(junk != '\n')
        cin.get(junk);
}

float readability(double syl, int words, int sent)
{
    return 206.835 - 1.015 * words / sent - 84.6 * syl / words;
}

inline void getString(string &thestring)
{
    getline(cin, thestring);
}

inline bool isTerminus(string &line, int pos)
{
    return (line[pos] == '.' || line[pos] == '?' || line[pos] == '!');
}

void save(Index &words, summary &sums, int* letters, string topTen[], int tencount[])
{
    ofstream fout;
    string filename;
    char answer;
    cout<<"Would you like to save any of this information to a file?"<<endl;
    cout<<"[S]ummary page     word [L]ist     [A]ll    [N]one"<<endl<<":";
    cin>>answer;
    answer = tolower(answer);
    while(answer != 's' && answer != 'l' && answer != 'a' && answer != 'n')
    {
        cout<<endl<<"Invalid entry"<<endl<<endl;
        cout<<"Would you like to save any of this information to a file?"<<endl;
        cout<<"[S]ummary page     word [L]ist     [A]ll    [N]one"<<endl<<":";
        cin>>answer;
        answer = tolower(answer);
    }
    switch(answer)
    {
        case 's':
        {
            cout<<endl<<"Enter a filename"<<endl<<":";
            cin>>filename;
            fout.open(filename.c_str());
            if(fout.fail())
            {
                cout<<endl<<"FAILURE";
                break;
            }
            sumOut(fout, sums, letters, topTen, tencount);
            fout.close();
        }
            break;
        case 'l':
        {
            cout<<endl<<"Enter a filename"<<endl<<":";
            cin>>filename;
            fout.open(filename.c_str());
            if(fout.fail())
            {
                cout<<endl<<"FAILURE";
                break;
            }
            fout<<words;
            fout.close();
        }
            break;
        case 'a':
        {
            cout<<endl<<"Enter a filename"<<endl<<":";
            cin>>filename;
            fout.open(filename.c_str());
            if(fout.fail())
            {
                cout<<endl<<"FAILURE";
                break;
            }
            sumOut(fout, sums, letters, topTen, tencount);
            fout<<words;
            fout.close();
        }
            break;
        case 'n':
            break;
    }

}

void sumOut(ostream& outs, summary& sums, int* letters, string topTen[], int tencount[])
{
    char c;
    outs<<sums;
    outs<<endl<<endl;
    for(int i = 0; i < 10; i++)
    {
        outs<<setw(3)<<i+1<<". "<<setw(7)<<topTen[i]<<" : "<<tencount[i]<<endl;;
    }
    for(int i = 0; i < 26; i++)
    {
        c = i + 65;
        outs<<"Words beginning with '"<<c<<"': "<<letters[i]<<endl;
    }

}

bool again()
{
    string ans;
    cout<<endl<<endl<<"Would you like to do that again?";
    getline(cin, ans);
    return towlower(ans[0]) == 'y';
}
