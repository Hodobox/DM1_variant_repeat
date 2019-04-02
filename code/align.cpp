#include "align.h"
long long CALLS_DELETE_TESTVAR;

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


    CALLS_DELETE_TESTVAR++;
    

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
    //precompute prefixes and suffixes

    auto start = chrono::steady_clock::now();

    string prefix = "";
    while(prefix.size() < sequence.size())
        prefix += pattern[0];
    vector<vector<int> > prefixes(prefix.size()+1,vector<int>(sequence.size()+1,UNCACHED));
    
    for(int prefixlen = 0; prefixlen <= prefix.size(); ++prefixlen)
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

    cerr << "pref done\n";

    string suffix = "";
    while(suffix.size() < sequence.size())
        suffix += pattern.back();
    vector<vector<int> > suffixes(suffix.size()+1,vector<int>(sequence.size()+1,UNCACHED));

    reverse(suffix.begin(),suffix.end());
    string reversed_sequence = sequence;
    reverse(reversed_sequence.begin(),reversed_sequence.end());
    vector<string> reversed_pattern = pattern;
    reverse(reversed_pattern.begin(),reversed_pattern.end());
    for(string &s:reversed_pattern)
        reverse(s.begin(),s.end());

    for(int suffixlen = 0; suffixlen <= suffix.size(); ++ suffixlen)
    {
        vector<vector<int> > cache(suffixlen+1,vector<int>(sequence.size()+1,UNCACHED));
        for(int seqlen = 0; seqlen <= sequence.size(); ++seqlen)
        {
            // TODO replace with bottom-up DP
            suffixes[suffixlen][seqlen] = align_greedymatch_multithread(suffix, reversed_sequence, suffixlen, seqlen, cache);
        }
    }

    cerr << "suff done\n";

    for(int k=0;k<alignments_to_do;++k)
    {

        //only for testing 1 seq, otherwise we'll get trashhed
        if(k%1000==0)
        {
            cerr << k << "/" << alignments_to_do << "\n";
        }

        vector<int> params = template_pattern_parameters(pattern, templates[k]);
        int prefixlen = pattern[0].size() * params[0];
        int suffixlen = pattern.back().size() * params.back();

        vector<int> pairscore_element = params;
        pairscore_element.push_back(index);
        int score;

        if(suffixlen > prefixlen)
        {

            vector<int> prefparams = params;
            prefparams.back() = 0;
            reverse(prefparams.begin(),prefparams.end());
            string tempprefix = produce_specific(reversed_pattern,prefparams,templates[k].size() - suffixlen);
            vector<vector<int> > cache(tempprefix.size()+1,vector<int>(reversed_sequence.size()+1, UNCACHED));
            score = align_gm_mt_opt(tempprefix,reversed_sequence,tempprefix.size(), reversed_sequence.size(), cache, suffixes, suffixlen);
        }
        else
        {
            vector<int> sufparams = params;
            sufparams[0] = 0;
            string tempsuffix = produce_specific(pattern,sufparams, templates[k].size() - prefixlen);
            vector<vector<int> > cache(tempsuffix.size()+1,vector<int>(sequence.size()+1,UNCACHED));
            score = align_gm_mt_opt(tempsuffix,sequence, tempsuffix.size(), sequence.size(), cache, prefixes, prefixlen);
        }

        pairscore_element.push_back(score);
        #pragma omp critical
        pair_scores.push_back(pairscore_element);
    }   

    auto end = chrono::steady_clock::now();

    cerr << "Average time per alignment: " << chrono::duration_cast<chrono::microseconds>(end - start).count() / (alignments_to_do) << " micro sec\n";

}

int align_GAL_multithread(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, int get_at_least)
{
    //cerr << tempsize << " " << seqsize << " GAL " << get_at_least << "\n";

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
    //cerr << "BSP " << best_score_possible << "\n";
    if(best_score_possible < get_at_least)
        return TERRIBLE_SCORE;

    CALLS_DELETE_TESTVAR++;
    

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize-1 << " wants " << get_at_least - match_mismatch_score << "\n";
    score_seq = align_GAL_multithread(temp, sequence, tempsize-1,seqsize-1, cache, get_at_least - match_mismatch_score);
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << "\n";
    int match_result = score_seq + match_mismatch_score;
    
    // this breaks here - why?
    /*if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }*/

    get_at_least = max(get_at_least, match_result);

    //cerr << tempsize << " " << seqsize << " tries " << tempsize << " " << seqsize-1 << " wants " << get_at_least - deletion_score << "\n";
   
    score_seq = align_GAL_multithread(temp, sequence, tempsize, seqsize-1, cache, get_at_least - deletion_score);
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << "\n";
    int delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);
    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize << " wants " << get_at_least - gap_score << "\n";
   
    score_seq = align_GAL_multithread(temp, sequence, tempsize-1, seqsize, cache,get_at_least - gap_score);
    int gap_result = score_seq + gap_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << "\n";

    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
    //cerr << tempsize << " " << seqsize << " = " << result << "\n";
    return result;
}

