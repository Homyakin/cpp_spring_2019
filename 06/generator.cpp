#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>

const size_t N = 1000000;

int main()
{
	std::srand(std::time(0));
	std::ofstream f("data.bin", std::ios::out | std::ios::trunc | std::ios::binary);
	for(size_t i = 0; i < N; ++i)
	{ 
		f << std::rand() << " ";
	}
	f.close();
	return 0;
}
