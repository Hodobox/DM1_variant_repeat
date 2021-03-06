#ifndef ALIGN_H
#define ALIGN_H

#include "produce.h"
#include "miscelaneous.h"
#include "garbage_cut.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
using namespace std;

extern long long CALLS_DELETE_TESTVAR;

extern const int UNCACHED;
// should be lower than maximum sequence length * 2
extern const int TERRIBLE_SCORE; 
// match_score should be the only positive score, everything else should be negative
extern const int
deletion_score,
gap_score,
mismatch_score,
match_score;

int align_greedymatch_multithread(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache);
//int align_gm_mt_opt(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> >  &prefixes, int prefixlen);
void align_gm_mt_opt_outer(vector<string> pattern,int index, string &sequence,vector<string> &templates,int alignments_to_do,vector<vector<int> > &pair_scores);
int align_GAL_multithread(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, int get_at_least = TERRIBLE_SCORE);
int align_original(string &temp,string &sequence, vector<vector<int> > &cache);
int align_GAL_gm(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, int get_at_least = TERRIBLE_SCORE);
int align_GAL_careful(string &temp, string &seq, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> > &GAL_cache, int get_at_least = TERRIBLE_SCORE);
int align_GAL_careful_gm(string &temp, string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> > &GAL_cache, int get_at_least = TERRIBLE_SCORE);
int align_GAL_gm_mt_opt(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> >  &prefixes, int prefixlen, int get_at_least = TERRIBLE_SCORE);
void align_GAL_gm_mt_opt_outer(vector<string> pattern,int index, string &sequence,vector<string> &templates,int alignments_to_do,vector<vector<int> > &pair_scores);
int align_GAL_careful_pair(string &temp, string &sequence, int tempsize, int seqsize, vector<vector<pair<int,int> > > &cache, int get_at_least = TERRIBLE_SCORE);

#endif