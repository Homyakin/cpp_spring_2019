#include <iostream>
#include "numbers.dat"

int main(int argc, char* argv[])
{
	if(argc == 0 || argc % 2 != 0)
	{
		std::cout << -1;
		return 0;
	}
	
	for(int i = 0; i < Size; ++i)
	{
		//std::cout << Data[i] << " ";
	}
	std::cout << Size;
	return 0;
}
