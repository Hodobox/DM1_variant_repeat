#include <iostream>
#include <fstream>
#include "window.h"

using namespace std;

const bool TESTING = true;
ifstream input;
ofstream output;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "Usage: ./bin $inputfile $outputfile\n";
        if(!TESTING) return 1;
    }

    if(argc >= 2)
    input.open(argv[1]);
    if(argc >= 3)
    output.open(argv[2]);

    vector<string> sequences = readfile(input);

    //vector< map<string,vector<pair<int,int> > > > result = window(sequences, 20, 3);

    // window multiple from 3 to 9, a1 -> a2
    vector<pair< pair<int,int>, vector< map<string,vector<pair<int,int> > > > > > result = window_multiple(sequences, {3,9}, 18, 6);
    outputdata(output, result, "repeats from 3 to 9, window size is 6 * repeat size");
    input.close();
    output.close();

    // filter it, a2 -> a3
    input.open(argv[2]);
    output.open(argv[3]);
    filter_window_data(input,output);
    input.close(); output.close();

    // print interesting from a3
    input.open(argv[3]);
    vector< pair< pair<int,int>, vector<map<string,vector<pair<int,int> > > > > > data = readdata(input);
    print_interesting_repeats(data);
}