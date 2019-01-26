#include <map>
#include <vector>
#include <algorithm>
using namespace std;

const int UNCACHED = -1000000;
// should be lower than maximum sequence length * 2
const int TERRIBLE_SCORE = -(1 << 16); 
const int
deletion_score = -1,
gap_score = -1,
mismatch_score = -1,
match_score = +1;

long long calls;

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

// original align with optimization - if current character matches, return match result
// without even trying deleteion or gap
// reasoning:
// let the strings be Tc Sc (some prefixes T,S and ending with the same character c)
// can a higher alignment score be achieved by choosing a gap or delete in this step?
// if we choose some number of gaps, followed by a deletion (or some number of deletions followed by a gap)
// then whatever alignment score is reached thanks to that, we could improve by appending the match from this step to it
// if we choose some number of gaps/deletions followed by a match, we can just as well match the current character  
// and gap/delete that one instead
// hence we (should) never get a higher alignment score through a delete or a gap.
int align_original_greedy_match(string &temp,string &sequence, vector<vector<int> > &cache)
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
    score_seq = align_original_greedy_match(temp, sequence, cache);
    int match_result = score_seq + match_mismatch_score;
    temp.push_back(temp_tail[0]);
    sequence.push_back(seq_tail[0]);

    if(match_mismatch_score == match_score)
    {
        cache[temp.size()][sequence.size()] = match_result;
        return match_result;
    }

    sequence.pop_back();
    score_seq = align_original_greedy_match(temp, sequence, cache);
    sequence.push_back(seq_tail[0]);
    int delete_result = score_seq + deletion_score;
    temp.pop_back();
    score_seq = align_original_greedy_match(temp, sequence, cache);
    temp.push_back(temp_tail[0]);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[temp.size()][sequence.size()] = result;
    return result;
}

// optimisation: prune recursive search tree by the following limitation:
// if I am in state [T,S] and I found some alignment with score K
// then any other searches (e.g. we found alignment through match_mismatch but want to try delete or gap)
// need to get a score higher than K for the search to have any effect on the result
// As such, each call is given a 'get_at_least' value - the caller indicates that if this score can not be obtained
// there is no point in searching deeper, as any result will eventually be discarded anyway.
// The first call is given a get_at_least value of 'TERRIBLE_SCORE', meaning that only actual alignment scores that
// were obtained during the search place a restriction on the search 
// (otherwise a bad alignment may cache a 'terrible' value at some state, and a future better-aligned call will 
// use this invalid value in its computation, lowering an actual score which could have been used)
int align_get_at_least(string &temp,string &sequence, vector<vector<int> > &cache, int get_at_least = TERRIBLE_SCORE)
{
	//calls++;

	// if one sequence is empty, we will have no choice but to gap/delete the rest
    if(temp.empty())
        return sequence.size() * deletion_score;
    if(sequence.empty())
        return temp.size() * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cache[temp.size()][sequence.size()] != UNCACHED)
    	return cache[temp.size()][sequence.size()];

    // for the result of this recursive call to matter (improve the result for the caller)
    // it would have to get at least get_at_least points
    // in the best case, we match the entirety of the smaller string, and then gap/delete the leftover of the other
    int best_score_possible = match_score * min(temp.size(),sequence.size()) + max(deletion_score,gap_score) * abs((int)temp.size() - (int)sequence.size());
    if(best_score_possible < get_at_least)
    	return TERRIBLE_SCORE;

    string temp_tail = temp.substr(temp.size()-1,1); //temp_tail = temp.substr(1);
    string seq_tail = sequence.substr(sequence.size()-1,1);//, seq_tail = sequence.substr(1);
    // in C++ swapped functioning of tail and head - we match the strings from the right
    // as then we can pop characters form the right side in O(1) and pass the heads by reference
    // which should save us one order of time

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    temp.pop_back();
    sequence.pop_back();
    score_seq = align_get_at_least(temp, sequence, cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    temp.push_back(temp_tail[0]);
    sequence.push_back(seq_tail[0]);

    
    // this breaks here - why?
    /*if(match_mismatch_score == match_score)
    {
        cache[temp.size()][sequence.size()] = match_result;
        return match_result;
    }*/
    

    get_at_least = max(get_at_least, match_result);

    sequence.pop_back();
    score_seq = align_get_at_least(temp, sequence, cache, get_at_least - deletion_score);
    sequence.push_back(seq_tail[0]);
    int delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);

    temp.pop_back();
    score_seq = align_get_at_least(temp, sequence, cache,get_at_least - gap_score);
    temp.push_back(temp_tail[0]);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cache[temp.size()][sequence.size()] = result;
    return result;
}

