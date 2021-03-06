#include "garbage_cut.h"
#include "align.h"
#include "produce.h"
#include "miscelaneous.h"

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

const bool TESTING = true;

// first by sequence number, then by parameters
bool pairscorecmp(vector<int> a,vector<int> b)
{
    if(a[3] != b[3])
        return a[3] < b[3];

    return a < b;   
}

void best_templates_from_raw_reads_time(vector<string> pattern, vector<string> &sequences, int test_against)
{
    int total_templates = 0, alignments_done = 0, sequences_complete = 0;
    vector<vector<int> > pair_scores;

    cerr << "Testing " << sequences.size() << " sequences against up to " << test_against << " templates\n";

    auto start = chrono::steady_clock::now();
    
    //#pragma omp parallel for ordered
    for(int i=0;i<sequences.size();i++)
    {
        vector<string> templates = produce_sequences_improved_outer(pattern, sequences[i].size());
        if(TESTING)
        {
            //srand(47);
            random_shuffle(templates.begin(),templates.end());
        }
        int alignments_to_do = min((int)templates.size(),test_against);
        //#pragma omp atomic
        alignments_done += alignments_to_do;

        //align_gm_mt_opt_outer(pattern, i, sequences[i],templates,alignments_to_do,pair_scores);
        //continue;
        for(int k=0;k<alignments_to_do;++k)
        {
            if(k % 1000 == 0)
            {
                //#pragma omp critical
                cerr << k << "/" << alignments_to_do << "\n";
            }
            
            vector<vector<int> > cache(templates[k].size()+1, vector<int>(sequences[i].size()+1, UNCACHED));
            int score = align_greedymatch_multithread(templates[k], sequences[i], templates[k].size(), sequences[i].size(), cache);
            vector<int> pairscore_element = template_pattern_parameters(pattern,templates[k]);
            pairscore_element.push_back(i);
            pairscore_element.push_back(score);
            //#pragma omp critical
            pair_scores.push_back(pairscore_element);
            
            //vector<vector<pair<int,int> > > cache(templates[k].size()+1,vector<pair<int,int> >(sequences[i].size()+1,{UNCACHED,-TERRIBLE_SCORE}));
            //vector<vector<int> > GAL_cache(templates[k].size()+1,vector<int>(sequences[i].size()+1,-TERRIBLE_SCORE));
            //int score = align_GAL_careful_pair(templates[k],sequences[i],templates[k].size(),sequences[i].size(),cache);
            //vector<int> pairscore_element = template_pattern_parameters(pattern,templates[k]);
            //pairscore_element.push_back(i);
            //pairscore_element.push_back(score);
            //#pragma omp critical
            //pair_scores.push_back(pairscore_element);
        }

       // #pragma omp atomic
        sequences_complete += 1;

        if(sequences_complete%10 == 0) 
        {
           // #pragma omp critical
            cerr << sequences_complete << "/" << sequences.size() << "\n";
        }
        
    }

    auto end = chrono::steady_clock::now();
    cerr << "Time sum for alignments: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " milli sec\n";
    cerr << "Average time per alignment: " << chrono::duration_cast<chrono::microseconds>(end - start).count() / (alignments_done) << " micro sec\n";

    if(TESTING)
    {
        sort(pair_scores.begin(),pair_scores.end(),pairscorecmp);
    }

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
        if(!TESTING) return 1;
        else cerr << "WARNING: INVALID ARGUMENTS\n";
    }

    if(TESTING)
    {
        cerr << "TESTING is set to true. For real full-data runs, set TESTING to false!\n";
    }

    input.open(argv[1]);
    output.open(argv[2]);
	srand(47);
	test_seqs(1, 1);
    cerr << CALLS_DELETE_TESTVAR << "\n";

    //pair<string,string> ctrexample = find_counterexample();

    //analyze_counterexample(ctrexample.first, ctrexample.second);

    return 0;
}
