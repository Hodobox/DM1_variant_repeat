#ifndef GARBAGE_CUT_H
#define GARBAGE_CUT_H
#include "garbage_cut.cpp"
#endif

#include "align.cpp"
#include "produce_seq.cpp"
#include "miscelaneous.cpp"

#include <omp.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <random>
#include <chrono>
#include <cstring>
using namespace std;


// have not tested
// might want to replace align_original with faster implementation if its going to be used
pair<int,string> produce_sequence_and_align(vector<string> pattern, string temp)
{
    vector<string> sequences = produce_sequences_outer(pattern, temp.size());
    vector<pair<int,string> > results;
   
    for(string seq : sequences)
    {
    	vector<vector<int> > cache(seq.size()+1,vector<int>(temp.size()+1,UNCACHED));
    	results.push_back({align_original(seq,temp,cache),seq});
    }
    sort(results.begin(),results.end());
    return results.back();
}

// have not tested
vector<pair<int,string> > collapse_sequence(vector<string> pattern,string sequence)
{
    vector<pair<int,string> > answer;

    for(string elem : pattern)
    {
        int repetitions = 1;

        while(elem.size() * repetitions <= sequence.size() && sequence.substr((repetitions-1)*elem.size(),elem.size()) == elem)
            repetitions++;

        answer.push_back({repetitions-1,elem});
    }
    return answer;
}

// assumes temp is a contatenation of strings in pattern, e.g. pattern = ["A","B","C"] then temp is (A)n (B)m (C)l for some n,m,l
vector<int> template_pattern_parameters(vector<string> pattern, string temp)
{
    vector<int> parameters(pattern.size(),0);
    int pat_index=0, temp_index = 0;
    while(pat_index < pattern.size() || temp_index+pattern[pat_index].size() <= temp.size())
    {
        if(temp_index+pattern[pat_index].size() > temp.size())
            pat_index++;
        else if(temp.substr(temp_index,pattern[pat_index].size()) == pattern[pat_index])
        {
            parameters[pat_index]++;
            temp_index += pattern[pat_index].size();
        }
        else pat_index++;
    }

    return parameters;
}

// replace align_original with faster implementation if its going to be used, and produce seq with faster
vector<pair<int,string> > best_templates_from_raw_reads(vector<string> pattern, vector<string> &sequences)
{
    int lensum = 0; for(string &seq : sequences) lensum += seq.size();
    int avg_sequence_length = round( (double)lensum / sequences.size());
    vector<string> candidate_patterns = produce_sequences_outer(pattern, avg_sequence_length);
    vector<pair<int,string> > scores(candidate_patterns.size());
    
    for(int i=0;i<candidate_patterns.size();i++)
    {
        scores[i] = {0,candidate_patterns[i]};
        for(string &seq : sequences)
        {
        	vector<vector<int> > cache(candidate_patterns[i].size()+1,vector<int>(seq.size()+1,UNCACHED));
            scores[i].first += align_original(candidate_patterns[i], seq, cache);
        }
    }

    sort(scores.begin(),scores.end());
    reverse(scores.begin(),scores.end());
    return scores;
}

vector<pair<int,string> > best_templates_from_raw_reads_time(vector<string> pattern, vector<string> &sequences, int test_against)
{
    int lensum = 0; for(string &seq : sequences) lensum += seq.size();
    int avg_sequence_length = round( (double)lensum / sequences.size());

    auto start = chrono::steady_clock::now();

    vector<string> candidate_patterns = produce_sequences_outer_faster(pattern, avg_sequence_length);
    vector<pair<int,string> > scores(test_against);

    auto end = chrono::steady_clock::now();

    test_against = min(test_against,(int)candidate_patterns.size());
    cerr << "Testing " << sequences.size() << " sequences against " << test_against << " templates\n";
    cerr << "Time for pattern generation: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    cerr << candidate_patterns.size() << " of length " << avg_sequence_length << "\n";

    //random_shuffle(candidate_patterns.begin(),candidate_patterns.end());

    start = chrono::steady_clock::now();
    
    #pragma omp parallel for ordered
    for(int i=0;i<test_against;i++)
    {
        if(i > 35786 && (i%10)==0)
            cerr << i << "/" << candidate_patterns.size() << "\n";
        string candidate = candidate_patterns[i];
        scores[i] = {0,candidate_patterns[i]};
        for(int k=0;k<sequences.size();++k)
        {
            vector<vector<int> > cache(candidate_patterns[i].size()+1,vector<int>(sequences[k].size()+1,UNCACHED));
            //memset(cached,0,sizeof(cached));
            //int score = align_get_at_least_array(candidate_patterns[i], sequences[k]);
            int score = align_GAL_multithread(candidate_patterns[i],sequences[k], candidate_patterns[i].size(), sequences[k].size(), cache);
            //cout << "Template " << i << ", sequence " << k << " = " << score << "\n";
            scores[i].first += score;
        }
    }

    end = chrono::steady_clock::now();
    cerr << "Time sum for alignments: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    cerr << "Time per alignment: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() / (sequences.size() * test_against) << " ms\n";

    sort(scores.begin(),scores.end());
    reverse(scores.begin(),scores.end());
    return scores;
}

void printvector(vector<int> v)
{
    for(int x:v)
        cout << x << " ";
    
}

void test_seqs(int n = 20, int test_against = 20)
{
	ifstream data("../sequences/garbagefree.txt");

	int num_sequences;
	data >> num_sequences;

	vector<string> biopattern = {"CTG", "CCGCTG", "CTG"};

	n = min(n, num_sequences);

	vector<string> sequences(n);
	for(string &seq : sequences)
		data >> seq;

	vector<pair<int,string>> result = best_templates_from_raw_reads_time(biopattern, sequences, test_against);
	//cout << "best align " << result.first << ": "; printvector(template_pattern_parameters(biopattern,result.second));
    for(auto res : result)
    {
        cout << res.first << ", params = ";
        printvector(template_pattern_parameters(biopattern,res.second));
        cout << "\n";
    }
}



int main()
{
    //rawdata_to_garbagefree();

	srand(47);
	test_seqs(50000,50000);
	return 0;

	vector<string> biopattern = {"CTG", "CCGCTG", "CTG"};
	string sequence = "GCTGTTGCTGCTGCTTGCTGCTGCTTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCATGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCT";//GCTGCTGCTGCTGCTGTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGATGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCCGCTGCCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCCGCTGCGCTGCGCTGCGCTGCCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCGCTGCCGCTGCCGCAGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCT";
	//cin >> sequence;
	vector<string> candidate_templates = produce_sequences_outer(biopattern, sequence.size());
	cout << candidate_templates.size() << " candidates, sequence len " << sequence.size() << "\n";

	vector<vector<int> >  cache(candidate_templates[0].size()+1,vector<int>(sequence.size()+1,UNCACHED));
	auto start = chrono::steady_clock::now();
	cout << align_get_at_least(candidate_templates[0], sequence, cache) << " best align score\n";
	auto end = chrono::steady_clock::now();
}
