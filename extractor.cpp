#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

vector<int> tokenize(string &s)
{
    vector<int> res;
    int cur;
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

int main()
{
    string filename;
    cin >> filename;
    ifstream data(filename);
    string line;
    vector<pair<int,vector<int>> > best;
    while(getline(data,line))
    {
        if(count(line.begin(),line.end(),',') > 0)
        {
            cout << line << "\n";
        }
        else
        {
            vector<int> numbers = tokenize(line);
            vector<int> nml = {numbers[0],numbers[1],numbers[2]};
            int seqnum = numbers[3], score = numbers[4];
            if(seqnum >= best.size())
            {
                best.resize(seqnum+1,{-1,{}});
            }
            if(best[seqnum].first < score)
            {
                best[seqnum] = {score,nml};
            }
        }
    }

    for(int i=0;i<best.size();++i)
        cout << best[i].first << " " << best[i].second[0] << " " << best[i].second[1] << " " << best[i].second[2] << "\n";

}
