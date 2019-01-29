#ifndef ALIGN_H
#include "align.cpp"
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

string cut_garbage(string &sequence,string pattern, int n,int match_treshold)
{
	int index_front=-1, index_back=-1;
	string temp;
	for(int i=0;i<n;++i)
		temp += pattern;
	int score = -1;
	do
	{
		++index_front;
		string sequence_excerpt = sequence.substr(index_front,temp.size());
		vector<vector<int> > cache(temp.size()+1,vector<int>(temp.size()+1,UNCACHED));
		score = align_original_greedy_match(temp,sequence_excerpt,cache);
	} while(score < match_treshold);

    score = -1;
    do
    {
        ++index_back;
        string sequence_excerpt = sequence.substr(sequence.size()-1-index_back-temp.size(),temp.size());
        vector<vector<int> > cache(temp.size()+1,vector<int>(temp.size()+1,UNCACHED));
        score = align_original_greedy_match(temp,sequence_excerpt,cache);
    } while(score < match_treshold);

    //cout << index_front << " " << index_back << "\n";

	return sequence.substr(index_front, sequence.size()-index_front-index_back);
}
