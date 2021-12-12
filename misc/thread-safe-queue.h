#ifndef THREAD_SAFE_QUEUE_
#pragma once
#define THREAD_SAFE_QUEUE_

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue(void)
		: q(), m() , c()
	{
	}

	~ThreadSafeQueue(void) { }

	void Enqueue(T t)
	{
		std::lock_guard<std::mutex> lock(m);
		q.push(t);
		c.notify_one();
	}

	T Dequeue(void)
	{
		std::unique_lock<std::mutex> lock(m);

		if (q.empty())
		{
			return T();
		}
		//while(q.empty())
		//{
		//c.wait(lock);
		//}
		T val = q.front();
		q.pop();
		return val;
	}

	// Same as dequeue, but without queue removal
	T Peek(void)
	{
		std::unique_lock<std::mutex> lock(m);

		if (q.empty())
		{
			return T();
		}
		//while(q.empty())
		//{
		//c.wait(lock);
		//}
		T val = q.front();
		return val;
	}

	bool IsQueueEmpty()
	{
		std::unique_lock<std::mutex> lock(m);
		if (q.empty())
		{
			return true;
		}
		return false;
	}

private:
	std::queue<T> q;
	mutable std::mutex m;
	std::condition_variable c;
};
#endif
