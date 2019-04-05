#ifndef MISC
#define MISC

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

// Lexicographically Smallest Rotation
string LSR(string s);
vector<string> readfile(ifstream &input);
void outputdataset(ofstream &output, vector<map<string,vector<pair<int,int> > > >data, int repeat_size, int window_size);
void outputdata(ofstream &output,vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > >data, string msg);
vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > readdata(ifstream &input);
void filter_window_data(ifstream &input, ofstream &output);
void print_interesting_repeats(vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > &data);

#endif // misc.h