#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <atomic>
#include <condition_variable>


std::mutex m;
std::condition_variable dataReady;
std::atomic<bool> last_ping = false;
const int N = 500000;


void say(std::string word, bool ping)
{
	for (int i = 0; i < N; ++i)
	{
		std::unique_lock<std::mutex> lock(m);
		while (last_ping == ping)
				dataReady.wait(lock);
		if (last_ping != ping)
		{
			std::cout << word << std::endl;
			last_ping = ping;
			dataReady.notify_all();
		}
	}
}

void say_unblock(std::string word, bool ping) //эффективнее
{
	for (int i = 0; i < N; ++i)
	{
		if (last_ping != ping)
		{
			std::cout << word << std::endl;
			last_ping = ping;
		}
		else
		{
			--i;
		}
	}
}

int main()
{	
	std::thread t1(say_unblock, "ping", true);
	std::thread t2(say_unblock, "pong", false);
	t1.join();
	t2.join();
	return 0;
}