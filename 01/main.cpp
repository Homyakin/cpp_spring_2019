#include <iostream>
#include "numbers.dat"
#include <vector>
#include <cstdlib>
#include <utility>
#include <cmath>


using pair_ii = std::pair<int, int>;

int main(int argc, char* argv[])
{
	if(argc == 1 || argc % 2 == 0)
	{
		return -1;
	}
	
	std::vector<pair_ii> interval;
	int m = 0;
	for(int i = 1; i < argc; i+=2)
	{
		int a = std::atoi(argv[i]), b = std::atoi(argv[i + 1]);
		if(a <= b)
		{
			interval.push_back(std::make_pair(a, b));
			m = (b > m ? b : m);
		}else
			std::cout << "0 ";
	}
	
	std::vector<int> simple;
	std::vector<int> Data2;
	simple.push_back(2);
	int k = 0;
	while(Data[k] < 2) ++k;
	while(Data[k] == 2)
	{
		++k;
		Data2.push_back(2);
	}
	for(int i = 3; i <= m; i+=2)
	{
		bool sim = true;
		for(int j = 0; j < simple.size() && simple[j] <= std::sqrt(i); ++j)
		{
			if(i % simple[j] == 0)
			{
				sim = false;
				break;
			}
		}
		if(sim)
		{
			while(Data[k] < i) ++k;
			while(Data[k] == i)
			{
				++k;
				Data2.push_back(i);
			}
			simple.push_back(i);
		}
	}
	
	for(int i = 0; i < interval.size(); ++i)
	{
		int j = 0;
		int cnt = 0;
		
		while(j < Data2.size() && Data2[j] < interval[i].first) ++j;
		while(j < Data2.size() && Data2[j] <= interval[i].second)
		{
			
			++cnt;
			++j;
		}
		std::cout << cnt << " ";
	}
	return 0;
}
