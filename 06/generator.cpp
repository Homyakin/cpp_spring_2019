#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>

const size_t N = 10000000;

int main()
{
	std::srand(std::time(0));
	std::ofstream f("data.bin", std::ios::out | std::ios::trunc | std::ios::binary);
	for(size_t i = 0; i < N; ++i)
	{
		uint64_t x = std::rand();
		f.write((char*)&x, sizeof(uint64_t));
	}
	f.close();
	return 0;
}
