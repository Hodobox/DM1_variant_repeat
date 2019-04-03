#ifndef GARBAGE_H
#define GARBAGE_H

#include "align.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
std::string cut_garbage(std::string &sequence,std::string pattern, int n,int match_treshold);

#endif