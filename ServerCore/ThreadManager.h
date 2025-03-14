#pragma once

#include <thread>
#include <functional>

/*--------------
:
:  ThreadManager
:
----------------*/

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void   Lanuch(function<void(void)> callback);
	void   Join();

	static void InitTLS();
	static void DestroyTLS();

private:
	Mutex           _lock;
	vector<thread>  _threads;
};

