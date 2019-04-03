#include <iostream>
#include <fstream>
#include "window.h"

using namespace std;

const bool TESTING = true;
ifstream input;
ofstream output;

vector<string> readfile()
{
    int n;
    input >> n;
    vector<string> res(n);
    for(int i=0;i<n;++i)
        input >> res[i];
    return res;
}

void outputdata(vector<map<string,vector<pair<int,int> > > >data)
{
    output << data.size() << "\n";
    for(int i=0;i<data.size();++i)
    {
        output << data[i].size() << "\n";
        for(auto x : data[i])
        {
            output << x.first << "\n";
            output << x.second.size() << "\n";
            for(int i=0;i<x.second.size();++i)
            {
                if(i) output << " ";
                output << x.second[i].first << "," << x.second[i].second;
            }
            output << "\n";
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "Usage: ./bin $inputfile $outputfile\n";
        if(!TESTING) return 1;
    }

    input.open(argv[1]);
    output.open(argv[2]);

    vector<string> sequences = readfile();

    vector< map<string,vector<pair<int,int> > > > result = window(sequences, 20, 3);

    outputdata(result);

}