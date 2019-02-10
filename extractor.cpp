#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

vector<int> tokenize(string &s)
{
    vector<int> res;
    int cur = 0;
    bool real = false;
    for(char c:s)
    {
        if(c==' ')
        {
            if(real)
                res.push_back(cur);
            real = false;
            cur = 0;
        }
        else if(isdigit(c))
        {
            real = true;
            cur = cur*10 + (c-'0');
        }
    }

    if(real) res.push_back(cur);

    return res;
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cerr << "usage: ./binary $inputPath $outputPath\n";
        return 1;
    }

    ifstream data(argv[1]);
    ofstream output(argv[2]);
    string line;

    int cnt = 0;
    vector<int> best_score;
    vector< vector< vector<int> > > best_scoring_templates;

    while(getline(data,line))
    {
        cnt++;
        if(cnt%100000==0)
            cerr << cnt << "\n";

        vector<int> numbers = tokenize(line);
        vector<int> nml = {numbers[0],numbers[1],numbers[2]};
        int seqnum = numbers[3], score = numbers[4];

        if(seqnum >= best_score.size())
        {
            best_score.resize(seqnum+1,0);
            best_scoring_templates.resize(seqnum+1);
        }

        if(score > best_score[seqnum])
        {
            best_score[seqnum] = score;
            best_scoring_templates[seqnum].clear();
        }

        if(score == best_score[seqnum])
            best_scoring_templates[seqnum].push_back(nml);

    }

    //format
    // num of sequences
    // for each sequence (in order):
    // score, num of best templates
    // n m l of each template

    output << best_score.size() << "\n";
    
    for(int i=0;i<best_score.size();++i)
    {
        output << best_score[i] << " " << best_scoring_templates[i].size() << "\n";
        for(vector<int> nml : best_scoring_templates[i])
        {
            for(int k=0;k<nml.size();++k)
            {
                if(k) output << " ";
                output << nml[k];
            }
            output << "\n";
        }

    }

    return 0;

}
