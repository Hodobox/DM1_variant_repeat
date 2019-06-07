#include "misc.h"

string LSR(string s)
{
    string best = s;
    for(int i=0;i+1<s.size();++i)
    {
        s.push_back(s[0]);
        s = s.substr(1);
        best = min(best,s);
    }
    return best;
}

vector<string> readfile(ifstream &input)
{
    int n;
    input >> n;
    vector<string> res(n);
    for(int i=0;i<n;++i)
        input >> res[i];
    return res;
}

void outputdataset(ofstream &output, vector<map<string,vector<pair<int,int> > > >data, int repeat_size,int window_size)
{
    output << repeat_size << " " << data.size() << " " << window_size << "\n";
    for(int i=0;i<data.size();++i)
    {
        output << data[i].size() << "\n";
        for(auto x : data[i])
        {
            output << x.first << " " << x.second.size() << "\n";
            for(int i=0;i<x.second.size();++i)
            {
                if(i) output << " ";
                output << x.second[i].first << "," << x.second[i].second;
            }
            output << "\n";
        }
    }
}

void outputdata(ofstream &output,vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > >data, string msg)
{
    output << msg << "\n";
    output << data.size() << "\n";
    for(int i=0;i<data.size();++i)
        outputdataset(output, data[i].second, data[i].first.first, data[i].first.second);
}

vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > readdata(ifstream &input)
{
    string msg;
    getline(input, msg);
    cerr << "parsed file header: " << msg << "\n";

    int repeat_sizes;
    input >> repeat_sizes;

    vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > data(repeat_sizes);

    for(int i=0;i<repeat_sizes;++i)
    {
        int repeat_size, seq_count, window_size;
        input >> repeat_size >> seq_count >> window_size;
        data[i].first = {repeat_size, window_size};
        data[i].second.resize(seq_count);
        for(int k=0;k<seq_count;++k)
        {
            int numstr;
            input >> numstr;
            for(int o=0;o<numstr;++o)
            {
                string repeat;
                int num_occurences;
                input >> repeat >> num_occurences;
                for(int z=0;z<num_occurences;++z)
                {
                    int pos,score;
                    input >> pos;
                    input.ignore();
                    input >> score;
                    data[i].second[k][repeat].push_back({pos,score});
                }
            }
        }
    }

    return data;
}

bool interesting(int repeat_size, int window_size, vector<pair<int,int> > occurences)
{
    // prototype rough heuristic thing: a repeat is interesting if it covers at least 90% of its window 4 times in a row
    double covers = 0.9*(window_size-repeat_size+1);

    for(int i=0;i+4<occurences.size();++i)
    {
        if(occurences[i].second < covers) continue;
        for(int k=0;k<4;++k)
        {
            if(occurences[i+k].first + 1!= occurences[i+k+1].first || occurences[i+k+1].second < covers) 
                break;
        
            if(k==3)
                return true;
        }
    }

    return false;

}

void filter_window_data(ifstream &input, ofstream &output)
{
    vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > data = readdata(input);

    int repeat_sizes = data.size();

    vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > filtered(repeat_sizes);

    set<string> interesting_repeats;

    // for each repeat size
    for(int i=0;i<repeat_sizes;++i)
    {
        int repeat_size = data[i].first.first, window_size = data[i].first.second;
        filtered[i].first = {repeat_size, window_size};
        filtered[i].second.resize(data[i].second.size());
        // for each sequence
        for(int k=0;k<data[i].second.size();++k)
        {
            //for each repeat
            for(auto &repeat : data[i].second[k])
            {
                if( interesting(repeat_size, window_size, repeat.second) )
                {
                    interesting_repeats.insert(repeat.first);
                    filtered[i].second[k][repeat.first] = repeat.second;
                }
            }
        }
    }

    // for each repeat size
    for(int i=0;i<repeat_sizes;++i)
    {
        int repeat_size = data[i].first.first, window_size = data[i].first.second;
        filtered[i].first = {repeat_size, window_size};
        filtered[i].second.resize(data[i].second.size());
        // for each sequence
        for(int k=0;k<data[i].second.size();++k)
        {
            //for each repeat
            for(auto &repeat : data[i].second[k])
            {
                if( interesting_repeats.find(repeat.first) != interesting_repeats.end() )
                {
                    filtered[i].second[k][repeat.first] = repeat.second;
                }
            }
        }
    }

    string headermsg;
    input.seekg(0, input.beg);
    getline(input, headermsg);
    outputdata(output, filtered, headermsg);
}

void print_interesting_repeats(vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > &data)
{
    set<string> repeats;

    for(int i=0;i<data.size();++i)
        for(int k=0;k<data[i].second.size();++k)
            for(auto &repeat : data[i].second[k])
                repeats.insert(repeat.first);

    for(auto &repeat : repeats)
        cout << repeat << "\n";
}