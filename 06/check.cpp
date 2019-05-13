#include <fstream>
#include <cstdint>
#include <iostream>
using uint = uint64_t;

int main()
{
	std::ifstream f("data.bin", std::ios::in | std::ios::binary);
	uint x1, x2;
	f.read((char*)&x1, sizeof(uint));
	while(f.read((char*)&x2, sizeof(uint)))
	{
		if(x2 < x1) 
		{
			std::cout << "false" << std::endl;
			break;
		}
		x1 = x2;
	}
	f.close();
	return 0;
}
