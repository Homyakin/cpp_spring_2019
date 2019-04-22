#include <iostream>
#include <thread>
#include <string>
#include <atomic>

std::atomic<bool> last_ping = false;
const int N = 500000;

void say(std::string word, bool ping)
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
	std::thread t1(say, "ping", true);
	std::thread t2(say, "pong", false);
	t1.join();
	t2.join();
	return 0;
}
