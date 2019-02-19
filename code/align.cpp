#include "align.h"
int CALLS_DELETE_TESTVAR;

const int UNCACHED = -(1 << 15);
// should be lower than maximum sequence length * 2
const int TERRIBLE_SCORE = -(1 << 12); 
// match_score should be the only positive score, everything else should be negative
const int
deletion_score = -1,
gap_score = -1,
mismatch_score = -1,
match_score = +1;

// should have identical outputs to pyth_orig/main.py (original python implementation)
int align_original(string &temp,string &sequence, vector<vector<int> > &cache)
{
    //calls++;

    if(temp.empty())
        return sequence.size() * deletion_score;
    if(sequence.empty())
        return temp.size() * gap_score;

    if(cache[temp.size()][sequence.size()] != UNCACHED)
        return cache[temp.size()][sequence.size()];

    string temp_tail = temp.substr(temp.size()-1,1); //temp_tail = temp.substr(1);
    string seq_tail = sequence.substr(sequence.size()-1,1);//, seq_tail = sequence.substr(1);
    // in C++ swapped functioning of tail and head; we match from the right
    // as then we can pop characters form the right side in O(1) and pass the heads by reference
    // which should save us one order of time

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    temp.pop_back();
    sequence.pop_back();
    score_seq = align_original(temp, sequence, cache);
    int match_result = score_seq + match_mismatch_score;
    temp.push_back(temp_tail[0]);
    sequence.push_back(seq_tail[0]);

    sequence.pop_back();
    score_seq = align_original(temp, sequence, cache);
    sequence.push_back(seq_tail[0]);
    int delete_result = score_seq + deletion_score;
    temp.pop_back();
    score_seq = align_original(temp, sequence, cache);
    temp.push_back(temp_tail[0]);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[temp.size()][sequence.size()] = result;
    return result;
}

int align_greedymatch_multithread(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache)
{
    // if one sequence is empty, we will have no choice but to gap/delete the rest
    if(!tempsize)
        return seqsize * deletion_score;
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize] != UNCACHED)
        return cache[tempsize][seqsize];


    CALLS_DELETE_TESTVAR++;
    

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    score_seq = align_greedymatch_multithread(temp, sequence, tempsize-1,seqsize-1, cache);
    int match_result = score_seq + match_mismatch_score;

    if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }
    
    score_seq = align_greedymatch_multithread(temp, sequence, tempsize, seqsize-1, cache);
    int delete_result = score_seq + deletion_score;

    score_seq = align_greedymatch_multithread(temp, sequence, tempsize-1, seqsize, cache);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
    return result;
}

int align_gm_mt_opt(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> >  &prefixes, int prefixlen)
{
    // if one sequence is empty, we will have no choice but to gap/delete the rest
    if(!tempsize)
        return prefixes[prefixlen][seqsize];
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize] != UNCACHED)
        return cache[tempsize][seqsize];


    //CALLS_DELETE_TESTVAR++;
    

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    score_seq = align_gm_mt_opt(temp, sequence, tempsize-1,seqsize-1, cache, prefixes, prefixlen);
    int match_result = score_seq + match_mismatch_score;

    if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }
    
    score_seq = align_gm_mt_opt(temp, sequence, tempsize, seqsize-1, cache, prefixes, prefixlen);
    int delete_result = score_seq + deletion_score;

    score_seq = align_gm_mt_opt(temp, sequence, tempsize-1, seqsize, cache, prefixes, prefixlen);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
    return result;
}
void align_gm_mt_opt_outer(vector<string> pattern,int index, string &sequence,vector<string> &templates,int alignments_to_do,vector<vector<int> > &pair_scores)
{
    //precompute prefixes

    auto start = chrono::steady_clock::now();

    vector<vector<int> > prefixes(sequence.size()+1,vector<int>(sequence.size()+1,UNCACHED));
    string prefix = "";
    while(prefix.size() < sequence.size())
        prefix += pattern[0];

    prefix = prefix.substr(0,sequence.size());

    for(int prefixlen = 0; prefixlen <= sequence.size(); ++prefixlen)
    {
        //cerr << prefixlen << "/" << sequence.size() << " " << CALLS_DELETE_TESTVAR << "\n";
        vector<vector<int> > cache(prefixlen+1,vector<int>(sequence.size()+1,UNCACHED));
        for(int seqlen = 0; seqlen <= sequence.size(); ++seqlen)
        {   
            // TODO replace with bottom-up DP
            prefixes[prefixlen][seqlen] = align_greedymatch_multithread(prefix, sequence, prefixlen, seqlen, cache);
            //cerr << prefix.substr(0,prefixlen) << " v " << sequence.substr(0,seqlen) << " = " << prefixes[prefixlen][seqlen] << "\n";
        }
    }

    //auto end = chrono::steady_clock::now();

    //cerr << "prefix calc: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " milli sec\n";
   
   //start = chrono::steady_clock::now();

   /* for(int i=0;i<=sequence.size();++i)
    {
        for(int k=0;k<=sequence.size();++k)
        {
            cerr << prefixes[i][k] << " ";
        }
        cerr << "\n";
    }*/

    for(int k=0;k<alignments_to_do;++k)
    {
        vector<int> params = template_pattern_parameters(pattern, templates[k]);
        int prefixlen = pattern[0].size() * params[0];
        vector<int> sufparams = params;
        sufparams[0] = 0;

        if(k%1000==0)
        {
            cerr << k << "/" << alignments_to_do << "\n";
        }
        //cerr << params[0] << " " << params[1] << " " << params[2] << " " << prefixlen << "\n";
        string suffix = produce_specific(pattern,sufparams, templates[k].size() - prefixlen);
        vector<vector<int> > cache(suffix.size()+1,vector<int>(sequence.size()+1,UNCACHED));
        int score = align_gm_mt_opt(suffix,sequence, suffix.size(), sequence.size(), cache, prefixes, prefixlen);
        vector<int> pairscore_element = params;
        pairscore_element.push_back(index);
        pairscore_element.push_back(score);
        #pragma omp critical
        pair_scores.push_back(pairscore_element);
    }   

    auto end = chrono::steady_clock::now();

    cerr << "Average time per alignment: " << chrono::duration_cast<chrono::microseconds>(end - start).count() / (alignments_to_do) << " micro sec\n";

}

int align_GAL_multithread(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, int get_at_least)
{
   //calls++;

    // if one sequence is empty, we will have no choice but to gap/delete the rest
    if(!tempsize)
        return seqsize * deletion_score;
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize] != UNCACHED)
        return cache[tempsize][seqsize];

    // for the result of this recursive call to matter (improve the result for the caller)
    // it would have to get at least get_at_least points
    // in the best case, we match the entirety of the smaller string, and then gap/delete the leftover of the other
    int best_score_possible = match_score * min(tempsize,seqsize) + max(deletion_score,gap_score) * abs(tempsize-seqsize);
    if(best_score_possible < get_at_least)
        return TERRIBLE_SCORE;

    //CALLS_DELETE_TESTVAR++;
    

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    score_seq = align_GAL_multithread(temp, sequence, tempsize-1,seqsize-1, cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    
    // this breaks here - why?
    /*if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }*/

    get_at_least = max(get_at_least, match_result);

    score_seq = align_GAL_multithread(temp, sequence, tempsize, seqsize-1, cache, get_at_least - deletion_score);
    int delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);

    score_seq = align_GAL_multithread(temp, sequence, tempsize-1, seqsize, cache,get_at_least - gap_score);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
    return result;
}
