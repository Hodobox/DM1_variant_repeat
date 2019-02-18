#include "produce.h"
vector<string> produce_sequences(vector<string> &pattern, int max_length, int outer_index,int inner_index,string &partial_result)
{
    if(partial_result.size() >= max_length)
        return {partial_result};

    string current_pattern = pattern[outer_index];
    vector<string> results = {};
    bool next_pattern_available = (outer_index+1 < pattern.size()) && (inner_index == 0);

    if(next_pattern_available)
    {
        vector<string> jump_right_now = produce_sequences(pattern,max_length,outer_index+1, 0,partial_result);
        for(string &p : jump_right_now)
            results.push_back(p);
    }

    int next_inner_index = (inner_index + 1) % current_pattern.size();
    char current_character = pattern[outer_index][inner_index];
    
    partial_result.push_back(current_character);
    vector<string> same_pattern_result = produce_sequences(pattern,max_length,outer_index,next_inner_index, partial_result);
    for(string p : same_pattern_result)
        results.push_back(p);
    partial_result.pop_back();

    return results;
}


vector<string> produce_sequences_outer(vector<string> pattern, int max_length)
{
    string tmp;
    vector<string> result = produce_sequences(pattern, max_length, 0, 0, tmp);
    set<string> unique;
    for(string &r : result)
        unique.insert(r);
    result.clear();
    for(auto r : unique)
        result.push_back(r);
    return result;
}

void produce_sequences_faster(vector<string> &pattern, int max_length, int outer_index,int inner_index,string &partial_result, vector<string> &result)
{
    if(partial_result.size() >= max_length)
    {
        result.push_back(partial_result);
        return;
    }

    string current_pattern = pattern[outer_index];
    bool next_pattern_available = (outer_index+1 < pattern.size()) && (inner_index == 0);

    if(next_pattern_available)
    {
        produce_sequences_faster(pattern,max_length,outer_index+1, 0, partial_result, result);
    }

    int next_inner_index = (inner_index + 1) % current_pattern.size();
    char current_character = pattern[outer_index][inner_index];
    
    partial_result.push_back(current_character);
    produce_sequences_faster(pattern,max_length,outer_index,next_inner_index, partial_result, result);
    partial_result.pop_back();
}


vector<string> produce_sequences_outer_faster(vector<string> pattern, int max_length)
{
    string tmp;
    vector<string> result;
    produce_sequences_faster(pattern, max_length, 0, 0, tmp, result);
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

    if(res.size() != length)
    {
        cerr << "warning: produced sequence different size than given length\n";
    }

    return res;
}
