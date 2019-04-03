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

// try to find small counterexample for GAL_multithread, i.e. where it differs from align_original
pair<string,string> find_counterexample()
{

    // sequence will be DNA, so made up of ACTG
    // so each character can be represented by 2 bits

    // for each sequence, we will generate all templates and test them with original and GAL
    // do this for longer and longer sequences until we get a counterexample

    string alphabet[] = {"A","C","G","T"};
    vector<string> pattern = { "CTG", "CCGCTG", "CTG" };

    for(int seqlen = 1; seqlen <= 10; ++seqlen)    
    {
        cout << seqlen << " len\n";
        for(long long seqcode = 0; seqcode < 1LL<<(seqlen*2); seqcode++)
        {
            string seq = "";
            for(int position = 0; position < seqlen; ++position)
            {
                string digit = alphabet[(seqcode >> (position*2)) & 3];
                seq = digit + seq;
            }
            
            vector<string> templates = produce_sequences_improved_outer(pattern, seqlen);

            for(int i=0;i<templates.size();++i)
            {
                string temp = templates[i];

                vector<vector<int> > cache0(temp.size()+1,vector<int>(seq.size()+1,UNCACHED));
                int score0 = align_greedymatch_multithread(temp,seq,temp.size(),seq.size(),cache0);

                //vector<vector<int> > cache1(temp.size()+1,vector<int>(seq.size()+1,UNCACHED));
                //int score1 = align_GAL_multithread(temp,seq,temp.size(),seq.size(),cache1);
                
                vector<vector<int> > cache2(temp.size()+1,vector<int>(seq.size()+1,UNCACHED));
                vector<vector<int> > GAL_cache(temp.size()+1,vector<int>(seq.size()+1,-TERRIBLE_SCORE));
                int score2 = align_GAL_careful(temp,seq,temp.size(),seq.size(),cache2, GAL_cache);
           

                if(score0 != score2)
                {
                    cout << "MISMATCH with " << seq << ", " << temp << ": " << score0 << " " << score2 << "\n";
                    return {seq, temp};
                }
            }

        }

    }

}

void analyze_counterexample(string seq, string temp)
{
    vector<vector<int> > cache0(temp.size()+1,vector<int>(seq.size()+1,UNCACHED));
    int score0 = align_greedymatch_multithread(temp,seq,temp.size(),seq.size(),cache0);

    //vector<vector<int> > cache1(temp.size()+1,vector<int>(seq.size()+1,UNCACHED));
    //int score1 = align_GAL_multithread(temp,seq,temp.size(),seq.size(),cache1);

    vector<vector<int> > cache2(temp.size()+1,vector<int>(seq.size()+1,UNCACHED));
    vector<vector<int> > GAL_cache(temp.size()+1,vector<int>(seq.size()+1,-TERRIBLE_SCORE));
    int score2 = align_GAL_careful(temp,seq,temp.size(),seq.size(),cache2, GAL_cache);
           

    cout << seq << " against " << temp << "\n";
    cout << "GM: " << score0 << "; GAL cf: " << score2 << "\n";

    cout << "GM cache:\n";

    for(int i=0;i<=temp.size();++i)
    {
        for(int k=0;k<=seq.size();++k)
        {
            if(k) cout << "\t";
            if(cache0[i][k] != UNCACHED)
            cout << cache0[i][k];
            else cout << "X";
        }
        cout << "\n";
    }
    cout << "---\n";

    cout << "GAL cf cache:\n";

    for(int i=0;i<=temp.size();++i)
    {
        for(int k=0;k<=seq.size();++k)
        {
            if(k) cout << "\t";
            if(cache2[i][k] != UNCACHED)
            cout << cache2[i][k];
            else cout << "X";
        }
        cout << "\n";
    }
    cout << "---\n";

    cout << "GAL GAL_cache:\n";

    for(int i=0;i<=temp.size();++i)
    {
        for(int k=0;k<=seq.size();++k)
        {
            if(k) cout << "\t";
            if(GAL_cache[i][k] != -TERRIBLE_SCORE)
            cout << GAL_cache[i][k];
            else cout << "X";
        }
        cout << "\n";
    }
    cout << "---\n";

}