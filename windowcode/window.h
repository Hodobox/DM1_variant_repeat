#ifndef WINDOW
#define WINDOW

#include "misc.h"
#include <iostream>
#include <set>
#include <map>
#include <vector>

using namespace std;
pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > >  window(vector<string> &sequences, int window_size, int repeat_size);
vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > >  window_multiple(vector<string> &sequences, pair<int,int> repeat_sizes, int min_window_size, double window_size_scale = 6);

#endif // window.h