int align_GAL_gm(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, int get_at_least)
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

    score_seq = align_GAL_gm(temp, sequence, tempsize-1,seqsize-1, cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    
    // this breaks here - why?
    if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }

    get_at_least = max(get_at_least, match_result);

    score_seq = align_GAL_gm(temp, sequence, tempsize, seqsize-1, cache, get_at_least - deletion_score);
    int delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);

    score_seq = align_GAL_gm(temp, sequence, tempsize-1, seqsize, cache,get_at_least - gap_score);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
    return result;
}

int align_GAL_careful(string &temp, string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> > &GAL_cache, int get_at_least)
{

    //cerr << tempsize << " " << seqsize << " GAL " << get_at_least << " cached " <<  GAL_cache[tempsize][seqsize] << "\n";

    if(!tempsize)
        return seqsize * deletion_score;
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize] != UNCACHED && GAL_cache[tempsize][seqsize] <= get_at_least) 
        return cache[tempsize][seqsize];

    // for the result of this recursive call to matter (improve the result for the caller)
    // it would have to get at least get_at_least points
    // in the best case, we match the entirety of the smaller string, and then gap/delete the leftover of the other
    int best_score_possible = match_score * min(tempsize,seqsize) + max(deletion_score,gap_score) * abs(tempsize-seqsize);
    //cerr << "BSP " << best_score_possible << "\n";
    if(best_score_possible < get_at_least)
    {
        GAL_cache[tempsize][seqsize] = min(GAL_cache[tempsize][seqsize], get_at_least);
        return TERRIBLE_SCORE;
    }

    GAL_cache[tempsize][seqsize] = min(GAL_cache[tempsize][seqsize], get_at_least);

    CALLS_DELETE_TESTVAR++;

   // if(CALLS_DELETE_TESTVAR % 1000 == 0)
    //    cerr << CALLS_DELETE_TESTVAR << " calls\n";

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;
    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize-1 << " wants " << get_at_least - match_mismatch_score << "\n";
   
    score_seq = align_GAL_careful(temp, sequence, tempsize-1,seqsize-1, cache, GAL_cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << match_mismatch_score << ")\n";

    get_at_least = max(get_at_least, match_result);

    //cerr << tempsize << " " << seqsize << " tries " << tempsize << " " << seqsize-1 << " wants " << get_at_least - deletion_score << "\n";
    score_seq = align_GAL_careful(temp, sequence, tempsize, seqsize-1, cache, GAL_cache, get_at_least - deletion_score);
    int delete_result = score_seq + deletion_score;
    get_at_least = max(get_at_least, delete_result);
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << deletion_score << ")\n";


    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize << " wants " << get_at_least - gap_score << "\n";
   
    score_seq = align_GAL_careful(temp, sequence, tempsize-1, seqsize, cache,GAL_cache, get_at_least - gap_score);
    int gap_result = score_seq + gap_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << gap_score << ")\n";


    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
   // cerr << tempsize << " " << seqsize << " " <<  get_at_least << " = " << result << "\n";
    
    return result;
}

