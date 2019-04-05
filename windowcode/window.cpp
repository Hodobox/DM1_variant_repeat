#include "window.h"

pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > >  window(vector<string> &sequences, int window_size, int repeat_size)
{
    pair<pair<int,int>, vector<map<string,vector<pair<int,int> > > > > results;
    results.first = {repeat_size, window_size};

    for(string &seq : sequences)
    {
        map<string,vector<pair<int,int> > > observations;

        for(int i=0;i+window_size <= seq.size(); ++i)
        {

            for(int inner = 0;inner+repeat_size <= window_size; ++inner)
            {
                string repeat = seq.substr(i+inner,repeat_size);
                repeat = LSR(repeat);
                if(observations[repeat].empty() || observations[repeat].back().first != i)
                {
                    observations[repeat].push_back({i,1});
                }
                else observations[repeat].back().second++;
            }
        }

        results.second.push_back(observations);
    }

    return results;
}

vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > window_multiple(vector<string> &sequences, pair<int,int> repeat_sizes, int min_window_size, double window_size_scale)
{
    vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > all_results;
 
    for(int repeat_size = repeat_sizes.first; repeat_size <= repeat_sizes.second; ++repeat_size)
    {
        int window_size = max(min_window_size, (int)(repeat_size * window_size_scale));
        cerr << "analyzing with repeat size = " << repeat_size << ", window size = " << window_size << "\n";
        pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > observations = window(sequences, window_size, repeat_size);
        all_results.push_back(observations);
    }  

    cerr << "window_multiple done\n";
    return all_results;
}
