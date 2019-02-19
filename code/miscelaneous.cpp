#include "miscelaneous.h"

void count_prefixes(vector<string> candidate_templates)
{
	set<long long> prefixhashes;

	for(string &pat : candidate_templates)
	{
		long long mod[] = {1000000007,1000000009}, base[] = {313,317},hashes[]={0,0};
		for(int i=0;i<pat.size();++i)
		{
			for(int k=0;k<2;++k)
			{
				hashes[k] = hashes[k]*base[k] + pat[i];
				hashes[k] %= mod[k];
			}
			prefixhashes.insert(hashes[0]*mod[1]+hashes[1]);
		}
	}

	cout << prefixhashes.size() << " unique prefixes\n";
	
}

void rawdata_to_garbagefree()
{
    ifstream data("../sequences/real.txt");
    ofstream output("../sequences/garbagefree.txt");
    int num_sequences;
    data >> num_sequences;
    vector<string> garbagefree;
    while(num_sequences--)
    {
        string sequence;
        data >> sequence;
        string result = cut_garbage(sequence, "CTG", 6, 15);
        if(result.size() > 950)
            garbagefree.push_back(result);
    }

    output << garbagefree.size() << "\n";
    for(string &s : garbagefree)
        output << s << "\n";

}

// assumes temp is a contatenation of strings in pattern, e.g. pattern = ["A","B","C"] then temp is (A)n (B)m (C)l for some n,m,l
vector<int> template_pattern_parameters(vector<string> pattern, string &temp)
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
