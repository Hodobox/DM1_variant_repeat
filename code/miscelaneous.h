#ifndef MISCELANEOUS_H
#define MISCELANEOUS_H

#include "garbage_cut.h"
#include <iostream>
#include <set>
#include <vector>
#include <fstream>

void count_prefixes(vector<string> candidate_templates);

void rawdata_to_garbagefree();

// assumes temp is a contatenation of strings in pattern, e.g. pattern = ["A","B","C"] then temp is (A)n (B)m (C)l for some n,m,l
vector<int> template_pattern_parameters(vector<string> pattern, string &temp);

#endif