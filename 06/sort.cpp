#include <fstream>
#include <cstdint>
#include <string>
#include <thread>
#include <cstdlib>
#include <vector>
#include <algorithm>


using uint = uint64_t;
const size_t MAX_THREADS = 2;
const size_t MAX_RAM = 8 * 1024 * 1024;
const size_t MAX_NUMBERS = MAX_RAM / 64;
const size_t MAX_THREAD_RAM = MAX_RAM / MAX_THREADS;
const size_t MAX_THREAD_NUMBERS = MAX_THREAD_RAM / 64;

size_t count_numbers(const std::string& fileName)
{
	std::ifstream in(fileName, std::ios::ate | std::ios::binary);
	return in.tellg() / sizeof(uint);
}

void unite_two_files(const std::string& fileName1, const std::string& fileName2, const std::string& outName)
{
	std::ifstream f1(fileName1, std::ios::binary);
	std::ifstream f2(fileName2, std::ios::binary);
	std::ofstream fout(outName, std::ios::trunc | std::ios::binary);
	bool fromFirst = true;
	bool fromSecond = true;
	int lastFile = 0;
	while(fromFirst || fromSecond)
	{
		uint x1, x2;
		if(lastFile != 2 && !(f1.read((char*)&x1, sizeof(uint)))) fromFirst = false;
		if(lastFile != 1 && !(f2.read((char*)&x2, sizeof(uint)))) fromSecond = false;
		if(x1 < x2 && fromFirst)
		{
			fout.write((char*)&x1, sizeof(uint));
			lastFile = 1;
		}else if(fromSecond)
		{
			fout.write((char*)&x2, sizeof(uint));
			lastFile = 2;
		}else if(fromFirst)
		{
			fout.write((char*)&x1, sizeof(uint));
			lastFile = 1;
		}
	}
	f1.close();
	f2.close();
	fout.close();
}

void unite_files(std::vector<std::string>& fileNames, const std::string& output)
{
	while(fileNames.size() > 2)
	{
		std::vector<std::string> newNames;
		for(size_t i = 0; i < fileNames.size(); i += 2)
		{
			if(i != fileNames.size() - 1)
			{
				std::string newFile = fileNames[i] + std::to_string(i) + "_";
				newNames.push_back(newFile);
				unite_two_files(fileNames[i], fileNames[i + 1], newFile);
			}
		}
		if(fileNames.size() % 2) newNames.push_back(fileNames[fileNames.size() - 1]);
		fileNames = newNames;
	}
	unite_two_files(fileNames[0], fileNames[1], output);
}

void sort_file(const std::string& fileName, size_t numbers)
{
	if(numbers <= MAX_THREAD_NUMBERS)
	{
		std::ifstream f(fileName, std::ios::binary);
		uint* num = new uint[numbers];
		f.read((char*)num, numbers * sizeof(uint));
		std::sort(num, num + numbers);
		f.close();
		std::ofstream fout(fileName, std::ios::trunc | std::ios::binary);
		fout.write((char*)num, numbers * sizeof(uint));
		fout.close();
		delete[] num;
	}else
	{
		std::ifstream f(fileName, std::ios::binary);
		size_t numberOfFiles = numbers / MAX_THREAD_NUMBERS + (numbers % MAX_THREAD_NUMBERS ? 1 : 0);
		size_t newNumbers = numbers / numberOfFiles;
		size_t residue = numbers % numberOfFiles;
		std::vector<std::string> fileNames;
		for(size_t i = 0; i < numberOfFiles; ++i)
		{
			std::string newFile = fileName + std::to_string(i);
			fileNames.push_back(newFile);
			std::ofstream fout(newFile, std::ios::trunc | std::ios::binary);
			size_t numbers = newNumbers + (residue ? 1 : 0);
			if(residue) --residue;
			uint* num = new uint[numbers];
			f.read((char*)num, numbers * sizeof(uint));
			fout.write((char*)num, numbers * sizeof(uint));
			fout.close();
			delete[] num;
			sort_file(newFile, numbers);
		}
		f.close();
		unite_files(fileNames, fileName);
	}

}

void start_sort(const std::string& fileName, size_t total)
{
	size_t numbersInFile = total / MAX_THREADS;
	size_t residue = total % MAX_THREADS;
	std::ifstream f(fileName, std::ios::binary);
	std::vector<std::thread> threads;
	std::vector<std::string> fileNames;
	for(size_t i = 0; i < MAX_THREADS; ++i)
	{
		std::string threadFile = std::to_string(i) + "_";
		fileNames.push_back(threadFile);
		std::ofstream ft(threadFile, std::ios::trunc | std::ios::binary);
		size_t numbers = numbersInFile + (residue ? 1 : 0);
		if(residue) --residue;
		for(size_t j = 0; j < numbers; j += MAX_NUMBERS)
		{
			uint* num = new uint[MAX_NUMBERS];
			f.read((char*)num, MAX_NUMBERS * sizeof(uint));
			ft.write((char*)num, MAX_NUMBERS * sizeof(uint));
			delete[] num;
		}
		ft.close();
		threads.emplace_back(std::thread(sort_file, threadFile, numbers));
	}
	f.close();
	for(size_t i = 0; i < MAX_THREADS; ++i)
	{
		threads[i].join();
	}
	threads.clear();
	unite_files(fileNames, fileName);
}

int main()
{
	std::string fileName = "data.bin";
	/*
	std::cout << "Enter file name: ";
	std::cin >> fileName;
	*/
	size_t total = count_numbers(fileName);
	start_sort(fileName, total);

	return 0;
}
