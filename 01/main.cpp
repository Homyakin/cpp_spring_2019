#include <iostream>
#include "numbers.dat"
#include <vector>

int main(int argc, char* argv[])
{
	if(argc == 0 || argc % 2 != 0)
	{
		std::cout << -1;
		return 0;
	}
	std::vector<int> first;
	std::vector<int> second;
	for(int i = 0; i < argc; i+=2)
	{
		if(argv[i] <= argv[i+1]
		{
			
		}
	}
	for(int i = 0; i < Size; ++i)
	{
		std::cout << Data[i] << " ";
	}
	std::cout << Size;
	return 0;
}
