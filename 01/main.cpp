#include <iostream>
#include "numbers.dat"
#include <vector>
#include <cstdlib>
#include <cmath>


void create_simple_data(std::vector<int> & Data2)
{
	std::vector<int> simple;
	simple.push_back(2);
	int k = 0;
	while(Data[k] < 2) ++k;
	while(Data[k] == 2)
	{
		++k;
		Data2.push_back(2);
	}
	for(int i = 3; i <= Data[Size - 1]; i += 2)
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
}

int count_simple(const std::vector<int> & Data2, int a, int b)
{
	if(a > b) return 0;
	int j = 0;
	int cnt = 0;
	while(j < Data2.size() && Data2[j] < a) ++j;
	while(j < Data2.size() && Data2[j] <= b)
	{
		++cnt;
		++j;
	}
	return cnt;
}

int main(int argc, char* argv[])
{
	if(argc == 1 || argc % 2 == 0)
	{
		return -1;
	}
	
	std::vector<int> Data2;
	create_simple_data(Data2);
	
	for(int i = 1; i < argc; i += 2)
		std::cout << count_simple(Data2, std::atoi(argv[i]), std::atoi(argv[i + 1])) << std::endl;
	
	return 0;
}
