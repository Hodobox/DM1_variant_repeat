#include <map>
#include <vector>
#include <algorithm>
using namespace std;

const int UNCACHED = -(1 << 15);
// should be lower than maximum sequence length * 2
const int TERRIBLE_SCORE = -(1 << 12); 
// match_score should be the only positive score, everything else should be negative
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

int align_GAL_multithread(string &temp,string &sequence, int tempsize,int seqsize, vector<vector<int> > &cache, int get_at_least = TERRIBLE_SCORE)
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

    char temp_tail = temp[tempsize-1];
    char seq_tail = sequence[seqsize-1];

    int match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int score_seq;

    score_seq = align_GAL_multithread(temp, sequence, tempsize-1,seqsize-1, cache, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    
    // this breaks here - why?
    /*if(match_mismatch_score == match_score)
    {
        cache[temp.size()][sequence.size()] = match_result;
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



int16_t align_get_at_least_short(string &temp,string &sequence, vector<vector<int16_t> > &cache, int16_t get_at_least = TERRIBLE_SCORE)
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
    int16_t best_score_possible = match_score * min(temp.size(),sequence.size()) + max(deletion_score,gap_score) * abs((int)temp.size() - (int)sequence.size());
    if(best_score_possible < get_at_least)
        return TERRIBLE_SCORE;

    string temp_tail = temp.substr(temp.size()-1,1); //temp_tail = temp.substr(1);
    string seq_tail = sequence.substr(sequence.size()-1,1);//, seq_tail = sequence.substr(1);
    // in C++ swapped functioning of tail and head - we match the strings from the right
    // as then we can pop characters form the right side in O(1) and pass the heads by reference
    // which should save us one order of time

    int16_t match_mismatch_score = (temp_tail == seq_tail) ? match_score : mismatch_score;

    int16_t score_seq;

    temp.pop_back();
    sequence.pop_back();
    score_seq = align_get_at_least_short(temp, sequence, cache, get_at_least - match_mismatch_score);
    int16_t match_result = score_seq + match_mismatch_score;
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
    score_seq = align_get_at_least_short(temp, sequence, cache, get_at_least - deletion_score);
    sequence.push_back(seq_tail[0]);
    int16_t delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);

    temp.pop_back();
    score_seq = align_get_at_least_short(temp, sequence, cache,get_at_least - gap_score);
    temp.push_back(temp_tail[0]);
    int16_t gap_result = score_seq + gap_score;

    int16_t result = max(gap_result,max(match_result,delete_result));
    cache[temp.size()][sequence.size()] = result;
    return result;
}


int arraycache[1501][1501];
bool cached[1501][1501];

int align_get_at_least_array(string &temp,string &sequence, int get_at_least = TERRIBLE_SCORE)
{
    //calls++;

    // if one sequence is empty, we will have no choice but to gap/delete the rest
    if(temp.empty())
        return sequence.size() * deletion_score;
    if(sequence.empty())
        return temp.size() * gap_score;

    // if we have computed the best alignment for template[0...current_size] , sequence[0....current_size], then return it
    if(cached[temp.size()][sequence.size()])
        return arraycache[temp.size()][sequence.size()];

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
    score_seq = align_get_at_least_array(temp, sequence, get_at_least - match_mismatch_score);
    int match_result = score_seq + match_mismatch_score;
    temp.push_back(temp_tail[0]);
    sequence.push_back(seq_tail[0]);

    
    // this breaks here - why?
    /*if(match_mismatch_score == match_score)
    {
        arraycache[temp.size()][sequence.size()] = match_result;
        return match_result;
    }*/
    

    get_at_least = max(get_at_least, match_result);

    sequence.pop_back();
    score_seq = align_get_at_least_array(temp, sequence, get_at_least - deletion_score);
    sequence.push_back(seq_tail[0]);
    int delete_result = score_seq + deletion_score;

    get_at_least = max(get_at_least, delete_result);

    temp.pop_back();
    score_seq = align_get_at_least_array(temp, sequence, get_at_least - gap_score);
    temp.push_back(temp_tail[0]);
    int gap_result = score_seq + gap_score;

    int result = max(gap_result,max(match_result,delete_result));
    cached[temp.size()][sequence.size()] = true;
    arraycache[temp.size()][sequence.size()] = result;
    return result;
}

