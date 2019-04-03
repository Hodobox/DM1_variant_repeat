#ifndef WINDOW
#define WINDOW

#include "misc.h"
#include <map>
#include <vector>
using namespace std;
vector< map<string, vector<pair<int,int> > > > window(vector<string> &sequences, int window_size, int repeat_size);


#endif // window.h