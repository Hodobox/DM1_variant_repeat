#include "misc.h"

string LSR(string s)
{
    string best = s;
    for(int i=0;i+1<s.size();++i)
    {
        s.push_back(s[0]);
        s = s.substr(1);
        best = min(best,s);
    }
    return best;
}