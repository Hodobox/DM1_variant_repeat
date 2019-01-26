#include <iostream>
#include <cmath>
#include "align.cpp"
#include "produce_seq.cpp"
#include "miscelaneous.cpp"
#include <fstream>
#include <random>

#include <chrono>
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

// replace align_original with faster implementation if its going to be used
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

// for testing purposes
vector<pair<int,string> > best_templates_from_raw_reads_time(vector<string> pattern, vector<string> &sequences, int test_against)
{
    int lensum = 0; for(string &seq : sequences) lensum += seq.size();
    int avg_sequence_length = round( (double)lensum / sequences.size());

	auto start = chrono::steady_clock::now();

    vector<string> candidate_patterns = produce_sequences_outer(pattern, avg_sequence_length);
    vector<pair<int,string> > scores(candidate_patterns.size());

    auto end = chrono::steady_clock::now();

    cout << "Time for pattern generation: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    random_shuffle(candidate_patterns.begin(),candidate_patterns.end());

    start = chrono::steady_clock::now();
    
    for(int i=0;i<test_against;i++)
    {
        scores[i] = {0,candidate_patterns[i]};
        for(int k=0;k<sequences.size();++k)
        {
        	string seq = sequences[k];
        	vector<vector<int> > cache(candidate_patterns[i].size()+1,vector<int>(seq.size()+1,UNCACHED));
            int score = align_get_at_least(candidate_patterns[i], seq, cache);
            cout << "Template " << i << ", sequence " << k << " = " << score << "\n";
            scores[i].first += score;
        }
    }

    end = chrono::steady_clock::now();

    cout << "Time sum for alignments: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    cout << "Time per alignment: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() / (sequences.size() * test_against) << " ms\n";

    sort(scores.begin(),scores.end());
    reverse(scores.begin(),scores.end());
    return scores;
}

void test_seqs(int n = 20, int test_against = 20)
{
	ifstream data("../sequences/real.txt");

	int num_sequences;
	data >> num_sequences;

	vector<string> biopattern = {"CTG", "CCGCTG", "CTG"};

	n = min(n, num_sequences);

	cout << "Testing " << n << " sequences against " << test_against << " templates\n";

	vector<string> sequences(n);
	for(string &seq : sequences)
		data >> seq;

	pair<int,string> result = best_templates_from_raw_reads_time(biopattern, sequences, test_against)[0];
	cout << "best align " << result.first << "\n";
	//cout << result.second << "\n";
}

int main()
{
	srand(47);
	test_seqs(20,20);
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
