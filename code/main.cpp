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

ifstream input;
ofstream output;

const bool TESTING = false;

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

void best_templates_from_raw_reads_time(vector<string> pattern, vector<string> &sequences, int test_against)
{
    int total_templates = 0, alignments_done = 0, sequences_complete = 0;
    vector<vector<int> > pair_scores;

    cerr << "Testing " << sequences.size() << " sequences against up to " << test_against << " templates\n";

    auto start = chrono::steady_clock::now();

    
    #pragma omp parallel for ordered
    for(int i=0;i<sequences.size();i++)
    {
        vector<string> templates = produce_sequences_outer_faster(pattern, sequences[i].size());
        if(TESTING)
        {
            random_shuffle(templates.begin(),templates.end());
        }
        int alignments_to_do = min((int)templates.size(),test_against);
        #pragma omp atomic
        alignments_done += alignments_to_do;
        for(int k=0;k<alignments_to_do;++k)
        {
            vector<vector<int> > cache(templates[k].size()+1,vector<int>(sequences[i].size()+1,UNCACHED));
            int score = align_GAL_multithread(templates[k],sequences[i], templates[k].size(), sequences[i].size(), cache);
            vector<int> pairscore_element = template_pattern_parameters(pattern,templates[k]);
            pairscore_element.push_back(i);
            pairscore_element.push_back(score);
            #pragma omp critical
            pair_scores.push_back(pairscore_element);
        }

        #pragma omp atomic
        sequences_complete += 1;

        if(sequences_complete%10 == 0) 
        {
            #pragma omp critical
            cerr << sequences_complete << "/" << sequences.size() << "\n";
        }

    }

    auto end = chrono::steady_clock::now();
    cerr << "Time sum for alignments: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    cerr << "Average time per alignment: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() / (alignments_done) << " ms\n";

    for(vector<int> v: pair_scores)
    {
        for(int i=0;i<v.size();++i)
        {
            if(i) output << " ";
            output << v[i];
        } output << "\n";
    }
}

void test_seqs(int n = 20, int test_against = 20)
{

	int num_sequences;
	input >> num_sequences;

	vector<string> biopattern = {"CTG", "CCGCTG", "CTG"};

	n = min(n, num_sequences);

	vector<string> sequences(n);
	for(string &seq : sequences)
		input >> seq;

    best_templates_from_raw_reads_time(biopattern, sequences, test_against);
}

string get_seq(int num)
{
    int numseq;
    input >> numseq;
    string sequence;
    for(int i=0;i<num;++i)
        input >> sequence;

    input >> sequence;
    return sequence;
}


int main(int argc, char** argv)
{
    //rawdata_to_garbagefree();

    if(argc != 3)
    {
        cerr << "Usage: ./binary $input_path $output_path\n";
        return 1;
    }

    if(TESTING)
    {
        cerr << "TESTING is set to true. For real full-data runs, set TESTING to false!\n";
    }

    input.open(argv[1]);
    output.open(argv[2]);

	//srand(47);
	//test_seqs(600, 1000000);

    /*string sequence = get_seq(386);

    vector<string> pat = produce_sequences_outer_faster({"CTG","CCGCTG","CTG"},sequence.size());
    cout << pat.size() << "\n";

    int best_score = -1;
    int done = 0;
    #pragma omp parallel for ordered
    for(int i=0;i<pat.size();++i)
    {
        string s = pat[i];
        #pragma omp atomic
        done += 1;

        if(done % 1000==0)
            cout << done << "\n";

        vector<vector<int> > cache(s.size()+1,vector<int>(sequence.size()+1,UNCACHED));
        int score = align_GAL_multithread(s,sequence,s.size(),sequence.size(),cache);
        
        #pragma omp critical
        best_score = max(best_score,score);

        if(score ==1097)
        {
            vector<int> params = template_pattern_parameters({"CTG","CCGCTG","CTG"},s);
            cout << params[0] << " " << params[1] << " " << params[2] << "\n";
        }
    }

    cout << best_score << "\n";

	return 0;
    */
    /*

	vector<string> biopattern = {"CTG", "CCGCTG", "CTG"};
	string sequence = "GCTGTTGCTGCTGCTTGCTGCTGCTTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCATGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCT";//GCTGCTGCTGCTGCTGTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGATGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCGCTGCCGCTGCCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCCGCTGCGCTGCGCTGCGCTGCCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCCGCTGCGCTGCGCTGCCGCTGCGCTGCCGCTGCCGCTGCGCTGCCGCTGCCGCAGCCGCTGCCGCTGCCGCTGCCGCTGCCGCTGCCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCTGCCGCTGCTGCTGCT";
	cin >> sequence;
	vector<string> candidate_templates = produce_sequences_outer_faster(biopattern, sequence.size());
	cerr << candidate_templates.size() << " candidates, sequence len " << sequence.size() << "\n";

    int best = -1;
    vector<int> params;

    auto start = chrono::steady_clock::now();


    int done = 0;
    vector<vector<int> > res;
    #pragma omp parallel for ordered
    for(int i=0;i<candidate_templates.size();++i)
    {
        done++;
        if(done%1000==0)
            cerr << done << "\n";
        vector<vector<int> >  cache(candidate_templates[i].size()+1,vector<int>(sequence.size()+1,UNCACHED));
        int score = align_GAL_multithread(candidate_templates[i], sequence, candidate_templates[i].size(), sequence.size(), cache);
        //cout << score << " " << template_pattern_parameters(biopattern,candidate_templates[i])[1] << "\n";
        vector<int> tmp = template_pattern_parameters(biopattern, candidate_templates[i]);
        tmp.push_back(score);
        #pragma omp critical
        res.push_back(tmp);
    }

    auto end = chrono::steady_clock::now();
	
    cerr << "Time sum for alignments: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
   
	for(int i=0;i<res.size();++i)
    {
        for(int k=0;k<res[i].size();++k)
        {
            if(k) cout << " ";
            cout << res[i][k];
        }
        cout << "\n";
    }

    */
}
