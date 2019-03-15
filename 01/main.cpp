#include <iostream>
#include "numbers.dat"
#include <vector>
#include <cstdlib>



int main(int argc, char* argv[])
{
	if(argc == 0 || argc % 2 != 0)
	{
		std::cout << -1;
		return 0;
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
	for(int i = 0; i < Size; ++i)
	{
		std::cout << Data[i] << " ";
	}
	std::cout << Size;
	return 0;
}
