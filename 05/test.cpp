#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>


std::condition_variable dataReady;
std::mutex m;
bool last_ping = false;
int pong_count = 0;
const int N = 500000;

void ping()
{
	for (int i = 0; i < N; ++i)
	{
		if (!last_ping)
		{
			std::lock_guard<std::mutex> lock(m);
			std::cout << "ping\n";
			last_ping = true;
		}
		else
		{
			--i;
		}
	}
}

void pong()
{
	for(pong_count = 0; pong_count < N; ++pong_count)
	{
		if (last_ping)
		{
			std::lock_guard<std::mutex> lock(m);
			std::cout << "pong\n";
			last_ping = false;
		}
		else
		{
			--pong_count;
		}
	}
	dataReady.notify_all();
}

int main()
{
	std::unique_lock<std::mutex> lock(m);
	std::thread t1(ping);
	std::thread t2(pong);
	while (pong_count != N)
		dataReady.wait(lock);
	t1.join();
	t2.join();
	return 0;
}