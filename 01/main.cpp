#include <iostream>
#include "numbers.dat"
#include <vector>
#include <cstdlib>
#include <cmath>


int main(int argc, char* argv[])
{
	if(argc == 0 || argc % 2 != 0)
	{
		return -1;
	}
	std::vector<int> first;
	std::vector<int> second;
	int intervals = 0;
	for(int i = 0; i < argc; i+=2)
	{
		int a = std::atoi(argv[i]), b = std::atoi(argv[i + 1]);
		if(a <= b)
		{
			if(intervals)
			{
				int j = 0;
				while(j < intervals && first[j] < a) ++j;
				int k = j;
				while(k < intervals && second[k] < b) ++k;
				bool f = false, s = false;
				if(!(a >= first[j] && a <= second[j]))
				{
					++j;
					f = true;
				}
				if(!(b >= first[k] && b <= second[k]))
				{
					++k;
					s = true;
				}
				if(j < intervals && f)
					first.insert(first.begin() + j,a);
				else if(f)
					first.push_back(a);
				if(k < intervals && s)
					second.insert(first.begin() + k, b);
				else if(s)
					second.push_back(b);
				if(k - j && (s || f))
				{
					first.erase(first.begin() + j + 1, first.begin() + k - 1);
				}
				
			}else
			{
				first.push_back(a);
				second.push_back(b);
				++intervals;
			}
		}
	}
	std::vector<int> simple;
	simple.push_back(2);
	int m = second[intervals - 1];
	for(int i = 3; i <= m; i+=2)
	{
		bool sim = true;
		for(int j = 0; j < simple.size() && simple[j] <= sqrt(i); ++j)
		{
			if(i % j == 0)
			{
				sim = false;
				break;
			}
		}
		if(sim) simple.push_back(i);
	}
	int cnt = 0;
	for(int i = 0; i < first.size(); ++i)
	{
		int j = 0;
		while(Data[j] < first[i]) ++j;
		while(Data[j] <= second[j])
		{
			for(int k = 0; k < simple.size(); ++k)
			{
				if(simple[k] == Data[j])
				{
					++cnt;
					break;
				}
			}
			++j;
		}
	}
	std::cout << cnt;
	return 0;
}
