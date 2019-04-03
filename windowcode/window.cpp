#include "window.h"

vector<map<string, vector<pair<int,int> > > > window(vector<string> &sequences, int window_size, int repeat_size)
{
    vector<map<string,vector<pair<int,int> > > > results;

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

        results.push_back(observations);
    }

    return results;
}
