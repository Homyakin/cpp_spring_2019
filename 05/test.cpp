#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <string>

std::condition_variable dataReady;
std::mutex m;
bool last_ping = false;
bool data = false;
const int N = 500000;

void say(std::string word, bool ping)
{
	for (int i = 0; i < N; ++i)
	{
		if (last_ping != ping)
		{
			std::lock_guard<std::mutex> lock(m);
			std::cout << word << std::endl;
			last_ping = ping;
		}
		else
		{
			--i;
		}
	}
	data = true;
	dataReady.notify_one();
}

int main()
{
	std::unique_lock<std::mutex> lock(m);
	std::thread t1(say, "ping", true);
	std::thread t2(say, "pong", false);
	while (!data)
		dataReady.wait(lock);
	t1.join();
	t2.join();
	return 0;
}