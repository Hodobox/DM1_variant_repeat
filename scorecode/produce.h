#ifndef PRODUCE_H
#define PRODUCE_H

#include <vector>
#include <string>
#include <set>
#include <cassert>
#include <iostream>
using namespace std;

//void produce_sequences_improved(vector<string> &pattern, int max_length, int outer_index,int inner_index,string &partial_result, vector<string> &result);
vector<string> produce_sequences_improved_outer(vector<string> pattern,int cutoff_length);
string produce_specific(vector<string> pattern, vector<int> howmany, int length = -1);

#endif