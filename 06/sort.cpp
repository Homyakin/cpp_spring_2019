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
const size_t MAX_THREAD_RAM = MAX_RAM / MAX_THREADS;
const size_t MAX_THREAD_NUMBERS = MAX_THREAD_RAM / 64;

size_t count_numbers(std::string& fileName)
{
    std::ifstream f(fileName, std::ios::in | std::ios::binary);
	size_t total = 0;
	for(uint x; f >> x; )
	{
		++total;
	}
	return total;
}

void unite_files(std::string fileName1, std::string fileName2, std::string outName)
{
    std::ifstream f1(fileName1, std::ios::out | std::ios::binary);
    std::ifstream f2(fileName2, std::ios::in | std::ios::binary);
    std::ofstream fout(outName, std::ios::out | std::ios::trunc | std::ios::binary);
    bool fromFirst = true;
    bool fromSecond = true;
    int lastFile = 0;
    while(fromFirst || fromSecond)
    {
        uint x1, x2;
        if(lastFile != 2 && !(f1 >> x1)) fromFirst = false;
        if(lastFile != 1 && !(f2 >> x2)) fromSecond = false;
        if(x1 < x2 && fromFirst)
        {
            fout << x1 << " ";
            lastFile = 1;
        }else if(fromSecond)
        {
            fout << x2 << " ";
            lastFile = 2;
        }else if(fromFirst)
        {
            fout << x1 << " ";
            lastFile = 1;
        }
    }
}
void sort_file(std::string fileName, size_t numbers)
{
    if(numbers <= MAX_THREAD_NUMBERS)
    {
        std::ifstream f(fileName, std::ios::in | std::ios::binary);
        uint* num = new uint[numbers];
        for(size_t i = 0; i < numbers; ++i)
        {
            f >> num[i];
        }
        std::sort(num, num + numbers);
        f.close();
        std::ofstream fout(fileName, std::ios::out | std::ios::trunc | std::ios::binary);
        for(size_t i = 0; i < numbers; ++i)
        {
            fout << num[i] << " ";
        }
        fout.close();
    }else
    {
        std::ifstream f(fileName, std::ios::in | std::ios::binary);
        size_t newNumbers = numbers / 2;
        int residue = numbers % 2;
        for(size_t i = 0; i < 2; ++i)
        {
            std::string newFile = fileName + std::to_string(i);
            std::ofstream fout(newFile, std::ios::out | std::ios::trunc | std::ios::binary);
            size_t numbers = newNumbers + (residue ? 1 : 0);
            if(residue) --residue;
            for(size_t j = 0; j < numbers; ++j)
            {
                uint x;
                f >> x;
                fout << x << " ";
            }
            fout.close();
            sort_file(newFile, numbers);
        }
        unite_files(fileName + "0", fileName + "1", fileName);
    }
}

void start_sort(std::string& fileName, size_t total)
{
    size_t numbersInFile = total / MAX_THREADS;
    size_t residue = total % MAX_THREADS;
    std::ifstream f(fileName, std::ios::in | std::ios::binary);
    std::vector<std::thread> threads;
    std::vector<std::string> fileNames;
    for(size_t i = 0; i < MAX_THREADS; ++i)
    {
        std::string threadFile = std::to_string(i) + "_";
        fileNames.push_back(threadFile);
        std::ofstream ft(threadFile, std::ios::out | std::ios::trunc | std::ios::binary);
        size_t numbers = numbersInFile + (residue ? 1 : 0);
        if(residue) --residue;
        for(size_t j = 0; j < numbers; ++j)
        {
            uint x;
            f >> x;
            ft << x << " ";
        }
        ft.close();
        threads.push_back(std::thread(sort_file, threadFile, numbers));
    }
    f.close();
    for(size_t i = 0; i < MAX_THREADS; ++i)
    {
        threads[i].join();
    }
    threads.clear();
    while(fileNames.size() > 1)
    {
        std::vector<std::string> newNames;
        for(size_t i = 0; i < fileNames.size(); i += 2)
        {
            if(i != fileNames.size() - 1)
            {
                std::string newFile = fileNames[i] + std::to_string(i) + "_";
                newNames.push_back(newFile);
                threads.push_back(std::thread(unite_files, fileNames[i], fileNames[i + 1], newFile));
            }
        }
        for(size_t i = 0; i < threads.size(); ++i)
        {
            threads[i].join();
        }
        threads.clear();
        if(fileNames.size() % 2) newNames.push_back(fileNames[fileNames.size() - 1]);
        fileNames = newNames;
    }

    f.open(fileNames[0], std::ios::in | std::ios::binary);
    std::ofstream fout(fileName, std::ios::out | std::ios::trunc | std::ios::binary);
    for(uint x; f >> x;)
    {
        fout << x << " ";
    }
    f.close();
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
