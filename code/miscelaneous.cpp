#include <iostream>
#include <set>
#include <vector>

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