#include "ThreadPool.h"
#include <iostream>
using namespace std;

ThreadPool::ThreadPool(int n)
{
	num_Threads = n;
	Infinite_loop_function();
	
}

ThreadPool::~ThreadPool()
{
	shutdown();
}

void ThreadPool::Infinite_loop_function()
{
	for (unsigned int i = 0u; i < num_Threads; ++i)
	{
		m_Threads.emplace_back([=] {
				while (true) {
					Task task;
					{
						std::unique_lock <std::mutex> umutex(m_mutex);
						m_condition_var.wait(umutex, [=] { return m_stopping || !m_Tasks.empty(); });
						if (m_Tasks.empty() && m_stopping){
							break;
						}

						task = std::move(m_Tasks.front());
						m_Tasks.pop(); 
						//std::cout << std::this_thread::get_id() << std::endl;
					}
   					task();
				}
		}); 
	}
}
void ThreadPool::shutdown() noexcept
{
	{
		std::unique_lock <std::mutex> umutex(m_mutex);
		m_stopping = true;
	}
	m_condition_var.notify_all();
	for (thread& thread : m_Threads) {
		thread.detach();
	}
}

void ThreadPool::Init()
{
	if (initialized == false) {
		thread_pool = new ThreadPool(thread::hardware_concurrency());
		initialized = true;
	}
}

void ThreadPool::enqueue(Task task)
{
	{
		std::unique_lock<std::mutex> lock{ m_mutex };
		m_Tasks.emplace(std::move(task));
	}
	m_condition_var.notify_one();
}
