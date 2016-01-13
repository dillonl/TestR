#ifndef RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP
#define RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP

#include "SparseKmerSet.hpp"
#include "utils/ThreadPool.hpp"

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include <concurrentqueue.h>

namespace rufus
{
	class KmerSetManager : private boost::noncopyable
	{
	public:
		typedef std::shared_ptr< KmerSetManager > SharedPtr;
		KmerSetManager() : m_kmer_queue(0), m_stopped(false)
		{
			m_kmer_set = std::make_shared< SparseKmerSet >();
			init();
		}

		~KmerSetManager()
		{
			stop();
		}

		inline void addKmer(InternalKmer internalKmer)
		{
			if (!m_stopped)
			{
				m_kmer_queue.enqueue(internalKmer);
				this->m_condition.notify_one();
			}
		}

		void stop()
		{
			m_stopped = true;
			this->m_condition.notify_one();
		}

		SparseKmerSet::SharedPtr getKmerSet()
		{
			return m_kmer_set;
		}

	private:
		void init()
		{
			start();
		}

		void start()
		{
			ThreadPool::Instance()->enqueue([this]()
			{
				auto internalKmer = 0;
				for (;;)
				{
					std::unique_lock< std::mutex > lock(this->m_queue_lock);
					// std::cout << "waiting" << std::endl;
					this->m_condition.wait(lock, [this] { return this->m_stopped || (this->m_kmer_queue.size_approx() > 0); });
					// std::cout << "done waiting" << std::endl;
					if (this->m_stopped && (this->m_kmer_queue.size_approx() == 0)) { return; }
					while (this->m_kmer_queue.size_approx() > 0)
					{
						// std::cout << "getting from queue: " << internalKmer << std::endl;
						if (m_kmer_queue.try_dequeue(internalKmer))
						{
							// std::cout << "got from queue1: " << internalKmer << std::endl;
							m_kmer_set->addKmer(internalKmer);
							// std::cout << "got from queue2" << std::endl;
						}
					}
				}
			});
		}

		// boost::lockfree::queue< InternalKmer > m_kmer_queue;
		moodycamel::ConcurrentQueue< InternalKmer > m_kmer_queue;

		std::mutex m_queue_lock;
		std::condition_variable m_condition;
		bool m_stopped;

		SparseKmerSet::SharedPtr m_kmer_set;

	};
}

#endif // RUFUS_CORE_CONTAINERS_KMERSETMANAGER_HPP