int align_GAL_careful_gm(string &temp, string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> > &GAL_cache, int get_at_least)
{

    //cerr << tempsize << " " << seqsize << " GAL " << get_at_least << " cached " <<  GAL_cache[tempsize][seqsize] << "\n";

    if(!tempsize)
        return seqsize * deletion_score;
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize] != UNCACHED && GAL_cache[tempsize][seqsize] <= get_at_least) 
        return cache[tempsize][seqsize];

    // for the result of this recursive call to matter (improve the result for the caller)
    // it would have to get at least get_at_least points
    // in the best case, we match the entirety of the smaller string, and then gap/delete the leftover of the other
    int best_score_possible = match_score * min(tempsize,seqsize) + max(deletion_score,gap_score) * abs(tempsize-seqsize);
    //cerr << "BSP " << best_score_possible << "\n";
    if(best_score_possible < get_at_least)
    {
        GAL_cache[tempsize][seqsize] = min(GAL_cache[tempsize][seqsize], get_at_least);
        return TERRIBLE_SCORE;
    }

    GAL_cache[tempsize][seqsize] = min(GAL_cache[tempsize][seqsize], get_at_least);

    CALLS_DELETE_TESTVAR++;

   // if(CALLS_DELETE_TESTVAR % 1000 == 0)
    //    cerr << CALLS_DELETE_TESTVAR << " calls\n";

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;
    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize-1 << " wants " << get_at_least - match_mismatch_score << "\n";
   
    score_seq = align_GAL_careful_gm(temp, sequence, tempsize-1,seqsize-1, cache, GAL_cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << match_mismatch_score << ")\n";

    if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }

    get_at_least = max(get_at_least, match_result);

    //cerr << tempsize << " " << seqsize << " tries " << tempsize << " " << seqsize-1 << " wants " << get_at_least - deletion_score << "\n";
    score_seq = align_GAL_careful_gm(temp, sequence, tempsize, seqsize-1, cache, GAL_cache, get_at_least - deletion_score);
    int delete_result = score_seq + deletion_score;
    get_at_least = max(get_at_least, delete_result);
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << deletion_score << ")\n";


    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize << " wants " << get_at_least - gap_score << "\n";
   
    score_seq = align_GAL_careful_gm(temp, sequence, tempsize-1, seqsize, cache,GAL_cache, get_at_least - gap_score);
    int gap_result = score_seq + gap_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << gap_score << ")\n";


    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
   // cerr << tempsize << " " << seqsize << " " <<  get_at_least << " = " << result << "\n";
    
    return result;
}


int align_GAL_gm_mt_opt(string &temp,string &sequence, int tempsize, int seqsize, vector<vector<int> > &cache, vector<vector<int> >  &prefixes, int prefixlen, int get_at_least)
{
    // if one sequence is empty, we will have no choice but to gap/delete the rest
    if(!tempsize)
        return prefixes[prefixlen][seqsize];
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize] != UNCACHED)
        return cache[tempsize][seqsize];


    int best_score_possible = match_score * min(tempsize,seqsize) + max(deletion_score,gap_score) * abs(tempsize-seqsize);
    if(best_score_possible < get_at_least)
        return TERRIBLE_SCORE;

    CALLS_DELETE_TESTVAR++;
    

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    score_seq = align_GAL_gm_mt_opt(temp, sequence, tempsize-1,seqsize-1, cache, prefixes, prefixlen, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    
    if(match_mismatch_score == match_score)
    {
        cache[tempsize][seqsize] = match_result;
        return match_result;
    }

    get_at_least = max(get_at_least, match_result);

    score_seq = align_GAL_gm_mt_opt(temp, sequence, tempsize, seqsize-1, cache, prefixes, prefixlen, get_at_least - deletion_score);
    int delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);

    score_seq = align_GAL_gm_mt_opt(temp, sequence, tempsize-1, seqsize, cache, prefixes, prefixlen, get_at_least - gap_score);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize] = result;
    return result;
}
void align_GAL_gm_mt_opt_outer(vector<string> pattern,int index, string &sequence,vector<string> &templates,int alignments_to_do,vector<vector<int> > &pair_scores)
{
    //precompute prefixes and suffixes

    auto start = chrono::steady_clock::now();

    string prefix = "";
    while(prefix.size() < sequence.size())
        prefix += pattern[0];
    vector<vector<int> > prefixes(prefix.size()+1,vector<int>(sequence.size()+1,UNCACHED));
    
    for(int prefixlen = 0; prefixlen <= prefix.size(); ++prefixlen)
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

    cerr << "pref done\n";

    string suffix = "";
    while(suffix.size() < sequence.size())
        suffix += pattern.back();
    vector<vector<int> > suffixes(suffix.size()+1,vector<int>(sequence.size()+1,UNCACHED));

    reverse(suffix.begin(),suffix.end());
    string reversed_sequence = sequence;
    reverse(reversed_sequence.begin(),reversed_sequence.end());
    vector<string> reversed_pattern = pattern;
    reverse(reversed_pattern.begin(),reversed_pattern.end());
    for(string &s:reversed_pattern)
        reverse(s.begin(),s.end());

    for(int suffixlen = 0; suffixlen <= suffix.size(); ++ suffixlen)
    {
        vector<vector<int> > cache(suffixlen+1,vector<int>(sequence.size()+1,UNCACHED));
        for(int seqlen = 0; seqlen <= sequence.size(); ++seqlen)
        {
            // TODO replace with bottom-up DP
            suffixes[suffixlen][seqlen] = align_greedymatch_multithread(suffix, reversed_sequence, suffixlen, seqlen, cache);
        }
    }

    cerr << "suff done\n";

    for(int k=0;k<alignments_to_do;++k)
    {

        //only for testing 1 seq, otherwise we'll get trashhed
        if(k%1000==0)
        {
            cerr << k << "/" << alignments_to_do << "\n";
        }

        vector<int> params = template_pattern_parameters(pattern, templates[k]);
        int prefixlen = pattern[0].size() * params[0];
        int suffixlen = pattern.back().size() * params.back();

        vector<int> pairscore_element = params;
        pairscore_element.push_back(index);
        int score;

        if(suffixlen > prefixlen)
        {

            vector<int> prefparams = params;
            prefparams.back() = 0;
            reverse(prefparams.begin(),prefparams.end());
            string tempprefix = produce_specific(reversed_pattern,prefparams,templates[k].size() - suffixlen);
            vector<vector<int> > cache(tempprefix.size()+1,vector<int>(reversed_sequence.size()+1, UNCACHED));
            score = align_GAL_gm_mt_opt(tempprefix,reversed_sequence,tempprefix.size(), reversed_sequence.size(), cache, suffixes, suffixlen);
        }
        else
        {
            vector<int> sufparams = params;
            sufparams[0] = 0;
            string tempsuffix = produce_specific(pattern,sufparams, templates[k].size() - prefixlen);
            vector<vector<int> > cache(tempsuffix.size()+1,vector<int>(sequence.size()+1,UNCACHED));
            score = align_GAL_gm_mt_opt(tempsuffix,sequence, tempsuffix.size(), sequence.size(), cache, prefixes, prefixlen);
        }

        pairscore_element.push_back(score);
        #pragma omp critical
        pair_scores.push_back(pairscore_element);
    }   

    auto end = chrono::steady_clock::now();

    cerr << "Average time per alignment: " << chrono::duration_cast<chrono::microseconds>(end - start).count() / (alignments_to_do) << " micro sec\n";

}

