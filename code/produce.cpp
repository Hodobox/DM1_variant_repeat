#include "produce.h"

void produce_sequences_improved(vector<string> &pattern,int length, int index, string &partial_result, vector<string> &result)
{
    if(partial_result.size() >= length)
    {
        result.push_back(partial_result);
        return;
    }

    if(index+1 < pattern.size())
        produce_sequences_improved(pattern,length,index+1,partial_result,result);

    int cur_length = partial_result.size();
    partial_result += pattern[index];
    produce_sequences_improved(pattern,length,index,partial_result,result);
    partial_result.resize(cur_length);
}

vector<string> produce_sequences_improved_outer(vector<string> pattern,int cutoff_length)
{
    string tmp;
    vector<string> result;
    produce_sequences_improved(pattern, cutoff_length, 0, tmp, result);
    set<string> unique;
    for(string &r : result)
        unique.insert(r);
    result.clear();
    for(auto r : unique)
        result.push_back(r);
    return result;
}

string produce_specific(vector<string> pattern, vector<int> howmany, int length)
{
    assert(pattern.size() == howmany.size());

    string res = "";
    for(int i=0;i<howmany.size();++i)
    {
        for(int k=0;k<howmany[i];++k)
            res += pattern[i];
    }

    if(length != -1 && res.size() != length)
    {
        cerr << "warning: produced sequence different size than given length\n";
    }

    return res;
}