int align_GAL_careful_pair(string &temp, string &sequence, int tempsize, int seqsize, vector<vector<pair<int,int> > > &cache, int get_at_least)
{

    //cerr << tempsize << " " << seqsize << " GAL " << get_at_least << " cached " <<  GAL_cache[tempsize][seqsize] << "\n";

    if(!tempsize)
        return seqsize * deletion_score;
    if(!seqsize)
        return tempsize * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[tempsize][seqsize].first != UNCACHED && cache[tempsize][seqsize].second <= get_at_least) 
        return cache[tempsize][seqsize].first;

    // for the result of this recursive call to matter (improve the result for the caller)
    // it would have to get at least get_at_least points
    // in the best case, we match the entirety of the smaller string, and then gap/delete the leftover of the other
    int best_score_possible = match_score * min(tempsize,seqsize) + max(deletion_score,gap_score) * abs(tempsize-seqsize);
    //cerr << "BSP " << best_score_possible << "\n";
    if(best_score_possible < get_at_least)
    {
        cache[tempsize][seqsize].second = min(cache[tempsize][seqsize].second, get_at_least);
        return TERRIBLE_SCORE;
    }

    cache[tempsize][seqsize].second = min(cache[tempsize][seqsize].second, get_at_least);

    CALLS_DELETE_TESTVAR++;

   // if(CALLS_DELETE_TESTVAR % 1000 == 0)
    //    cerr << CALLS_DELETE_TESTVAR << " calls\n";

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;
    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize-1 << " wants " << get_at_least - match_mismatch_score << "\n";
   
    score_seq = align_GAL_careful_pair(temp, sequence, tempsize-1,seqsize-1, cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << match_mismatch_score << ")\n";

    get_at_least = max(get_at_least, match_result);

    //cerr << tempsize << " " << seqsize << " tries " << tempsize << " " << seqsize-1 << " wants " << get_at_least - deletion_score << "\n";
    score_seq = align_GAL_careful_pair(temp, sequence, tempsize, seqsize-1, cache, get_at_least - deletion_score);
    int delete_result = score_seq + deletion_score;
    get_at_least = max(get_at_least, delete_result);
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << deletion_score << ")\n";


    //cerr << tempsize << " " << seqsize << " tries " << tempsize-1 << " " << seqsize << " wants " << get_at_least - gap_score << "\n";
   
    score_seq = align_GAL_careful_pair(temp, sequence, tempsize-1, seqsize, cache, get_at_least - gap_score);
    int gap_result = score_seq + gap_score;
    //cerr << tempsize << " " << seqsize << " gets " << score_seq << " (+" << gap_score << ")\n";


    int result = max(gap_result,max(match_result,delete_result));
    cache[tempsize][seqsize].first = result;
   // cerr << tempsize << " " << seqsize << " " <<  get_at_least << " = " << result << "\n";
    
    return result;
}
